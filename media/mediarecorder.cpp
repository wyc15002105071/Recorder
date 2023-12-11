#include "mediarecorder.h"
#include "common/log.h"

#define MODULE_TAG "MediaRecorder"

MediaRecorder::MediaRecorder()
    :mVideoEncoder(sp<RKHWEncApi>(new RKHWEncApi)),
     mMediaMuxer(sp<MediaMuxer>(new MediaMuxer)),
     mRecordState(REC_STATE_UNINITIALIZED),
     mVideoIndex(0),
     mRecordEosFlag(false)
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
    mRecordState = REC_STATE_RUNNING;
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

        VideoFrameBuffer video_buffer = mBufferlist.front();

        mVideoEncoder->sendFrame(video_buffer.dmabuf,frameIndex,mRecordEosFlag);
        mBufferlist.pop_front();
        mLock.unlock();

        RKHWEncApi::EncoderOut encOut;
        int ret = mVideoEncoder->getOutPacket(&encOut);
        if(ret == 0) {
            if(encOut.keyFrame) {
                RLOGD("IDR frame produced");
            }
            frameIndex++;
            mMediaMuxer->sendPacket(encOut.packet);
            if(encOut.eos) {
                RLOGD("recieve output eos");
                mRecordState = REC_STATE_STOPING;
                break;
            }
        }

        usleep(1*1000);
    }

    if(mMediaMuxer)
        mMediaMuxer->stopTask();

    if(mBufferlist.size() > 0)
        mBufferlist.clear();

    RLOGD("Record Thread finish");
    mRecordState = REC_STATE_STOPPED;
    mThreadExit = true;
    mRecordEosFlag = false;
//    if(test_file)
//        fclose(test_file);
}

bool MediaRecorder::startTask()
{
    if(mRecordState == REC_STATE_STOPING || mRecordState == REC_STATE_UNINITIALIZED || mRecordState == REC_STATE_INITIALIZING )
        return false;

    mRecordEosFlag = false;
    mThreadExit = false;
    this->start();

    if(mMediaMuxer) {
        MppPacket hdr_pkt = nullptr;
        mVideoEncoder->getHdrPacket(&hdr_pkt);
        RKHWEncApi::EncCfgInfo cfg = mVideoCfg;
        mMediaMuxer->createMuxCtx(cfg.width,cfg.height,VIDEO_ENCODE_TYPE_AVC,cfg.framerate,hdr_pkt);
        mMediaMuxer->setMux(true,MUX_FILE_TYPE_MP4);
        mMediaMuxer->startTask();
    }
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

bool MediaRecorder::initVideoRecorder(int width, int height, __u32 format, int type)
{
    mRecordState = REC_STATE_INITIALIZING;
    if(!mVideoEncoder)
        mVideoEncoder = sp<RKHWEncApi>(new RKHWEncApi);
    RKHWEncApi::EncCfgInfo cfg;
    memset(&cfg,0,sizeof (RKHWEncApi::EncCfgInfo));
    cfg.width       = width;
    cfg.height      = height;
    cfg.format      = format;
    cfg.type        = type;
    cfg.bitrateMode = 1;
    cfg.framerate   = 60;

    if(mVideoEncoder->prepare(&cfg)) {
        mRecordState = REC_STATE_UNINITIALIZED;
        RLOGE("failed to prepare video encoder");
        return false;
    }

    memcpy(&mVideoCfg,&cfg,sizeof(RKHWEncApi::EncCfgInfo));
    mRecordState = REC_STATE_INITIALIZED;
    return true;
}

bool MediaRecorder::sendVideoFrame(int dma_fd,int size,int width,int height,bool eos)
{
    mLock.lock();
    if(!mVideoEncoder || mRecordState == REC_STATE_UNINITIALIZED || mRecordState == REC_STATE_STOPPED) {
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
