#ifndef MEDIARECORDER_H
#define MEDIARECORDER_H

#include "common/common.h"
#include "common/log.h"
#include "common/rthread.h"
#include "common/Mutex.h"
#include "rkmpp_enc_api.h"
#include "mediamuxer.h"

class MediaRecorder : public RThread
{
public:
    MediaRecorder();
    ~MediaRecorder();

    virtual void run();
    virtual bool startTask();
    virtual void stopTask();

    bool initVideoRecorder(int width,int height,__u32 format,int type);
    bool sendVideoFrame(int dma_fd,int size,int width,int height,bool eos);
    void sendVideoFrame(int index);

private:
    void releaseRecorder();
private:
    typedef enum RecordState{
        REC_STATE_UNINITIALIZED,
        REC_STATE_STOPPED,
        REC_STATE_STOPING,
        REC_STATE_INITIALIZING,
        REC_STATE_INITIALIZED,
        REC_STATE_RUNNING,
    } RecordState_t;

    typedef struct VideoFrameBuffer {
        RKHWEncApi::DmaBuffer_t dmabuf;
        int     pts;
        bool    eos;
    } VideoFrameBuffer_t;

    sp<RKHWEncApi> mVideoEncoder;
    sp<MediaMuxer> mMediaMuxer;
    RKHWEncApi::EncCfgInfo mVideoCfg;
    RecordState    mRecordState;

    FILE *test_file = nullptr;

    int mVideoIndex;
    int mCurrentIndex = -1;
    bool mRecordEosFlag;
    std::list<VideoFrameBuffer> mBufferlist;
};

#endif // MEDIARECORDER_H
