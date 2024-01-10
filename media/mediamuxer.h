#ifndef MEDIAMUXER_H
#define MEDIAMUXER_H

#include "rk_mpi.h"
#include "mpp_mem.h"
#include <memory>
#include <list>
#include "rthread.h"
#include "mediautils.h"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
}

class MediaMuxer:public RThread
{
public:
    typedef struct MediaInfo {
        void         *extra_data;
        int32_t       extra_size;
        int32_t       framerate;
        int32_t       width;
        int32_t       height;
        Suffix_t      suffix_type;
        bool          push_stream;
        Push_t        push_type;

        Video_CodingType  coding_type;
    } MediaInfo_t;

    typedef enum PacketData {
        FLAG_END_OF_STREAM  = (1 << 0),
        FLAG_OUTPUT_INTRA = (1 << 1),
    } PacketData_t;

    typedef struct MediaPacket {
        void         *data;
        int32_t       size;
        int64_t       pts;
        int64_t       dts;
        int32_t       flags;
        bool          is_video;
    } MediaPacket_t;

    MediaMuxer();
    ~MediaMuxer();
//    virtual void run();
    int prepare(MediaInfo info);
    int writeData(MediaMuxer::MediaPacket *media_pkt);
//    void createMuxCtx(int width,int height,Video_CodingType video_type,int framerate,MppPacket hdr_pkt);
//    void sendPacket(MppPacket packet);
//    void setMux(bool need_mux,MUX_FILE_TYPE file_type);
//    void setPush(bool need_push,MUX_PUSH_TYPE push_type);
//    virtual bool startTask();
//    virtual void stopTask();
protected:
    typedef struct MediaMuxCtx {
        AVFormatContext *format_ctx;
        AVOutputFormat  *output_fmt;
        MediaInfo        info;
        int32_t          video_stream;
        int32_t          audio_stream;
    } MediaMuxCtx_t;

    MediaMuxCtx mMediaCtx;
#define MAX_QUEUE_SIZE 20
    std::list<MppPacket>mPackets;

    const int VIDEOSTREAM = 0;
    const int AUDIOSTREAM = 1;
};

#endif // MUXER_H
