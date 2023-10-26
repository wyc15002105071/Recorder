#ifndef V4L2_DEVICE_H
#define V4L2_DEVICE_H

#include "media_utils.h"
#include "rthread.h"
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <string>
#include <stdlib.h>
#include <sys/select.h>
#include "widgets/videowidget.h"
#include "hwencoder.h"

class V4L2Device : public RThread
{
public:
    V4L2Device();
    ~V4L2Device();
    virtual void run();
#define MAX_BUF_CNT 4
#define FMT_NUM_PLANES 1
    bool init_device(bool is_hdmi_in);
    virtual void stopTask();
    void startEnc();
    void stopEnc();

    void setVideoWidget(VideoWidget *video_widget) { mVideoWidget = std::move(video_widget);};
private:
    int mDeviceFd;
    VideoWidget *mVideoWidget;
    HwEncoder *mHwEncoder;
    struct v4l2_plane mPlanes[MAX_BUF_CNT];
    struct v4l2_buffer mBufferArray[MAX_BUF_CNT];

    bool mIsEncoding;

    typedef struct {
        int buf_fd;
        int buf_size;
        void *vir_addr;
    } BufferObject;

    BufferObject mBo[MAX_BUF_CNT];
};

#endif // V4L2DEVICE_H
