#include "sndaudiodevice.h"
#include "common/common.h"
#include "common/log.h"

#define SUCCESS 0
#define FAILED -1
#define BUFSIZE (1024 * 4)
#define MODULE_TAG "SNDAudioDevice"

SNDAudioDevice::SNDAudioDevice() :
    mType(AudioDevice_TypeNone)
{

}

int SNDAudioDevice::open(AudioDevice::AudioDeviceType_t type, AudioDevice::AudioFormat_t &format)
{
    int ret = SUCCESS;
    bool hdmiin = (type == AudioDevice_TypeHdmiIn) ? true:false;
    snd_pcm_stream_t stream = (type == AudioDevice_TypeSpeeker) ? SND_PCM_STREAM_PLAYBACK : SND_PCM_STREAM_CAPTURE;
    bool find_device = false;
    mType = type;

    if(type == AudioDevice_TypeNone) {
        ret = FAILED;
        goto RET;
    }

    findAllDevices(stream);

    if(mDevices.count() <= 0) {
        RLOGE("no device found");
        ret = FAILED;
        goto RET;
    }

    if(hdmiin) {
        int hdmiin_present = 0;
        int rate = 0;
        if (get_hdmiin_audio_info("audio_present", &hdmiin_present)) {
            if (!hdmiin_present) {
                RLOGE("hdmiin audio is no present, don't open hdmiin sound");
                ret = FAILED;
                goto RET;
            }
        }
        if (get_hdmiin_audio_info("audio_rate", &rate)) {
            if (rate <= 0) {
                RLOGE("hdmiin audio rate no available");
                ret = FAILED;
                goto RET;
            }
        }

        format.sample_rate = rate;
        for(int i = 0;i<mDevices.count();i++) {
            RLOGD("name :%s\n",mDevices.at(i).name.c_str());
            if(mDevices.at(i).name.find("hdmiin") != mDevices.at(i).name.npos) {
                RLOGE("find hdmiin device\n");
                mDevinfo = mDevices.at(i);
                find_device = true;
                break;
            }
        }
    } else {
        mDevinfo = mDevices.at(0);
        find_device = true;
    }

    if(!find_device) {
        RLOGE("not find device");
        ret = FAILED;
        goto RET;
    }

    RLOGD("hw %d:%d %s\n",mDevinfo.card,mDevinfo.device,mDevinfo.name.c_str());

    ret = openSnd(stream,format);
RET:
    return ret;
}

void SNDAudioDevice::close()
{
    if(mHandle) {
        snd_pcm_drain(mHandle);
        snd_pcm_close(mHandle);
        mHandle = nullptr;
    }
}

int SNDAudioDevice::read(void *buf)
{
    int ret = SUCCESS;
    int64_t size = 0;
    if(!mHandle || mType == AudioDevice_TypeNone || mType == AudioDevice_TypeSpeeker) {
        RLOGE("device can not read");
        ret = FAILED;
        return ret;
    }
    int avail = snd_pcm_avail_update(mHandle);
    if (avail > 0) {
        if (avail > BUFSIZE)
            avail = BUFSIZE;

        size = snd_pcm_readi(mHandle, buf, avail);
    }
    if(size <= 0) {
        //RLOGE("read err ocur %s",strerror(errno));
        if(size == -EPIPE) {
            snd_pcm_prepare(mHandle);
            size = snd_pcm_readi(mHandle, buf, avail);
        }
    }

    return size;
}

int SNDAudioDevice::write(void *buf, int size)
{
    int ret = SUCCESS;
    int write_sz = 0;
    if(!mHandle || size <= 0 || mType == AudioDevice_TypeNone || mType != AudioDevice_TypeSpeeker) {
        RLOGE("device can not write");
        ret = FAILED;
        return ret;
    }

    int avail = snd_pcm_avail_update(mHandle);

    if (avail > 0) {
        if (avail > BUFSIZE)
            avail = BUFSIZE;
        write_sz = snd_pcm_writei(mHandle, buf, avail);
    }

    return write_sz;
}

int SNDAudioDevice::wait(int timeout)
{
    if(!mHandle)
        return FAILED;
    return snd_pcm_wait(mHandle, timeout);
}

void SNDAudioDevice::findAllDevices(snd_pcm_stream_t stream)
{
    snd_ctl_t *handle;
    int card, err, dev, idx;
    snd_ctl_card_info_t *info;
    snd_pcm_info_t *pcminfo;
    snd_ctl_card_info_alloca(&info);
    snd_pcm_info_alloca(&pcminfo);

    card = -1;
    if (snd_card_next(&card) < 0 || card < 0)
    {
        RLOGE("no soundcards found...\n");
        return;
    }

    mDevices.clear();

    while (card >= 0)
    {
        char name[32];
        sprintf(name, "hw:%d", card);
        if ((err = snd_ctl_open(&handle, name, 0)) < 0)
        {
            RLOGE("control open (%i): %s\n", card, snd_strerror(err));
            goto next_card;
        }
        if ((err = snd_ctl_card_info(handle, info)) < 0)
        {
            RLOGE("control hardware info (%i): %s\n", card, snd_strerror(err));
            snd_ctl_close(handle);
            goto next_card;
        }
        dev = -1;
        while (1)
        {
            unsigned int count;
            if (snd_ctl_pcm_next_device(handle, &dev) < 0)
                RLOGE("snd_ctl_pcm_next_device\n");

            if (dev < 0)
                break;

            snd_pcm_info_set_device(pcminfo, dev);
            snd_pcm_info_set_subdevice(pcminfo, 0);
            snd_pcm_info_set_stream(pcminfo, stream);
            if ((err = snd_ctl_pcm_info(handle, pcminfo)) < 0)
            {
                if (err != -ENOENT)
                    RLOGE("control digital audio info (%i): %s\n", card, snd_strerror(err));
                continue;
            }
            RLOGD("card %i: %s [%s], device %i: %s [%s]\n",
                  card, snd_ctl_card_info_get_id(info), snd_ctl_card_info_get_name(info),
                  dev,
                  snd_pcm_info_get_id(pcminfo),
                  snd_pcm_info_get_name(pcminfo));

            AudioDeviceInfo_t dev_info;
            dev_info.card = card;
            dev_info.device = dev;
            dev_info.id = snd_ctl_card_info_get_id(info);
            dev_info.name = snd_ctl_card_info_get_name(info);
            mDevices.push_back(dev_info);
        }
        snd_ctl_close(handle);
next_card:
        if (snd_card_next(&card) < 0)
        {
            RLOGE("snd_card_next\n");
            break;
        }
    }
}

int SNDAudioDevice::openSnd(snd_pcm_stream_t stream, AudioFormat_t &format)
{
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_sw_params_t *sw_params;
    const char *dirname = "CAPTURE";
    int err;

    uint32_t rate = format.sample_rate;
    uint32_t channels = format.channels;
    snd_pcm_format_t fmt = (snd_pcm_format_t) format.format;
    RLOGD("stream %d, sample rate %d,channels %d,format %d\n",stream, rate,channels,format);
    char name[1024] = {0};
    sprintf(name,"hw:%d,%d",mDevinfo.card,mDevinfo.device);

    if ((err = snd_pcm_open(&mHandle, name, stream, 0)) < 0) {
        RLOGE("%s (%s): cannot open audio device (%s)\n",
              name, dirname, snd_strerror(err));
        return err;
    }

    if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
        RLOGE("%s (%s): cannot allocate hardware parameter structure(%s)\n",
              name, dirname, snd_strerror(err));
        return err;
    }

    if ((err = snd_pcm_hw_params_any(mHandle, hw_params)) < 0) {
        RLOGE("%s (%s): cannot initialize hardware parameter structure(%s)\n",
              name, dirname, snd_strerror(err));
        return err;
    }

    if ((err = snd_pcm_hw_params_set_access(mHandle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        RLOGE("%s (%s): cannot set access type(%s)\n",
              name, dirname, snd_strerror(err));
        return err;
    }

    if ((err = snd_pcm_hw_params_set_format(mHandle, hw_params, fmt)) < 0) {
        RLOGE("%s (%s): cannot set sample format(%s)\n",
              name, dirname, snd_strerror(err));
        return err;
    }

    if ((err = snd_pcm_hw_params_set_rate_near(mHandle, hw_params, &rate, NULL)) < 0) {
        RLOGE("%s (%s): cannot set sample rate(%s)\n",
              name, dirname, snd_strerror(err));
        return err;
    }

    if ((err = snd_pcm_hw_params_set_channels(mHandle, hw_params, channels)) < 0) {
        RLOGE("%s (%s): cannot set channel count(%s)\n",
              name, dirname, snd_strerror(err));
        return err;
    }

    if(stream == SND_PCM_STREAM_PLAYBACK) {
        snd_pcm_uframes_t buf_size = BUFSIZE * 4;
        snd_pcm_hw_params_get_buffer_size(hw_params,&buf_size);
        RLOGD("get buffer size is %d",buf_size);

        //buf_size /= 2;
        err = snd_pcm_hw_params_set_buffer_size_near(mHandle, hw_params, &(buf_size));
        //检查设置周期大小是否成功
        if (err < 0) {
            RLOGE("Can't set buffer size: %s\n", snd_strerror(err));
            return err;
        }

        snd_pcm_uframes_t period_size = 1024*4;
        int dir;
        snd_pcm_hw_params_get_period_size(hw_params, &period_size, &dir);
        RLOGD("get period size is %d", period_size);

        err = snd_pcm_hw_params_set_period_size_near(mHandle, hw_params, &period_size, &dir);
        //检查设置周期大小是否成功
        if (err < 0) {
            RLOGE("Can't set period size: %s\n", snd_strerror(err));
            return err;
        }
    }

    if ((err = snd_pcm_hw_params(mHandle, hw_params)) < 0) {
        RLOGE("%s (%s): cannot set parameters(%s)\n",
              name, dirname, snd_strerror(err));
        return err;
    }

    snd_pcm_hw_params_free(hw_params);

    if ((err = snd_pcm_sw_params_malloc(&sw_params)) < 0) {
        RLOGE("%s (%s): cannot allocate software parameters structure(%s)\n",
              name, dirname, snd_strerror(err));
        return err;
    }
    if ((err = snd_pcm_sw_params_current(mHandle, sw_params)) < 0) {
        RLOGE("%s (%s): cannot initialize software parameters structure(%s)\n",
              name, dirname, snd_strerror(err));
        return err;
    }
    if ((err = snd_pcm_sw_params_set_avail_min(mHandle, sw_params, BUFSIZE)) < 0) {
        RLOGE("%s (%s): cannot set minimum available count(%s)\n",
              name, dirname, snd_strerror(err));
        return err;
    }
    if ((err = snd_pcm_sw_params_set_start_threshold(mHandle, sw_params, 0U)) < 0) {
        RLOGE("%s (%s): cannot set start mode(%s)\n",
              name, dirname, snd_strerror(err));
        return err;
    }
    if ((err = snd_pcm_sw_params(mHandle, sw_params)) < 0) {
        RLOGE("%s (%s): cannot set software parameters(%s)\n",
              name, dirname, snd_strerror(err));
        return err;
    }

    if(stream == SND_PCM_STREAM_PLAYBACK) {
        if ((err = snd_pcm_prepare(mHandle)) < 0) {
            RLOGE("cannot prepare audio interface for use(%s)\n",
                  snd_strerror(err));
            return err;
        }
    } else {
        if ((err = snd_pcm_start(mHandle)) < 0) {
            RLOGE("cannot prepare audio interface for use(%s)\n",
                  snd_strerror(err));
            return err;
        }
    }

    RLOGD("open %s success\n",name);
}
