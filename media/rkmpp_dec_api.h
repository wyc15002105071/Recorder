#ifndef RKHWDECAPI_H
#define RKHWDECAPI_H

#include "rk_mpi.h"
#include "mpp_mem.h"
#include "mpp_log.h"
#include "mpp_env.h"

extern "C"{
#include "libavcodec/avcodec.h"
}

class RKHWDecApi
{
public:
    typedef struct OutputFrame_t {
        int32_t     fd;
        int32_t     width;
        int32_t     height;
        int32_t     hor_stride;
        int32_t     ver_stride;
        int32_t     size;
        void       *vir_addr;
        void       *handler;
        void       *buffer;
    } OutputFrame;

    typedef struct DecCfgInfo {
        int32_t         width;
        int32_t         height;
        int32_t         hor_stride;
        int32_t         ver_stride;
        MppCodingType   type;
        MppFrameFormat  out_fmt;
        bool        thumbnail;
        bool        low_latency;
    } DecCfgInfo_t;

    typedef enum FrameData {
        FLAG_END_OF_STREAM  = (1 << 0),
        FLAG_CODEC_CONFIG   = (1 << 1),
    } FrameData_t;

    RKHWDecApi();
    ~RKHWDecApi();
    int prepare(DecCfgInfo &info);
    void release();
    int sendPacket(uint8_t *data, int32_t size, int64_t pts, int64_t dts,int32_t flags);
    int getOutputFrame(OutputFrame *out_frame);
    void deinitOutputFrame(OutputFrame *out_frame);
    MppCodingType formatFF2Mpp(AVCodecID codecid);
    int convertNV12ToRGB(OutputFrame *src_frame,OutputFrame *dst_frame);

private:
    typedef struct DecoderCtx {
        MppCtx           mppCtx;
        MppApi          *mppApi;
        MppBufferGroup   group;
        DecCfgInfo       info;
        int32_t          initOk;
    }DecoderCtx_t;

    DecoderCtx mDecCtx;
};

#endif // RKHWDECAPI_H
