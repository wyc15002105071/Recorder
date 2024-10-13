#ifndef MEDIARECORDER_H
#define MEDIARECORDER_H

#include "common/common.h"
#include "common/log.h"
#include "common/rthread.h"
#include "common/Mutex.h"
#include "rkmpp_enc_api.h"
#include "mediamuxer.h"
#include <QPixmap>

#define RECORD_OTHER 1

class MediaRecorder : public RThread
{
public:
    MediaRecorder();
    ~MediaRecorder();
    typedef enum VideoProfile {
        VideoProfile_Low,
        VideoProfile_Standard,
        VideoProfile_High
    } VideoProfile_t;
    virtual void run();
    virtual bool startTask();
    virtual void stopTask();

    bool initVideoRecorder(int width,int height,__u32 format,int framerate,int type,bool push_stream,VideoProfile profile = VideoProfile_Standard);
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
    RKHWEncApi::EncCfgInfo mVideoCfg;
    sp<MediaMuxer> mMediaMuxer;
#if RECORD_OTHER
    sp<MediaMuxer> mMediaMuxerOther;
#endif
    RecordState    mRecordState;

    FILE *test_file = nullptr;

    int mVideoIndex;
    int mCurrentIndex = -1;
    bool mRecordEosFlag;
    bool mPushStream;
    std::list<VideoFrameBuffer> mBufferlist;

    QPixmap mOsd;
};

#endif // MEDIARECORDER_H
