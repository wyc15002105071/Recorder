#include "v4l2_device.h"
#include "log.h"
#include <vector>
#include "drm_fourcc.h"

using namespace std;

#define MODULE_TAG "V4L2Device"

static string fcc2s(__u32 val)
{
    std::string s;

    s += val & 0x7f;
    s += (val >> 8) & 0x7f;
    s += (val >> 16) & 0x7f;
    s += (val >> 24) & 0x7f;
    if (val & (1U << 31))
        s += "-BE";
    return s;
}

V4L2Device::V4L2Device()
    :mDeviceFd(0)
    ,mVideoWidget(nullptr)
    ,mHwEncoder(nullptr)
{

}

V4L2Device::~V4L2Device()
{
    stopTask();

    if(mHwEncoder) {
        mHwEncoder->stopTask();
        delete mHwEncoder;
        mHwEncoder = nullptr;
    }
}

void V4L2Device::run()
{
    while(!mThreadExit) {
        for (int i = 0; i < MAX_BUF_CNT; i++ ) {
            int ts;
            fd_set fds;
            struct timeval tv;
            FD_ZERO(&fds);
            FD_SET(mDeviceFd, &fds);
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            ts = select(mDeviceFd + 1, &fds, NULL, NULL, &tv);

            if(ts == 0) {
                RLOGE("select time out");
                continue;
            }
            mLock.lock();
            if (ioctl(mDeviceFd, VIDIOC_DQBUF, &mBufferArray[i]) == -1) {
                RLOGE("DQBUF failed");
                mLock.unlock();
                continue;
            }

            if (mBufferArray[i].index > MAX_BUF_CNT) {
                RLOGE("buffer index out of bounds\n");
                mLock.unlock();
                continue;
            }
            if(mVideoWidget)
                mVideoWidget->PrepareUpdate(mBufferArray[i].index);
            if(mIsEncoding) {
                if(mHwEncoder) {
                    mHwEncoder->prepareSendframe(mBufferArray[i].index,
                                                 mBufferArray[i].m.planes[0].m.fd,
                                                 mBo[i].buf_size,
                                                 mBo[i].vir_addr);
                }
            }

            if (V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE == mBufferArray[i].type)
                mBufferArray[i].bytesused = mBufferArray[i].m.planes[0].bytesused;


            if(mBufferArray[i].index >= 0) {
                if (ioctl(mDeviceFd, VIDIOC_QBUF, &mBufferArray[i]) == -1) {
                    RLOGE("VIDIOC_QBUF failed");
                    mLock.unlock();
                    break;
                }
            }
            mLock.unlock();
        }
    }
}

void V4L2Device::startEnc()
{
    if(!mHwEncoder->startTask())
        return;
    mIsEncoding = true;
}

void V4L2Device::stopEnc()
{
    mHwEncoder->stopTask();
    mIsEncoding = false;
}

bool V4L2Device::init_device(bool is_hdmi_in)
{
    int count = getVideoNodesCount("/dev");
    RLOGD("video node count is %d",count);
    vector<VideoNodeInfo> info(count);
    getVideoNodesInfo(info);

    char node[100] = {0};

    for(int i=0;i<count;i++) {
        RLOGD("node path is %s,driver is %s,card is %s",
              info[i].node_path.c_str(),info[i].driver.c_str(),info[i].card.c_str());
        if(is_hdmi_in) {
            if(info[i].driver.find("hdmirx") != string::npos) {
                strcpy(node,info[i].node_path.c_str());
                break;
            }
            else if(info[i].driver.find("mainpath") != string::npos){
                strcpy(node,info[i].node_path.c_str());
                break;
            }
        }
    }

    RLOGD("node is %s,is hdmi in:%d!",node,is_hdmi_in);
    if(!strcmp(node,""))
        return false;

    mDeviceFd = open(node, O_RDWR);
    bool is_multiplanar = false;
    if (mDeviceFd == -1) {
        RLOGE("unable to open %s",node);
        return false;
    }
    RLOGD("open %s success!",node);
    struct v4l2_capability caps;
    if (ioctl(mDeviceFd, VIDIOC_QUERYCAP, &caps) == -1) {
        RLOGE("Failed to query device capabilities");
        close(mDeviceFd);
        return false;
    }

    is_multiplanar = caps.capabilities & (V4L2_CAP_VIDEO_CAPTURE_MPLANE |
                                          V4L2_CAP_VIDEO_M2M_MPLANE |
                                          V4L2_CAP_VIDEO_OUTPUT_MPLANE);
    int vidcap_buftype = is_multiplanar ? V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE :
                                          V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int vidout_buftype = is_multiplanar ? V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE :
                                          V4L2_BUF_TYPE_VIDEO_OUTPUT;

    int buftype = 0;
    if(!is_multiplanar) {
        if(caps.capabilities & V4L2_CAP_VIDEO_CAPTURE) {
            buftype = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        }else if(caps.capabilities & V4L2_CAP_VIDEO_OUTPUT) {
            buftype = V4L2_BUF_TYPE_VIDEO_OUTPUT;
        }
    }else {
        if(caps.capabilities & V4L2_CAP_VIDEO_CAPTURE_MPLANE) {
            buftype = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        }else if(caps.capabilities & V4L2_CAP_VIDEO_OUTPUT_MPLANE) {
            buftype = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
        }
    }
    if(buftype == 0) {
        RLOGE("buf type is unkown");
        close(mDeviceFd);
        return false;
    }

    struct v4l2_format format;
    memset(&format, 0, sizeof(struct v4l2_format));
    format.type = buftype;
    if (ioctl(mDeviceFd, VIDIOC_G_FMT, &format) == -1) {
        RLOGE("unable to get fmt");
        close(mDeviceFd);
        return false;
    }

    RLOGD("current video fmt is:width:%d,height:%d,PixFormat is %s",format.fmt.pix.width,format.fmt.pix.height,fcc2s(format.fmt.pix.pixelformat).c_str());

    // 请求缓冲区
    struct v4l2_requestbuffers req;
    memset(&req, 0, sizeof(struct v4l2_requestbuffers));
    req.count = MAX_BUF_CNT; // 请根据需要调整缓冲区数量
    req.type = buftype;
    req.memory = V4L2_MEMORY_DMABUF;

    if (ioctl(mDeviceFd, VIDIOC_REQBUFS, &req) == -1) {
        RLOGE("VIDIOC_REQBUFS failed");
        close(mDeviceFd);
        return false;
    }

    RLOGD("VIDIOC_REQBUFS success!");

    if(mVideoWidget) {
        int fmt = DRM_FORMAT_RGB888;
        const char *pixel_fmt = fcc2s(format.fmt.pix.pixelformat).c_str();
        if(!strcmp(pixel_fmt,"BGR3") ||  !strcmp(pixel_fmt,"RGB3")) {
            fmt = DRM_FORMAT_RGB888;
        }
        else if(!strcmp(pixel_fmt,"NV12")) {
            fmt = DRM_FORMAT_NV12;
        }
        else if(!strcmp(pixel_fmt,"NV21")) {
            fmt = DRM_FORMAT_NV21;
        }
        else if(!strcmp(pixel_fmt,"NV16")) {
            fmt = DRM_FORMAT_NV16;
        }
        mVideoWidget->allocBuffers(format.fmt.pix.width,format.fmt.pix.height
                                   ,fmt,is_hdmi_in?1:0,1,MAX_BUF_CNT);
    }

    {
        int fmt = MPP_FMT_RGB888;
        const char *pixel_fmt = fcc2s(format.fmt.pix.pixelformat).c_str();
        if(!strcmp(pixel_fmt,"BGR3")) {
            fmt = MPP_FMT_BGR888;
        }
        else if(!strcmp(pixel_fmt,"RGB3")) {
            fmt = MPP_FMT_BGR888;
        }
        else if(!strcmp(pixel_fmt,"NV12")) {
            fmt = MPP_FMT_YUV420SP;
        }
        else if(!strcmp(pixel_fmt,"NV21")) {
            fmt = MPP_FMT_YUV420SP;
        }
        else if(!strcmp(pixel_fmt,"NV16")) {
            fmt = MPP_FMT_YUV422SP;
        }
        if(!mHwEncoder) {
            mHwEncoder = new HwEncoder;
        }
        mHwEncoder->createHwEncoder(format.fmt.pix.width,format.fmt.pix.height,fmt);
    }

    {
        for (int i = 0; i < MAX_BUF_CNT; i++) {

            int buf_fd = 0,buf_size = 0;
            void *addr = nullptr;
            if(mVideoWidget) {
                mVideoWidget->getBufferInfo(buf_fd,buf_size,&addr,i);
                mBo[i].vir_addr = addr;
                mBo[i].buf_fd = buf_fd;
                mBo[i].buf_size = buf_size;
            }
//            if(hw_encoder)
//                hw_encoder->import_buf(buf_fd,buf_size,i);
            memset(&mPlanes[i], 0, sizeof(struct v4l2_plane));
            memset(&mBufferArray[i], 0, sizeof(struct v4l2_buffer));

            mBufferArray[i].index = i;
            mBufferArray[i].type = buftype;
            mBufferArray[i].memory = V4L2_MEMORY_DMABUF;//V4L2_MEMORY_DMABUF V4L2_MEMORY_MMAP;
            if (caps.capabilities & V4L2_CAP_VIDEO_CAPTURE_MPLANE) {
                mBufferArray[i].m.planes = &mPlanes[i];
                mBufferArray[i].length = FMT_NUM_PLANES;
            }

            int ret = ioctl(mDeviceFd, VIDIOC_QUERYBUF, &mBufferArray[i]);
            if (ret < 0) {
                RLOGE("VIDIOC_QUERYBUF Failed, error: %s\n", strerror(errno));
                close(mDeviceFd);
                return false;
            }
            RLOGD("VIDIOC_QUERYBUF success!");

            if (caps.capabilities & V4L2_CAP_VIDEO_CAPTURE_MPLANE) {
                for (int j=0; j<FMT_NUM_PLANES; j++) {
                    //mHinNodeInfo->bufferArray[i].m.planes[j].m.fd = mSidebandWindow->getBufferHandleFd(mHinNodeInfo->buffer_handle_poll[i]);
                    mBufferArray[i].m.planes[j].m.fd = buf_fd;
                    mBufferArray[i].m.planes[j].length = 0;
                    mBufferArray[i].m.planes[j].data_offset = 0;
                }
            }
            ret = ioctl(mDeviceFd, VIDIOC_QBUF, &mBufferArray[i]);
            if (ret < 0) {
                RLOGE("VIDIOC_QBUF Failed, error: %s\n", strerror(errno));
                continue;
            }

            RLOGD("VIDIOC_QBUF %d success!",i);
        }

    }
    enum v4l2_buf_type type = (v4l2_buf_type)buftype;
    if (ioctl(mDeviceFd, VIDIOC_STREAMON, &type) == -1) {
        perror("无法启动视频流");
        close(mDeviceFd);
        return false;
    }

    // 开始循环采集数据
    RLOGD("start streaming...\n");
    return true;
}

void V4L2Device::stopTask()
{
    mThreadExit = true;
    if(isRunning()) {
        quit();
        wait(QUIT_TIMEOUT);
    }

    if(mHwEncoder) {
        mHwEncoder->stopTask();
    }

}
