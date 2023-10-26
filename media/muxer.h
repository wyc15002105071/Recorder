#ifndef MUXER_H
#define MUXER_H

#include "rk_mpi.h"
#include "mpp_mem.h"
#include <memory>
#include <list>
#include "rthread.h"
#include "media_utils.h"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
}

class Muxer:public RThread
{
public:
    Muxer();
    virtual void run();
    void createMuxCtx(int width,int height,VIDEO_ENCODE_TYPE video_type,int framerate,MppPacket hdr_pkt);
    void sendPacket(MppPacket packet);
    void setMux(bool need_mux,MUX_FILE_TYPE file_type);
    void setPush(bool need_push,MUX_PUSH_TYPE push_type);
    virtual bool startTask();
    virtual void stopTask();
protected:
    typedef struct {
        VIDEO_ENCODE_TYPE   video_type;
        MUX_FILE_TYPE       file_type;
        MUX_PUSH_TYPE       push_type;
        MppPacket hdr_pkt;
        bool      need_mux;
        bool      need_push;
        int       framerate;
        int       width;
        int       height;
    }MuxContext;

    std::shared_ptr<MuxContext>mMuxCtx;
#define MAX_QUEUE_SIZE 20
    std::list<MppPacket>mPackets;

    const int VIDEOSTREAM = 0;
    const int AUDIOSTREAM = 1;
};

#endif // MUXER_H
