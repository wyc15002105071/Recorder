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

class VideoInputDevice : public RThread
{
public:
#define MAX_BUF_CNT 4
#define FMT_NUM_PLANES 1
    VideoInputDevice();
    ~VideoInputDevice();
    virtual void run();

    virtual void stopTask();
    virtual void reset();

    bool    initDevice(bool is_hdmi_in);
    void    deinit();
    void    startRecord();
    void    stopRecord();
    void    setVideoWidget(VideoWidget *video_widget) { mVideoWidget = std::move(video_widget);};
private:
    typedef struct DmaBufferObject{
        int32_t     buf_fd;
        int32_t     buf_size;
        void       *vir_addr;
        int32_t     width;
        int32_t     height;
    } DmaBufferObject_t;

    typedef struct StreamInfo{
        int32_t     width;
        int32_t     height;
        uint32_t    format;
    } StreamInfo_t;

    int32_t       mDeviceFd;
    VideoWidget  *mVideoWidget;
    bool          mIsEncoding;
    bool          mVideoEosFlag;

    sp<MediaRecorder>   mRecorder;
    struct v4l2_plane   mPlanes[MAX_BUF_CNT];
    struct v4l2_buffer  mBufferArray[MAX_BUF_CNT];

    StreamInfo      mStreamInfo;
    DmaBufferObject mDmaBo[MAX_BUF_CNT];
};

#endif // VIDEOINPUTDEVICE_H
