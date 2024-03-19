#include "mediarecorder.h"
#include "common/log.h"
#include "utils/osdutils.h"
#include "utils/storageutils.h"

#define MODULE_TAG "MediaRecorder"

MediaRecorder::MediaRecorder()
    : mVideoEncoder(sp<RKHWEncApi>(new RKHWEncApi))
    , mMediaMuxer(sp<MediaMuxer>(new MediaMuxer))
    , mRecordState(REC_STATE_UNINITIALIZED)
    , mVideoIndex(0)
    , mRecordEosFlag(false)
    , mPushStream(false)
{

}

MediaRecorder::~MediaRecorder()
{
    mRecordState = REC_STATE_UNINITIALIZED;
    stopTask();
    if(mVideoEncoder) {
        mVideoEncoder->release();
    }
}

void MediaRecorder::run()
{
    RLOGD("Record Thread start");
    int frameIndex = 0;
    int ret = 0;

    //int temp = 0;

    mRecordState = REC_STATE_RUNNING;
    MediaMuxer::MediaPacket packet;

    void *extra = nullptr;
    memset(&packet,0,sizeof(MediaMuxer::MediaPacket));
    if(mMediaMuxer.get()) {
        MppPacket hdr_pkt = nullptr;
        mVideoEncoder->getHdrPacket(&hdr_pkt);
        RKHWEncApi::EncCfgInfo cfg = mVideoCfg;
        void *extra_data = mpp_packet_get_data(hdr_pkt);
        int extra_size = mpp_packet_get_size(hdr_pkt);
        extra = malloc(extra_size);
        MediaMuxer::MediaInfo info;
        memset(&info,0,sizeof(MediaMuxer::MediaInfo));
        memcpy(extra,extra_data,extra_size);
        
        info.extra_data = extra;
        info.extra_size = extra_size;
        info.width = cfg.width;
        info.height = cfg.height;
        info.framerate = cfg.framerate;
        info.push_type = Push_RTSP;
        info.suffix_type = Suffix_MP4;
        info.coding_type = Video_CodingType_AVC;
        info.push_stream = mPushStream;

        switch(cfg.type) {
        case MPP_VIDEO_CodingAVC:
            info.coding_type = Video_CodingType_AVC;
            break;
        case MPP_VIDEO_CodingHEVC:
            info.coding_type = Video_CodingType_HEVC;
            break;
        case MPP_VIDEO_CodingVP8:
            info.coding_type = Video_CodingType_VP8;
            break;
        case MPP_VIDEO_CodingVP9:
            info.coding_type = Video_CodingType_VP9;
            break;
        }

        ret = mMediaMuxer->prepare(info);
        if(ret < 0) {
            mpp_packet_deinit(&hdr_pkt);
            RLOGE("mediamuxer prepare failed");
            goto EXIT;
        }
        mpp_packet_deinit(&hdr_pkt);
    }

    while(!mThreadExit) {
        mLock.lock();
        if(!mVideoEncoder) {
            mLock.unlock();
            break;
        }

        if(mBufferlist.size() <= 0) {
            mLock.unlock();
            usleep(1*!000);
            continue;
        }
        //if((temp++ % mVideoCfg.framerate) == 0) {
        //    long total,used,free = 0;
        //    StorageUtils::get_instance()->getStorageCapacity(VIDEOS_SAVE_DIR,total,used,free);
        //    RLOGD("used is %ld",used);
        //    if(free <= SZ_1G) {
        //        RLOGD("exit:available free memory is low");
        //        break;
        //    }
        //    if(temp >= mVideoCfg.framerate)
        //        temp = 0;
        //}

        VideoFrameBuffer video_buffer = mBufferlist.front();

        mVideoEncoder->sendFrame(video_buffer.dmabuf,frameIndex,mRecordEosFlag);
        mBufferlist.pop_front();
        mLock.unlock();

        RKHWEncApi::EncoderOut encOut;
        int ret = mVideoEncoder->getOutPacket(&encOut);
        if(ret == 0) {
            frameIndex++;
            void *data = mpp_packet_get_data(encOut.packet);
            packet.pts = encOut.pts;
            packet.dts = encOut.pts;
            packet.size = encOut.size;
            packet.data = data;
            if(encOut.keyFrame) {
                RLOGD("IDR frame produced");
                packet.flags |= MediaMuxer::FLAG_OUTPUT_INTRA;
            }

            packet.is_video = true;
            if(encOut.eos) {
                RLOGD("recieve output eos");
                mRecordState = REC_STATE_STOPING;
                packet.flags |= MediaMuxer::FLAG_END_OF_STREAM;
                break;
            }

            mMediaMuxer->writeData(&packet);

            encOut.release();
        }

        usleep(1*1000);
    }

    packet.flags |= MediaMuxer::FLAG_END_OF_STREAM;
    mMediaMuxer->writeData(&packet);
EXIT:
    if(mBufferlist.size() > 0)
        mBufferlist.clear();

    mVideoEncoder->release();
    mRecordState = REC_STATE_STOPPED;
    mThreadExit = true;
    mRecordEosFlag = false;

    OSDUtils::destroyOSD(mOsd);
    RLOGD("Record finish");
}

bool MediaRecorder::startTask()
{
    if(mRecordState == REC_STATE_STOPING || mRecordState == REC_STATE_UNINITIALIZED || mRecordState == REC_STATE_INITIALIZING )
        return false;

    mRecordEosFlag = false;
    mThreadExit = false;
    this->start();

    return true;
}

void MediaRecorder::stopTask()
{
    mLock.lock();
    mRecordEosFlag = true;
    mThreadExit = true;
    mRecordState = REC_STATE_STOPING;
    mLock.unlock();
    wait(QUIT_TIMEOUT);
}

bool MediaRecorder::initVideoRecorder(int width, int height, __u32 format, int type, bool push_stream,VideoProfile profile)
{
    mRecordState = REC_STATE_INITIALIZING;
    RKHWEncApi::EncCfgInfo cfg;
    memset(&cfg,0,sizeof (RKHWEncApi::EncCfgInfo));
    cfg.width       = width;
    cfg.height      = height;
    cfg.format      = format;
    cfg.type        = type;
    cfg.bitrateMode = 1;
    cfg.framerate   = 60;

    switch(profile) {
    case VideoProfile_Low:
        cfg.bitRate = (cfg.width * cfg.height / 32) * cfg.framerate;
        break;
    case VideoProfile_Standard:
        cfg.bitRate = (cfg.width * cfg.height / 16) * cfg.framerate;
        break;
    case VideoProfile_High:
        cfg.bitRate = (cfg.width * cfg.height / 8) * cfg.framerate;
        break;
    default:
        cfg.bitRate = (cfg.width * cfg.height / 16) * cfg.framerate;
        break;
    }

    OSDUtils::createOSD(mOsd,"清阅技术");

    RKHWEncApi::OSD_t osd_t;
    osd_t.data     = mOsd.toImage().bits();
    osd_t.width    = mOsd.size().width();
    osd_t.height   = mOsd.size().height();
    osd_t.size     = mOsd.toImage().sizeInBytes();
    cfg.osd_enable = 1;
    cfg.osd = osd_t;

    if(mVideoEncoder->prepare(&cfg)) {
        mRecordState = REC_STATE_UNINITIALIZED;
        RLOGE("failed to prepare video encoder");
        return false;
    }

    memcpy(&mVideoCfg,&cfg,sizeof(RKHWEncApi::EncCfgInfo));
    mRecordState = REC_STATE_INITIALIZED;
    mPushStream = push_stream;
    return true;
}

bool MediaRecorder::sendVideoFrame(int dma_fd,int size,int width,int height,bool eos)
{
    mLock.lock();
    if(!mVideoEncoder.get() || mRecordState == REC_STATE_UNINITIALIZED || mRecordState == REC_STATE_STOPPED) {
        mLock.unlock();
        return false;
    }

    VideoFrameBuffer video_buffer;

    RKHWEncApi::DmaBuffer_t dma_buffer;
    dma_buffer.dma_fd = dma_fd;
    dma_buffer.size   = size;
    dma_buffer.width  = width;
    dma_buffer.height = height;
    video_buffer.dmabuf = dma_buffer;
    video_buffer.pts = mVideoIndex;
    video_buffer.eos = eos;

    mBufferlist.push_back(video_buffer);
    mVideoIndex++;
    mLock.unlock();
    return true;
}

void MediaRecorder::sendVideoFrame(int index)
{
    mLock.lock();
    mCurrentIndex = index;
    mLock.unlock();
}

void MediaRecorder::releaseRecorder()
{
    mLock.lock();
    if(mVideoEncoder)
        mVideoEncoder->release();
    mLock.unlock();
}
