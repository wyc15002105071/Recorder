#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H

#include <string>
#include <stdio.h>
#include <iostream>

#define Audio_Interface_Snd 0
#define Audio_Interface_Qt5 1

class AudioDevice
{
public:
    AudioDevice();

    typedef struct AudioFormat {
        uint32_t sample_rate;
        uint32_t format;
        uint32_t channels;
    } AudioFormat_t;

    typedef struct AudioDeviceInfo {
        int card;
        int device;
        std::string id;
        std::string name;
    } AudioDeviceInfo_t;

    typedef enum AudioDeviceType {
        AudioDevice_TypeNone,
        AudioDevice_TypeMic,
        AudioDevice_TypeHdmiIn,
        AudioDevice_TypeSpeeker,
    } AudioDeviceType_t;

    static AudioDevice *create(int interface);

    virtual int open(AudioDeviceType type, AudioFormat_t &format) = 0;

    virtual void close() = 0;

    virtual int read(void *buf) = 0;

    virtual int write(void *buf, int size) = 0;

    virtual int wait(int timeout) = 0;

protected:
    bool get_hdmiin_audio_info(char *prop, int *value);
protected:
    AudioFormat_t mFormat;
    AudioDeviceInfo_t mDevinfo;
};

#endif // AUDIODEVICE_H
