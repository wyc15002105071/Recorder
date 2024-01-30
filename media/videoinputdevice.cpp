#include "videoinputdevice.h"
#include "log.h"
#include <vector>
#include "drm_fourcc.h"
#include "utils/osdutils.h"
#include "media/rkrgadef.h"
#include "utils/configutils.h"

using namespace std;

#define MODULE_TAG "VideoInputDevice"

VideoInputDevice::VideoInputDevice()
    : mDeviceFd(0)
    , mVideoWidget(nullptr)
    , mIsEncoding(false)
    , mVideoEosFlag(false)
    , mRecorder(sp<MediaRecorder>(new MediaRecorder))
    , mPusher(sp<StreamPusher>(new StreamPusher))
{
    connect(this,SIGNAL(onNeedReset()),this,SLOT(onReset()),Qt::UniqueConnection);
}

VideoInputDevice::~VideoInputDevice()
{
    stopTask();
    deinit();

    if(mRecorder) {
        mRecorder->stopTask();
    }
    disconnect(this,SIGNAL(onNeedReset()),this,SLOT(onReset()));
}

void VideoInputDevice::run()
{
    int retry = 0;
    bool needReset = false;
    while(!mThreadExit) {
        for (int i = 0; i < MAX_BUF_CNT; i++ ) {
            mLock.lock();
            int ts;
            fd_set fds;
            struct timeval tv;
            FD_ZERO(&fds);
            FD_SET(mDeviceFd, &fds);
            tv.tv_sec   = 1;
            tv.tv_usec  = 0;

            ts = select(mDeviceFd + 1, &fds, NULL, NULL, &tv);

            if(ts == 0) {
                RLOGE("select time out");
                mLock.unlock();
                if(mThreadExit){
                    break;
                }
                retry++;
                if(retry > 3) {
                    RLOGD("need reset...");
                    needReset = true;
                    retry = 0;
                    mThreadExit = true;
                    if(signalIn){
                        emit signalChange(false);
                        signalIn = false;
                    }
                    usleep(1*1000);
                    break;
                }else {
                    usleep(1*1000);
                    continue;
                }
            }

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
            if(!signalIn){
                emit signalChange(true);
                signalIn = true;
            }
            if(mVideoWidget)
                mVideoWidget->PrepareUpdate(mBufferArray[i].index);
            if(mIsPushing) {
                if(mPusher) {
                    mPusher->sendData(mDmaBo[i].buf_fd,mDmaBo[i].buf_size,
                                      mDmaBo[i].width,mDmaBo[i].height,mDmaBo->vir_addr);
                }
            }

            if(mIsEncoding) {
                RKRgaDef::RgaInfo srcInfo;
                RKRgaDef::RgaInfo dstInfo;
                int rga_format = RK_FORMAT_RGB_888;
                string fmt = fcc2s(mStreamInfo.format);
                if(fmt == "RGB3" || fmt == "BGR3") {
                    rga_format = RK_FORMAT_RGB_888;
                } else if(fmt == "NV12") {
                    rga_format = RK_FORMAT_YCbCr_420_SP;
                }

                RKRgaDef::SetRgaInfo(&srcInfo, mOsdBo.buf_fd, mOsdBo.width, mOsdBo.height,RK_FORMAT_RGBA_8888 ,ALIGN(mOsdBo.width,4), ALIGN(mOsdBo.height,2));
                RKRgaDef::SetRgaInfo(&dstInfo, mDmaBo[i].buf_fd, mDmaBo[i].width, mDmaBo[i].height,rga_format ,ALIGN(mDmaBo[i].width,4), ALIGN(mDmaBo[i].height,2));
                RKRgaDef::ProcessOSD(srcInfo,dstInfo);
                if(mRecorder) {
                    mRecorder->sendVideoFrame(mDmaBo[i].buf_fd,mDmaBo[i].buf_size,
                                              mDmaBo[i].width,mDmaBo[i].height,false);
                }
            } else if(mVideoEosFlag) {
                mRecorder->sendVideoFrame(mDmaBo[i].buf_fd,mDmaBo[i].buf_size,
                                          mDmaBo[i].width,mDmaBo[i].height,true);
                RLOGD("Send eos");
                mVideoEosFlag = false;
            }

            if(mIsPushing) {
                if(mPusher) {
                    mPusher->sendData(mDmaBo[i].buf_fd,mDmaBo[i].buf_size,
                                      mDmaBo[i].width,mDmaBo[i].height,mDmaBo->vir_addr);
                }
            }

            if(mVideoWidget)
                mVideoWidget->PrepareUpdate(mBufferArray[i].index);

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
            usleep(1*1000);
        }
    }

    if(needReset) {
        needReset = false;
        onNeedReset();
    }

    RLOGD("video input thread exit...");
}

bool VideoInputDevice::startTask()
{
    mThreadExit = false;
    start();
    return true;
}

void VideoInputDevice::startRecord(bool push)
{
    //    mLock.lock();
    if(mRecorder) {
        mRecorder->initVideoRecorder(mStreamInfo.width,mStreamInfo.height,mStreamInfo.format,MPP_VIDEO_CodingAVC, push);
        mRecorder->startTask();
    }

    QPixmap osd;
    //OSDUtils::createOSD(osd,"清阅技术");
    OSDUtils::createOSD(osd,ConfigUtils::ost_txt.isEmpty()?"清阅技术":ConfigUtils::ost_txt);

    if(mOsdBo.vir_addr) {
        memcpy(mOsdBo.vir_addr,osd.toImage().bits(),osd.toImage().sizeInBytes());
    }
    OSDUtils::destroyOSD(osd);

    mVideoEosFlag = false;
    mIsEncoding = true;
    //    mLock.unlock();
}

void VideoInputDevice::stopRecord()
{
    //    mLock.lock();
    if(mRecorder) {
        mRecorder->stopTask();
    }
    mVideoEosFlag = true;
    mIsEncoding = false;
    //    mLock.unlock();
}

void VideoInputDevice::startPush()
{
    mIsPushing = true;
    mPusher->prepare(mStreamInfo.width,mStreamInfo.height,mStreamInfo.format,MPP_VIDEO_CodingAVC);
    mPusher->start();
}

void VideoInputDevice::stopPush()
{
    mIsPushing = false;
    mPusher->stop();
}

string VideoInputDevice::getPushUrl()
{
    if(mPusher) {
        return mPusher->getUrl();
    }

    return "";
}

void VideoInputDevice::onReset()
{
    reset();
}

bool VideoInputDevice::initDevice(bool is_hdmi_in)
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

    is_multiplanar      = caps.capabilities & (V4L2_CAP_VIDEO_CAPTURE_MPLANE |
                                               V4L2_CAP_VIDEO_M2M_MPLANE |
                                               V4L2_CAP_VIDEO_OUTPUT_MPLANE);
    int vidcap_buftype  = is_multiplanar ? V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE :
                                           V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int vidout_buftype  = is_multiplanar ? V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE :
                                           V4L2_BUF_TYPE_VIDEO_OUTPUT;

    int buf_type = 0;
    if(!is_multiplanar) {
        if(caps.capabilities & V4L2_CAP_VIDEO_CAPTURE) {
            buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        }else if(caps.capabilities & V4L2_CAP_VIDEO_OUTPUT) {
            buf_type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
        }
    }else {
        if(caps.capabilities & V4L2_CAP_VIDEO_CAPTURE_MPLANE) {
            buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        }else if(caps.capabilities & V4L2_CAP_VIDEO_OUTPUT_MPLANE) {
            buf_type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
        }
    }
    if(buf_type == 0) {
        RLOGE("buf type is unkown");
        close(mDeviceFd);
        return false;
    }

    struct v4l2_format format;
    memset(&format, 0, sizeof(struct v4l2_format));
    format.type = buf_type;
    if (ioctl(mDeviceFd, VIDIOC_G_FMT, &format) == -1) {
        RLOGE("unable to get fmt");
        close(mDeviceFd);
        return false;
    }

    RLOGD("current video fmt is:width:%d,height:%d,PixFormat is %s",format.fmt.pix.width,format.fmt.pix.height,fcc2s(format.fmt.pix.pixelformat).c_str());

    // 请求缓冲区
    struct v4l2_requestbuffers req;
    memset(&req, 0, sizeof(struct v4l2_requestbuffers));
    req.count   = MAX_BUF_CNT; // 请根据需要调整缓冲区数量
    req.type    = buf_type;
    req.memory  = V4L2_MEMORY_MMAP;

    if (ioctl(mDeviceFd, VIDIOC_REQBUFS, &req) == -1) {
        RLOGE("VIDIOC_REQBUFS failed");
        close(mDeviceFd);
        return false;
    }

    RLOGD("VIDIOC_REQBUFS success!");

    memset(&mStreamInfo,0,sizeof(StreamInfo));
    mStreamInfo.width = format.fmt.pix.width;
    mStreamInfo.height = format.fmt.pix.height;
    mStreamInfo.format = format.fmt.pix.pixelformat;

    {
        for (int i = 0; i < MAX_BUF_CNT; i++) {
            int buf_fd = 0,buf_size = 0;
            void *addr = nullptr;
            memset(&mDmaBo[i],0,sizeof(DmaBufferObject));
            memset(&mPlanes[i], 0, sizeof(struct v4l2_plane));
            memset(&mBufferArray[i], 0, sizeof(struct v4l2_buffer));

            mBufferArray[i].index   = i;
            mBufferArray[i].type    = buf_type;
            mBufferArray[i].memory  = V4L2_MEMORY_MMAP;//V4L2_MEMORY_DMABUF V4L2_MEMORY_MMAP;
            if (caps.capabilities & V4L2_CAP_VIDEO_CAPTURE_MPLANE) {
                mBufferArray[i].m.planes    = &mPlanes[i];
                mBufferArray[i].length      = FMT_NUM_PLANES;
            }

            int ret = ioctl(mDeviceFd, VIDIOC_QUERYBUF, &mBufferArray[i]);
            if (ret < 0) {
                RLOGE("VIDIOC_QUERYBUF Failed, error: %s\n", strerror(errno));
                close(mDeviceFd);
                return false;
            }
            RLOGD("VIDIOC_QUERYBUF success!");

            if (V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE == buf_type) {
                // tmp_buffers[n_buffers].length = buf.m.planes[0].length;
                mDmaBo[i].buf_size = mBufferArray[i].m.planes[0].length;
                mDmaBo[i].vir_addr =
                        mmap(NULL /* start anywhere */,
                             mBufferArray[i].m.planes[0].length,
                        PROT_READ | PROT_WRITE /* required */,
                        MAP_SHARED /* recommended */,
                        mDeviceFd, mBufferArray[i].m.planes[0].m.mem_offset);
            } else {
                mDmaBo[i].buf_size = mBufferArray[i].length;
                mDmaBo[i].vir_addr =
                        mmap(NULL /* start anywhere */,
                             mBufferArray[i].length,
                             PROT_READ | PROT_WRITE /* required */,
                             MAP_SHARED /* recommended */,
                             mDeviceFd, mBufferArray[i].m.offset);
            }
            if (MAP_FAILED == mDmaBo[i].vir_addr) {
                RLOGE("ERROR: Failed to map device frame buffers\n");
                continue;
            }

            struct v4l2_exportbuffer expbuf = (struct v4l2_exportbuffer) {0} ;
            expbuf.type = buf_type;
            expbuf.index = i;
            expbuf.flags = O_CLOEXEC;
            if (ioctl(mDeviceFd, VIDIOC_EXPBUF, &expbuf) < 0) {
                RLOGE("get dma buf failed\n");
                continue;
            }

            RLOGD("get dmabuf %d",expbuf.fd);
            mDmaBo[i].buf_fd    = expbuf.fd;
            mDmaBo[i].width     = format.fmt.pix.width;
            mDmaBo[i].height    = format.fmt.pix.height;

            if(mVideoWidget) {
                mVideoWidget->importDmaBuffer(i,mDmaBo[i].buf_fd,mDmaBo[i].width,mDmaBo[i].height
                                              ,mStreamInfo.format,mDmaBo[i].buf_size,mDmaBo[i].vir_addr);
            }

            ret = ioctl(mDeviceFd, VIDIOC_QBUF, &mBufferArray[i]);
            if (ret < 0) {
                RLOGE("VIDIOC_QBUF Failed, error: %s\n", strerror(errno));
                continue;
            }

            RLOGD("VIDIOC_QBUF %d success!",i);
        }
    }

    memset(&mOsdBo,0,sizeof(DmaBufferObject));
    mOsdBo.width = OSD_DEFAULT_WIDTH;
    mOsdBo.height = OSD_DEFAULT_HEIGHT;
    Allocator *allocator = AllocatorService::getDrmAllocator();
    allocator->allocBuffers(mOsdBo.width,mOsdBo.height,DRM_FORMAT_RGBA8888,false,true,1);
    allocator->getBuffer(mOsdBo.buf_fd,mOsdBo.buf_size,&mOsdBo.vir_addr,0);


    enum v4l2_buf_type type = (v4l2_buf_type)buf_type;
    if (ioctl(mDeviceFd, VIDIOC_STREAMON, &type) == -1) {
        perror("无法启动视频流");
        close(mDeviceFd);
        return false;
    }

    // 开始循环采集数据
    RLOGD("start streaming...\n");
    return true;
}

void VideoInputDevice::deinit()
{
    if(mDeviceFd < 0)
        return;
    int buf_type = mBufferArray[0].type;
    ioctl(mDeviceFd,VIDIOC_STREAMOFF,&buf_type);

    for(int i=0;i<MAX_BUF_CNT;i++) {
        ioctl(mDeviceFd, VIDIOC_QUERYBUF, &mBufferArray[i]);
        munmap(mDmaBo[i].vir_addr, mDmaBo[i].buf_size);
        close(mDmaBo[i].buf_fd);
    }

    Allocator *allocator = AllocatorService::getDrmAllocator();
    allocator->destroyBuffers();

    close(mDeviceFd);
}

void VideoInputDevice::stopTask()
{
    if(isRunning()) {
        RLOGD("stop...");
        mThreadExit = true;
        quit();
        wait();
        deinit();
    }
    if(mRecorder) {
        mRecorder->stopTask();
    }
}

void VideoInputDevice::reset()
{
    RLOGD("reset...");
    deinit();
    if(mVideoWidget) {
        mVideoWidget->reset();
    }
    initDevice(true);
    startTask();
}
