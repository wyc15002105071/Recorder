#include "audiodevice.h"
#include <stdlib.h>
#include <string.h>
#include "media/sndaudiodevice.h"
#include "common/log.h"

AudioDevice::AudioDevice()
{
    memset(&mFormat,0,sizeof (AudioFormat_t));
    memset(&mDevinfo,0,sizeof (AudioDeviceInfo_t));
}

AudioDevice *AudioDevice::create(int interface)
{
    AudioDevice *device = nullptr;
    switch (interface) {
    case Audio_Interface_Snd: {
        device = new SNDAudioDevice();
    } break;
    case Audio_Interface_Qt5: {
        //ToDO
    } break;
    default:
        break;
    }

    return device;
}

bool AudioDevice::get_hdmiin_audio_info(char *prop, int *value)
{
    char strfile[128];
    FILE* file = NULL;
    char info[20] = {0};

    if (!value)
        return false;
    sprintf(strfile, "/sys/class/hdmirx/%s/%s", "hdmirx", prop);
    if (access(strfile, 0)) {
        RLOGE("No exist %s", strfile);
        return false;
    }
    file = fopen(strfile, "r");
    if (!file) {
        RLOGE("Could reading %s property", strfile);
        return false;
    }
    fread(info, sizeof(char), sizeof(info)/sizeof(char) - 1, file);
    fclose(file);
    *value = atoi(info);
    return true;
}
