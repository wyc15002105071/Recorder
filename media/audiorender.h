#ifndef AUDIORENDER_H
#define AUDIORENDER_H

#include "common/common.h"
#include "common/log.h"
#include "common/rthread.h"
#include "media/audiodevice.h"

class AudioRender : public RThread
{
    Q_OBJECT
public:
    AudioRender();
    virtual void run();

    int open();
    void close();
    void setOutVolume(float volume); // 0.0-1.0

    virtual void reset();

signals:
    void onNeedReset();

private slots:
    void onReset();

private:
    sp<AudioDevice> mAudioInputDevice;
    sp<AudioDevice> mAudioOutputDevice;

    float mOutVolume;
    AudioDevice::AudioFormat_t mAudioFormat;
private:
    void outPcmVolumeProcess(short* buf, float volume, int channels, size_t len);
};

#endif // AUDIORENDER_H
