#ifndef VIDEOINPUTDEVICE_H
#define VIDEOINPUTDEVICE_H

#include "mediautils.h"
#include "rthread.h"
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <string>
#include <stdlib.h>
#include <sys/select.h>
#include "widgets/videowidget.h"
#include "mediarecorder.h"
#include "media/streampusher.h"

class VideoInputDevice : public RThread
{
    Q_OBJECT
public:
#define MAX_BUF_CNT 4
#define FMT_NUM_PLANES 1
    VideoInputDevice();
    ~VideoInputDevice();
    virtual void run();

    virtual bool startTask();
    virtual void stopTask();
    virtual void reset();

    bool    initDevice(bool is_hdmi_in);
    void    deinit();
    void    startRecord(bool push = false);
    void    stopRecord();
    void    startPush();
    void    stopPush();
    void    setVideoWidget(VideoWidget *video_widget) { mVideoWidget = std::move(video_widget);};
    std::string getPushUrl();
private:
    typedef struct StreamInfo{
        int32_t     width;
        int32_t     height;
        uint32_t    format;
    } StreamInfo_t;

    int32_t       mDeviceFd;
    VideoWidget  *mVideoWidget;
    bool          mIsEncoding;
    bool          mIsPushing;
    bool          mVideoEosFlag;

    sp<MediaRecorder>   mRecorder;
    sp<StreamPusher>    mPusher;
    struct v4l2_plane   mPlanes[MAX_BUF_CNT];
    struct v4l2_buffer  mBufferArray[MAX_BUF_CNT];

    StreamInfo      mStreamInfo;
    DmaBufferObject mDmaBo[MAX_BUF_CNT];

signals:
    void onNeedReset();

public slots:
    void onReset();
};

#endif // VIDEOINPUTDEVICE_H
