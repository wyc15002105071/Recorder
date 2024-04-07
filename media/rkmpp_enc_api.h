#ifndef RKHWENCAPI_H
#define RKHWENCAPI_H

#include <stdio.h>
#include "rk_mpi.h"
#include "mpp_mem.h"
#include "mpp_log.h"
#include "mpp_env.h"

class RKHWEncApi
{
public:
    RKHWEncApi();
    ~RKHWEncApi();
    typedef struct OSD_t {
        void   *data;
        int32_t width;
        int32_t height;
        int32_t size;
    }OSD;

    typedef struct EncCfgInfo {
        int32_t     width;
        int32_t     height;
        int32_t     horStride;
        int32_t     verStride;
        int32_t     format;         /* input yuv format */
        int32_t     type;           /* output format AVC/HEVC/VP9 and so on */
        int32_t     idrInterval;
        int32_t     bitrateMode;    /* 0 - VBR mode; 1 - CBR mode; 2 - FIXQP mode */
        int32_t     bitRate;        /* target bitrate ,0 - auto calculate */
        int32_t     framerate;      /* target framerate */
        int32_t     qp;             /* coding quality, from 1~51 */
        int32_t     scaleWidth;
        int32_t     scaleHeight;
        int32_t     profile;
        int32_t     level;
        int32_t     rotation;
        int32_t     osd_enable;
        OSD_t       osd;
    } EncCfgInfo_t;

    typedef struct EncoderOut {
        MppPacket   packet;
        int32_t     size;
        uint64_t    pts;
        bool        keyFrame;
        bool        eos;

        void release() { mpp_packet_deinit(&packet); }
    } EncoderOut_t;

    typedef struct {
        int32_t     dma_fd;
        int32_t     width;
        int32_t     height;
        int32_t     size;
    } DmaBuffer_t;

    MPP_RET prepare(EncCfgInfo *enc_cfg_info);
    MPP_RET sendFrame(DmaBuffer_t buffer,uint64_t pts,bool eos);
    MPP_RET getOutPacket(EncoderOut *out);
    MPP_RET getHdrPacket(MppPacket *hdr_pkt);

    MPP_RET drain();

    MPP_RET release();

    void sendFrame(int index,int pts);
    void import(int fd,int size,int index);
    void getCfgInfo(EncCfgInfo &info);
private:
    typedef struct EncoderCtx {
        MppCtx           mppCtx;
        MppApi          *api;
        MppBufferGroup   group;
        MppEncCfg        cfg;
        MppFrame         input_frame;
        EncCfgInfo       info;
        bool             spsPpsHeaderReceived;
        bool             eos;
        FILE            *inFile;
        FILE            *outFile;
    } EncoderCtx_t;

    EncoderCtx mCtx;
    MppBuffer mBuffer[4];
private:
    MppFrameFormat getFormat(uint32_t format);
    MPP_RET setupEncCfg();
    MPP_RET setupBaseCodec();
    MPP_RET setupFrameRate();
    MPP_RET setupBitRate();
    MPP_RET setupProfileParams();
    MPP_RET setupQp();
    RK_S32  getEncoderDefaultStride(int32_t width,MppFrameFormat fmt);
};

#endif // RKHWENCAPI_H
