#include "rkmpp_dec_api.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "common/log.h"
#include "media/rkrgadef.h"
#include "mpp_common.h"
#define MODULE_TAG "RKHWDecApi"

RKHWDecApi::RKHWDecApi()
{
    memset(&mDecCtx,0,sizeof(DecoderCtx));
    RLOGD("construct");
}

RKHWDecApi::~RKHWDecApi()
{
    RLOGD("destruct");
    release();
}

int RKHWDecApi::prepare(DecCfgInfo &info)
{
    int32_t ret = MPP_NOK;
    MppFrame frame  = nullptr;
    DecoderCtx *p = &mDecCtx;
    int32_t buf_size = 0;
    ret = mpp_create(&p->mppCtx,&p->mppApi);
    if(ret != MPP_OK) {
        RLOGD("mpp create failed\n");
        goto err;
    }

    {
        uint32_t fastParser = 1;
        p->mppApi->control(p->mppCtx, MPP_DEC_SET_PARSER_FAST_MODE, &fastParser);
    }

    ret = mpp_init(p->mppCtx,MPP_CTX_DEC,info.type);
    if(ret != MPP_OK) {
        RLOGE("mpp init failed\n");
        goto err;
    }

    p->info.hor_stride  = MPP_ALIGN(info.width,16);
    p->info.ver_stride  = MPP_ALIGN(info.height,16);
    p->info.width       = info.width;
    p->info.height      = info.height;
    p->info.out_fmt     = MPP_FMT_YUV420SP;
    p->info.type        = info.type;
    p->info.low_latency = info.low_latency;
    mpp_frame_init(&frame);
    mpp_frame_set_width(frame, p->info.width);
    mpp_frame_set_height(frame, p->info.height);
    mpp_frame_set_fmt(frame, (MppFrameFormat)p->info.out_fmt);
    mpp_frame_set_hor_stride(frame, p->info.hor_stride);
    mpp_frame_set_ver_stride(frame, p->info.ver_stride);
    p->mppApi->control(p->mppCtx, MPP_DEC_SET_FRAME_INFO, (MppParam)frame);
    p->mppApi->control(p->mppCtx, MPP_DEC_SET_OUTPUT_FORMAT, (MppParam)&p->info.out_fmt);
    buf_size = mpp_frame_get_buf_size(frame);
    mpp_frame_deinit(&frame);

    if (p->info.low_latency || p->info.thumbnail) {
        uint32_t deinterlace = 0, immediate = 1;
        RLOGD("enable lowLatency, enable mpp immediate-out mode");
        p->mppApi->control(p->mppCtx, MPP_DEC_SET_ENABLE_DEINTERLACE, &deinterlace);
        p->mppApi->control(p->mppCtx, MPP_DEC_SET_IMMEDIATE_OUT, &immediate);
    }

        ret = mpp_buffer_group_get_internal(&p->group, MPP_BUFFER_TYPE_DRM);

    //    /* Set buffer to mpp decoder */
    //    ret = p->mppApi->control(p->mppCtx, MPP_DEC_SET_EXT_BUF_GROUP, p->group);
    //    if (ret) {
    //        mpp_err("%p set buffer group failed ret %d\n", p->mppCtx, ret);
    //        goto err;
    //    }

    //    ret = mpp_buffer_group_limit_config(p->group, buf_size, 24);
    //    if (ret) {
    //        mpp_err("%p//    dst_frame->fd           = mpp_buffer_get_fd(mppBuffer);
    //    dst_frame->width        = src_frame->hor_stride;
    //    dst_frame->height       = src_frame->ver_stride;
    //    dst_frame->hor_stride   = MPP_ALIGN(src_frame->hor_stride,8) * 4;
    //    dst_frame->ver_stride   = src_frame->ver_stride;
    //    dst_frame->vir_addr     = mpp_buffer_get_ptr(mppBuffer);
    //    dst_frame->size         = src_frame->hor_stride * src_frame->ver_stride * 4;
    //    dst_frame->handler      = frame; limit buffer group failed ret %d\n", p->mppCtx, ret);
    //        goto err;
    //    }

    //    /*
    //                         * All buffer group config done. Set info change ready to let
    //                         * decoder continue decoding
    //                         */
    //    ret = p->mppApi->control(p->mppCtx, MPP_DEC_SET_INFO_CHANGE_READY, NULL);
    //    if (ret) {
    //        mpp_err("%p info change ready failed ret %d\n", p->mppCtx, ret);
    //        goto err;
    //    }

    RLOGD("init: width [%d:%d] stride [%d:%d] type %d format %d\n",
          p->info.width,p->info.height,p->info.hor_stride, p->info.ver_stride,p->info.type,p->info.out_fmt);

    p->initOk = 1;
    return ret;
err:
    if(p->mppCtx) {
        mpp_destroy(p->mppCtx);
        p->mppCtx = nullptr;
    }
    p->initOk = 0;
    return ret;
}

void RKHWDecApi::release()
{
    DecoderCtx *p = &mDecCtx;
    if(p->group) {
        mpp_buffer_group_clear(p->group);
        mpp_buffer_group_put(p->group);
        p->group = nullptr;
    }
    if(p->mppCtx) {
        p->mppApi->reset(p->mppCtx);
        mpp_destroy(p->mppCtx);
        p->mppCtx = nullptr;
    }

    memset(p,0,sizeof(DecoderCtx));
}

int RKHWDecApi::sendPacket(uint8_t *data, int32_t size, int64_t pts, int64_t dts,int32_t flags)
{
    DecoderCtx *p = &mDecCtx;
    MPP_RET ret = MPP_OK;
    if(!p->initOk || !p->mppCtx) {
        return MPP_NOK;
    }

    MppPacket packet = nullptr;

    mpp_packet_init(&packet, data, size);
    mpp_packet_set_pts(packet, pts);
    mpp_packet_set_dts(packet, dts);
    mpp_packet_set_pos(packet, data);
    mpp_packet_set_length(packet, size);

    if (flags & FLAG_END_OF_STREAM) {
        RLOGD("send input eos");
        mpp_packet_set_eos(packet);
    }

    if (flags & FLAG_CODEC_CONFIG) {
        mpp_packet_set_extra_data(packet);
    }

    ret = p->mppApi->decode_put_packet(p->mppCtx, packet);
    RLOGD("decode send %d\n",ret);
    mpp_packet_deinit(&packet);

    return ret;
}

int RKHWDecApi::getOutputFrame(RKHWDecApi::OutputFrame *out_frame)
{
    MPP_RET ret;
    DecoderCtx *p = &mDecCtx;
    MppFrame frame = nullptr;
    int tryCount = 0;

    if(!p->initOk || !p->mppCtx) {
        return MPP_NOK;
    }
REDO:
    ret = p->mppApi->decode_get_frame(p->mppCtx, &frame);
    tryCount++;
    if (MPP_OK != ret || !frame) {
        if (tryCount < 100) {
            RLOGD("need to get frame\n");
            usleep(5 * 1000);
            goto REDO;
        }
        return MPP_NOK;
    }
    if(ret == MPP_OK) {
        uint32_t width  = mpp_frame_get_width(frame);
        uint32_t height = mpp_frame_get_height(frame);
        uint32_t hstride = mpp_frame_get_hor_stride(frame);
        uint32_t vstride = mpp_frame_get_ver_stride(frame);
        MppFrameFormat format = mpp_frame_get_fmt(frame);
        uint32_t err = mpp_frame_get_errinfo(frame);
        uint32_t eos = mpp_frame_get_eos(frame);
        MppBuffer mppBuffer = mpp_frame_get_buffer(frame);
        uint64_t pts = mpp_frame_get_pts(frame);
        RLOGD("decode get %d\n",ret);
        RLOGD("get one frame [%d:%d] stride [%d:%d] pts %ld err %d eos %d fmt %d\n",
              width, height, hstride, vstride, pts, err, eos,format);

        out_frame->width = width;
        out_frame->height = height;
        out_frame->hor_stride = hstride;
        out_frame->ver_stride = vstride;
        out_frame->size   = mpp_frame_get_buf_size(frame);
        out_frame->fd = mpp_buffer_get_fd_with_caller(mppBuffer,nullptr);
        out_frame->vir_addr = mpp_buffer_get_ptr_with_caller(mppBuffer,nullptr);
        out_frame->handler = frame;
        out_frame->buffer = mppBuffer;
        FILE *fp = fopen("test.yuv","wb");
        fwrite(out_frame->vir_addr,1,out_frame->width*out_frame->height*3/2,fp);
        fclose(fp);

    }

    return ret;
}

void RKHWDecApi::deinitOutputFrame(RKHWDecApi::OutputFrame *out_frame)
{
    RLOGD("deinit outframe");

    mpp_buffer_put(out_frame->buffer);
    mpp_frame_deinit(&out_frame->handler);
    out_frame->buffer = nullptr;
    out_frame->handler = nullptr;
}

MppCodingType RKHWDecApi::formatFF2Mpp(AVCodecID codecid)
{
    switch (codecid) {
    case AV_CODEC_ID_H264:
        return  MPP_VIDEO_CodingAVC;
    case AV_CODEC_ID_HEVC:
        return MPP_VIDEO_CodingHEVC;
    case AV_CODEC_ID_MJPEG:
        return MPP_VIDEO_CodingMJPEG;
    case AV_CODEC_ID_VP8:
        return MPP_VIDEO_CodingVP8;
    case AV_CODEC_ID_VP9:
        return MPP_VIDEO_CodingVP9;
    default:
        return MPP_VIDEO_CodingUnused;
    }
}

int RKHWDecApi::convertNV12ToRGB(RKHWDecApi::OutputFrame *src_frame, RKHWDecApi::OutputFrame *dst_frame)
{
    int ret = -1;
    if(src_frame->fd <= 0) {
        return ret;
    }
    DecoderCtx *p = &mDecCtx;
    RKRgaDef::RgaInfo srcInfo, dstInfo;
    memset(dst_frame,0,sizeof(RKHWDecApi::OutputFrame));

    MppBuffer mppBuffer = nullptr;
    MppBuffer frame = nullptr;

    mpp_frame_init(&frame);
    mpp_buffer_get(p->group,&mppBuffer,src_frame->hor_stride * src_frame->ver_stride * 4);
    mpp_frame_set_buffer(frame,mppBuffer);

    dst_frame->fd           = mpp_buffer_get_fd(mppBuffer);
    dst_frame->width        = src_frame->hor_stride;
    dst_frame->height       = src_frame->ver_stride;
    dst_frame->hor_stride   = MPP_ALIGN(src_frame->hor_stride,8) * 4;
    dst_frame->ver_stride   = src_frame->ver_stride;
    dst_frame->vir_addr     = mpp_buffer_get_ptr(mppBuffer);
    dst_frame->size         = src_frame->hor_stride * src_frame->ver_stride * 4;
    dst_frame->handler      = frame;
    dst_frame->buffer       = mppBuffer;
    RKRgaDef::SetRgaInfo(&srcInfo, src_frame->fd, src_frame->hor_stride, src_frame->ver_stride, MPP_ALIGN(src_frame->hor_stride,4), MPP_ALIGN(src_frame->ver_stride,2));
    RKRgaDef::SetRgaInfo(&dstInfo, dst_frame->fd, dst_frame->width, dst_frame->height, dst_frame->width, dst_frame->height);

    if(!RKRgaDef::NV12ToRGB(srcInfo,dstInfo)) {
        RLOGE("NV12ToRGB failed\n");
        ret = -1;
    } else {
        RLOGD("NV12ToRGB success\n");
    }
//    mpp_buffer_inc_ref(mppBuffer);

    //    deinitOutputFrame(src_frame);

    return ret;
}
