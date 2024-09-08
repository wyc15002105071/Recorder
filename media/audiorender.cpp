#include "audiorender.h"
#define BUFSIZE (1024*4)

#define MODULE_TAG "AudioRender"

#define VOLUME_MAX 10.0f
#define VOLUME_MIN 0.0f

AudioRender::AudioRender()
    : mAudioInputDevice(sp<AudioDevice>(AudioDevice::create(Audio_Interface_Snd)))
    , mAudioOutputDevice(sp<AudioDevice>(AudioDevice::create(Audio_Interface_Snd)))
    , mOutVolume(0.2f)
{
    connect(this,SIGNAL(onNeedReset()),this,SLOT(onReset()),Qt::UniqueConnection);
}

void AudioRender::setOutVolume(float volume)
{
    mLock.lock();

    mOutVolume = volume;
    if(mOutVolume > 1.0f) mOutVolume = 1.0f;
    if(mOutVolume < 0.0f) mOutVolume = 0.0f;
    
    mLock.unlock();
}

void AudioRender::reset()
{
    RLOGD("Audio Render start reset...");
    int err = 0;
    if(isRunning()) {
        stopTask();
    }

    startTask();
}

void AudioRender::onReset()
{
    reset();
}

void AudioRender::run()
{
    int buf[BUFSIZE * 2];
    int retry = 0;
    bool need_reset = false;
    int err = 0;
    err = open();
    if(err != 0) {
        need_reset = true;
        goto RET;
    }

    RLOGD("audio render thread start...");
    while (!mThreadExit) {
        if(mAudioOutputDevice->wait(500) < 0) {
            RLOGE("out device time out %s",strerror(errno));
            usleep(1*1000);
            if(retry >= 5) {
                retry = 0;
                need_reset = true;
                break;
            }

            retry++;
            continue;
        }

        int64_t size = mAudioInputDevice->read(buf);
        if(size <= 0) {
            usleep(1*1000);
            continue;
        }

        mLock.lock();
        float volume = mOutVolume;
        mLock.unlock();

        int channels = mAudioFormat.channels;
        outPcmVolumeProcess((short *)buf, volume, channels, size);
        mAudioOutputDevice->write(buf,size);
        usleep(1*1000);
    }

RET:
    close();

    mThreadExit = true;
    if(need_reset) {
        msleep(1*1000);
        onNeedReset();
    }
    RLOGD("audio render thread exit...");
}

int AudioRender::open()
{
    mAudioFormat.sample_rate = 48000;
    mAudioFormat.channels = 2;
    mAudioFormat.format   = 2;

    AudioDevice::AudioFormat_t format = mAudioFormat;
    int ret = 0;
    if(!mAudioInputDevice || !mAudioOutputDevice) {
        RLOGE("device is null,open failed");
        return -1;
    }
    ret = mAudioInputDevice->open(AudioDevice::AudioDevice_TypeHdmiIn,format);
    if(ret != 0) {
        RLOGE("audio input device open failed!");
        ret = -1;
        goto RET;
    }

    ret = mAudioOutputDevice->open(AudioDevice::AudioDevice_TypeSpeeker,format);
    if(ret != 0) {
        RLOGE("audio output device open failed!");
        ret = -1;
        goto RET;
    }

    RLOGD("open audio render success!");
RET:
    return ret;
}

void AudioRender::close()
{
    if(mAudioInputDevice)
        mAudioInputDevice->close();
    if(mAudioOutputDevice)
        mAudioOutputDevice->close();
}

void AudioRender::outPcmVolumeProcess(short* buffer, float volume, int channels, size_t len)
{
    if ((!buffer) || (len <= 0) || (channels <= 0))  {
        return ;
    }
    int16_t *pcm = (int16_t *)buffer;
    float left = volume * (VOLUME_MAX - VOLUME_MIN) + VOLUME_MIN;
    int frames = len / sizeof(int8_t);
    for (int frame = 0; frame < frames; frame ++) {
        int32_t temp = 0;
        for (int ch = 0;  ch < channels; ch++) {
            temp = (int32_t)(pcm[ch] * left);
            if (temp > 0x7fff) {
                temp = 0x7fff;
            } else if (temp < -0x8000) {
                temp = -0x8000;
            }
            pcm[ch] = temp;
        }
        pcm += channels;
    }
}
