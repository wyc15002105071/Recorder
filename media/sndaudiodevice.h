#ifndef SNDAUDIODEVICE_H
#define SNDAUDIODEVICE_H

#include "media/audiodevice.h"
#include "alsa/asoundlib.h"
#include <QList>

class SNDAudioDevice : public AudioDevice
{
public:
    SNDAudioDevice();

    virtual int open(AudioDeviceType_t type, AudioFormat_t &format);

    virtual void close();

    virtual int read(void *buf);

    virtual int write(void *buf, int size);

    virtual int wait(int timeout);
private:
    void findAllDevices(snd_pcm_stream_t stream);
    int openSnd(snd_pcm_stream_t stream, AudioFormat_t &format);
private:
    QList<AudioDeviceInfo_t> mDevices;
    snd_pcm_t *mHandle;
    AudioDeviceType_t mType;
};

#endif // SNDAUDIODEVICE_H
