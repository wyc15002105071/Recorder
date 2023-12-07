#include "rkmpp_enc_api.h"
#include "common/log.h"
#include "media/mediautils.h"

#define MODULE_TAG "RKHWEncApi"

RKHWEncApi::RKHWEncApi()
{
    memset(&mCtx,0,sizeof(EncoderCtx));
}

RKHWEncApi::~RKHWEncApi()
{
    release();
}

MPP_RET RKHWEncApi::prepare(EncCfgInfo *enc_cfg_info)
{
    MPP_RET ret         = MPP_OK;
    EncoderCtx *p       = nullptr;
    EncCfgInfo *info    = nullptr;
    MppPollType timeout = MPP_POLL_BLOCK;

    memset(&mCtx,0,sizeof(EncoderCtx));
    memcpy(&mCtx.info,enc_cfg_info,sizeof(EncCfgInfo));

    p = &mCtx;
    info = &mCtx.info;

    if(info->width <= 0 || info->height <= 0) {
        RLOGE("invaild parameter width %d height %d",info->width,info->height);
        return MPP_ERR_VALUE;
    }

    ret = mpp_create(&p->mppCtx, &p->api);
    if (ret) {
        RLOGE("mpp_create failed ret %d\n", ret);
        return ret;
    }

    ret = p->api->control(p->mppCtx, MPP_SET_OUTPUT_TIMEOUT, &timeout);
    if (MPP_OK != ret) {
        RLOGE("mpi control set output timeout %d ret %d\n", timeout, ret);
        goto error;
    }

    ret = mpp_init(p->mppCtx, MPP_CTX_ENC, (MppCodingType)info->type);
    if (ret) {
        RLOGE("mpp_init failed ret %d\n", ret);
        goto error;
    }

    ret = setupEncCfg();
    if (ret) {
        RLOGE("failed to set config, ret=0x%x", ret);
        goto error;
    }

    return MPP_OK;
error:
    release();
    return ret;
}

MPP_RET RKHWEncApi::sendFrame(DmaBuffer_t buffer,uint64_t pts,bool eos)
{
    MPP_RET ret;
    EncoderCtx *p       = &mCtx;
    EncCfgInfo *info    = &mCtx.info;

    if(buffer.dma_fd < 0 || buffer.size < 0) {
        return MPP_NOK;
    }

    p->eos = eos;

    MppBuffer mpp_buffer = nullptr;
    MppBufferInfo buffer_info;

    if(!p->input_frame)
        mpp_frame_init(&p->input_frame);

    memset(&buffer_info, 0, sizeof(MppBufferInfo));
    buffer_info.type = MPP_BUFFER_TYPE_EXT_DMA;
    buffer_info.fd =  buffer.dma_fd;
    buffer_info.size = buffer.size & 0x07ffffff;
    buffer_info.index = (buffer.size & 0xf8000000) >> 27;
    mpp_buffer_import(&mpp_buffer, &buffer_info);
    mpp_frame_set_buffer(p->input_frame, mpp_buffer);
    mpp_buffer_put(mpp_buffer);

    if(eos) {
        RLOGD("send input eos");
        mpp_frame_set_eos(p->input_frame,1);
    }

    mpp_frame_set_width(p->input_frame, info->width);
    mpp_frame_set_height(p->input_frame, info->height);
    mpp_frame_set_hor_stride(p->input_frame, info->horStride);
    mpp_frame_set_ver_stride(p->input_frame, info->verStride);
    mpp_frame_set_pts(p->input_frame, pts);
    mpp_frame_set_dts(p->input_frame, pts);
    mpp_frame_set_fmt(p->input_frame,(MppFrameFormat)info->format);
    mpp_frame_set_eos(p->input_frame,eos);

    ret = p->api->encode_put_frame(p->mppCtx, p->input_frame);
    if (ret) {
        RLOGE("failed to put_frame, err %d", ret);
        mpp_frame_deinit(&p->input_frame);
        return MPP_NOK;
    }

    return MPP_OK;
}

MPP_RET RKHWEncApi::getOutPacket(EncoderOut *out)
{
    EncoderCtx *p       = &mCtx;
    EncCfgInfo *info    = &mCtx.info;
    MPP_RET     ret;

    memset(out,0,sizeof(EncoderOut));

    ret = p->api->encode_get_packet(p->mppCtx, &out->packet);
    if (ret) {
        mpp_packet_deinit(&out->packet);
        return ret;
    } else {
        out->pts = mpp_packet_get_pts(out->packet);
        out->size = mpp_packet_get_length(out->packet);
        out->eos = mpp_packet_get_eos(out->packet);

        int32_t isIntra = 0;
        MppMeta meta = mpp_packet_get_meta(out->packet);
        mpp_meta_get_s32(meta, KEY_OUTPUT_INTRA, &isIntra);
        out->keyFrame = isIntra;
    }

    return MPP_OK;
}

MPP_RET RKHWEncApi::getHdrPacket(MppPacket *hdr_pkt)
{
    EncoderCtx *p       = &mCtx;

    void *hdrBuf = nullptr;
    uint32_t hdrBufSize = 1024;

    hdrBuf = malloc(hdrBufSize * sizeof(uint8_t));
    if (hdrBuf)
        mpp_packet_init(hdr_pkt, hdrBuf, hdrBufSize);

    if (*hdr_pkt) {
        p->api->control(p->mppCtx, MPP_ENC_GET_HDR_SYNC, *hdr_pkt);
    }


    return MPP_OK;
}

MPP_RET RKHWEncApi::drain()
{
    RLOGD("drain in");
    while(true) {
        EncoderOut encOut;
        MPP_RET ret = getOutPacket(&encOut);
        if(ret == MPP_OK) {
            encOut.release();
        }else {
            break;
        }
    }
    RLOGD("drain out");
    return MPP_OK;
}

MPP_RET RKHWEncApi::release()
{
    if (mCtx.cfg) {
        mpp_enc_cfg_deinit(mCtx.cfg);
        mCtx.cfg = nullptr;
    }

    if (mCtx.input_frame) {
        mpp_frame_deinit(&mCtx.input_frame);
    }

    if (mCtx.mppCtx) {
        mpp_destroy(mCtx.mppCtx);
        mCtx.mppCtx = nullptr;
    }

    if (mCtx.inFile != nullptr) {
        fclose(mCtx.inFile);
        mCtx.inFile = nullptr;
    }

    if (mCtx.outFile != nullptr) {
        fclose(mCtx.outFile);
        mCtx.outFile = nullptr;
    }

    return MPP_OK;
}

void RKHWEncApi::sendFrame(int index, int pts)
{
    MPP_RET ret;
    MppFrame frame = nullptr;
    mpp_frame_init(&frame);

    mpp_frame_set_buffer(frame, mBuffer[index]);
//    mpp_buffer_put(mBuffer[index]);
    mpp_frame_set_width(frame, 3840);
    mpp_frame_set_height(frame, 2160);
    mpp_frame_set_hor_stride(frame, 3840);
    mpp_frame_set_ver_stride(frame, 2160);
    mpp_frame_set_pts(frame, pts);
    mpp_frame_set_fmt(frame, MPP_FMT_YUV420SP);

    ret = mCtx.api->encode_put_frame(mCtx.mppCtx, frame);
    if(ret) {
        RLOGD("put frame failed %d",ret);
    }

    if (frame) {
        mpp_frame_deinit(&frame);
    }
}

void RKHWEncApi::import(int fd, int size, int index)
{
    MppBufferInfo buffer_info;


    memset(&buffer_info, 0, sizeof(MppBufferInfo));
    buffer_info.type = MPP_BUFFER_TYPE_EXT_DMA;
    buffer_info.fd =  fd;
    buffer_info.size = size & 0x07ffffff;
    buffer_info.index = (size & 0xf8000000) >> 27;
    mpp_buffer_import(&mBuffer[index], &buffer_info);


    int ffd = mpp_buffer_get_fd(mBuffer[index]);
    RLOGD("impot buf fd %d,size %d",ffd,size);
}

MppFrameFormat RKHWEncApi::getFormat(uint32_t format)
{
    MppFrameFormat fmt = MPP_FMT_RGB888;
    const char *pixel_fmt = fcc2s(format).c_str();
    if(!strcmp(pixel_fmt,"BGR3")) {
        fmt = MPP_FMT_BGR888;
    }
    else if(!strcmp(pixel_fmt,"RGB3")) {
        fmt = MPP_FMT_BGR888;
    }
    else if(!strcmp(pixel_fmt,"NV12")) {
        fmt = MPP_FMT_YUV420SP;
    }
    else if(!strcmp(pixel_fmt,"NV21")) {
        fmt = MPP_FMT_YUV420SP;
    }
    else if(!strcmp(pixel_fmt,"NV16")) {
        fmt = MPP_FMT_YUV422SP;
    }

    return fmt;
}

MPP_RET RKHWEncApi::setupEncCfg()
{
    MPP_RET ret = MPP_OK;
    EncoderCtx *p       = &mCtx;
    EncCfgInfo *info    = &mCtx.info;
    ret = mpp_enc_cfg_init(&mCtx.cfg);
    if (ret) {
        RLOGE("failed to get enc_cfg, ret %d", ret);
        return MPP_NOK;
    }

    setupBaseCodec();

    setupFrameRate();

    setupBitRate();

    setupProfileParams();

    setupQp();

    ret = p->api->control(p->mppCtx, MPP_ENC_SET_CFG, mCtx.cfg);
    if (ret) {
        RLOGE("failed to setup codec cfg, ret %d", ret);
        ret = MPP_NOK;
    } else {
        /* optional */
        MppEncSeiMode seiMode = MPP_ENC_SEI_MODE_DISABLE;
        ret = p->api->control(p->mppCtx, MPP_ENC_SET_SEI_CFG, &seiMode);
        if (ret) {
            RLOGE("failed to setup sei cfg, ret %d", ret);
            ret = MPP_NOK;
        }

        if (info->type == MPP_VIDEO_CodingAVC || info->type == MPP_VIDEO_CodingHEVC) {
            MppEncHeaderMode header_mode = MPP_ENC_HEADER_MODE_EACH_IDR;
            ret = p->api->control(p->mppCtx, MPP_ENC_SET_HEADER_MODE, &header_mode);
            if (ret) {
                RLOGE("mpi control enc set header mode failed ret %d\n", ret);
                return ret;
            }
        }
    }

    return ret;
}

MPP_RET RKHWEncApi::setupBaseCodec()
{
    EncoderCtx *p       = &mCtx;
    EncCfgInfo *info    = &mCtx.info;
    MppEncCfg  cfg      = mCtx.cfg;

    info->format = getFormat(info->format);

    info->horStride = info->horStride ? info->horStride:getEncoderDefaultStride(info->width,(MppFrameFormat)info->format);
    info->verStride = info->verStride ? info->verStride:info->height;

    RLOGD("width %d height %d hor_stride %d ver_stride %d format %d type %d",
          info->width,info->height,info->horStride,info->verStride,info->format,info->type);

    mpp_enc_cfg_set_s32(cfg, "codec:type", info->type);
    mpp_enc_cfg_set_s32(cfg, "prep:width", info->width);
    mpp_enc_cfg_set_s32(cfg, "prep:height", info->height);
    mpp_enc_cfg_set_s32(cfg, "prep:hor_stride", info->horStride);
    mpp_enc_cfg_set_s32(cfg, "prep:ver_stride", info->verStride);
    mpp_enc_cfg_set_s32(cfg, "prep:format", (MppFrameFormat)info->format);

    return MPP_OK;
}

MPP_RET RKHWEncApi::setupFrameRate()
{
    EncoderCtx *p       = &mCtx;
    EncCfgInfo *info    = &mCtx.info;
    MppEncCfg   cfg     =  mCtx.cfg;
    uint32_t    gop     = 0;
    if(info->framerate == 0)
        info->framerate = 60;
    if(info->idrInterval == 0)
        info->idrInterval = 1;
    gop = info->idrInterval * info->framerate;
    RLOGD("framerate %d gop %d",info->framerate,gop);

    mpp_enc_cfg_set_s32(cfg, "rc:gop", gop);
    mpp_enc_cfg_set_s32(cfg, "rc:fps_in_flex", 0);
    mpp_enc_cfg_set_s32(cfg, "rc:fps_in_num", info->framerate);
    mpp_enc_cfg_set_s32(cfg, "rc:fps_in_denorm", 1);
    mpp_enc_cfg_set_s32(cfg, "rc:fps_out_flex", 0);
    mpp_enc_cfg_set_s32(cfg, "rc:fps_out_num", info->framerate);
    mpp_enc_cfg_set_s32(cfg, "rc:fps_out_denorm", 1);
    /* drop frame or not when bitrate overflow */
    mpp_enc_cfg_set_u32(cfg, "rc:drop_mode", MPP_ENC_RC_DROP_FRM_DISABLED);
    mpp_enc_cfg_set_u32(cfg, "rc:drop_thd", 20);        /* 20% of max bps */
    mpp_enc_cfg_set_u32(cfg, "rc:drop_gap", 1);         /* Do not continuous drop frame */
    return MPP_OK;
}

MPP_RET RKHWEncApi::setupBitRate()
{
    EncoderCtx *p       = &mCtx;
    EncCfgInfo *info    = &mCtx.info;
    MppEncCfg   cfg     =  mCtx.cfg;

    int32_t bps          = info->width * info->height / 8 * info->framerate;
    int32_t bps_min      = bps * 15 / 16;
    int32_t bps_max      = bps * 17 / 16;

    RLOGD("bitrate mode %d bps %d bps_min %d bps_max %d",info->bitrateMode,bps,bps_min,bps_max);

    mpp_enc_cfg_set_s32(cfg, "rc:mode", info->bitrateMode);
    /* setup bitrate for different rc_mode */
    mpp_enc_cfg_set_s32(cfg, "rc:bps_target", bps);
    switch (info->bitrateMode) {
    case MPP_ENC_RC_MODE_FIXQP : {
        /* do not setup bitrate on FIXQP mode */
    } break;
    case MPP_ENC_RC_MODE_CBR : {
        /* CBR mode has narrow bound */
        mpp_enc_cfg_set_s32(cfg, "rc:bps_max", bps_max ? bps_max : bps * 17 / 16);
        mpp_enc_cfg_set_s32(cfg, "rc:bps_min", bps_min ? bps_min : bps * 15 / 16);
    } break;
    case MPP_ENC_RC_MODE_VBR :
    case MPP_ENC_RC_MODE_AVBR : {
        /* VBR mode has wide bound */
        mpp_enc_cfg_set_s32(cfg, "rc:bps_max", bps_max ? bps_max : bps * 17 / 16);
        mpp_enc_cfg_set_s32(cfg, "rc:bps_min", bps_min ? bps_min : bps * 1 / 16);
    } break;
    default : {
        /* default use CBR mode */
        mpp_enc_cfg_set_s32(cfg, "rc:bps_max", bps_max ? bps_max : bps * 17 / 16);
        mpp_enc_cfg_set_s32(cfg, "rc:bps_min", bps_min ? bps_min : bps * 15 / 16);
    } break;
    }

    return MPP_OK;
}

MPP_RET RKHWEncApi::setupProfileParams()
{
    EncoderCtx *p       = &mCtx;
    EncCfgInfo *info    = &mCtx.info;
    MppEncCfg   cfg     =  mCtx.cfg;

    if(info->profile == 0)
        info->profile = 100;
    if(info->level == 0)
        info->level = 42;
    switch (info->type) {
    case MPP_VIDEO_CodingAVC : {
        RK_U32 constraint_set;
        /*
            * H.264 profile_idc parameter
            * 66  - Baseline profile
            * 77  - Main profile
            * 100 - High profile
            */
        mpp_enc_cfg_set_s32(cfg, "h264:profile", info->profile);
        /*
            * H.264 level_idc parameter
            * 10 / 11 / 12 / 13    - qcif@15fps / cif@7.5fps / cif@15fps / cif@30fps
            * 20 / 21 / 22         - cif@30fps / half-D1@@25fps / D1@12.5fps
            * 30 / 31 / 32         - D1@25fps / 720p@30fps / 720p@60fps
            * 40 / 41 / 42         - 1080p@30fps / 1080p@30fps / 1080p@60fps
            * 50 / 51 / 52         - 4K@30fps
            */
        mpp_enc_cfg_set_s32(cfg, "h264:level", info->level);
        mpp_enc_cfg_set_s32(cfg, "h264:cabac_en", 1);
        mpp_enc_cfg_set_s32(cfg, "h264:cabac_idc", 0);
        mpp_enc_cfg_set_s32(cfg, "h264:trans8x8", 1);

        mpp_env_get_u32("constraint_set", &constraint_set, 0);
        if (constraint_set & 0x3f0000)
            mpp_enc_cfg_set_s32(cfg, "h264:constraint_set", constraint_set);
    } break;
    case MPP_VIDEO_CodingHEVC  :
    case MPP_VIDEO_CodingVP9   :
    case MPP_VIDEO_CodingMJPEG :
    case MPP_VIDEO_CodingVP8   : {
    } break;
    default : {
        RLOGE("unsupport encoder coding type %d\n", info->type);
    } break;
    }

    return MPP_OK;
}

MPP_RET RKHWEncApi::setupQp()
{
    EncoderCtx *p       = &mCtx;
    EncCfgInfo *info    = &mCtx.info;
    MppEncCfg   cfg     =  mCtx.cfg;

    switch (info->type) {
    case MPP_VIDEO_CodingAVC :
    case MPP_VIDEO_CodingHEVC : {
        switch (info->bitrateMode) {
        case MPP_ENC_RC_MODE_FIXQP : {
            RK_S32 fix_qp = info->qp ? info->qp : 26;

            mpp_enc_cfg_set_s32(cfg, "rc:qp_init", fix_qp);
            mpp_enc_cfg_set_s32(cfg, "rc:qp_max", fix_qp);
            mpp_enc_cfg_set_s32(cfg, "rc:qp_min", fix_qp);
            mpp_enc_cfg_set_s32(cfg, "rc:qp_max_i", fix_qp);
            mpp_enc_cfg_set_s32(cfg, "rc:qp_min_i", fix_qp);
            mpp_enc_cfg_set_s32(cfg, "rc:qp_ip", 0);
        } break;
        case MPP_ENC_RC_MODE_CBR :
        case MPP_ENC_RC_MODE_VBR :
        case MPP_ENC_RC_MODE_AVBR : {
            mpp_enc_cfg_set_s32(cfg, "rc:qp_init", info->qp ? info->qp : -1);
            mpp_enc_cfg_set_s32(cfg, "rc:qp_max", 51);
            mpp_enc_cfg_set_s32(cfg, "rc:qp_min", 10);
            mpp_enc_cfg_set_s32(cfg, "rc:qp_max_i", 51);
            mpp_enc_cfg_set_s32(cfg, "rc:qp_min_i", 10);
            mpp_enc_cfg_set_s32(cfg, "rc:qp_ip", 2);
        } break;
        default : {
            RLOGE("unsupport encoder rc mode %d\n", info->bitrateMode);
        } break;
        }
    } break;
    case MPP_VIDEO_CodingVP8 : {
        /* vp8 only setup base qp range */
        mpp_enc_cfg_set_s32(cfg, "rc:qp_init", info->qp ? info->qp : 40);
        mpp_enc_cfg_set_s32(cfg, "rc:qp_max",  127);
        mpp_enc_cfg_set_s32(cfg, "rc:qp_min",  0);
        mpp_enc_cfg_set_s32(cfg, "rc:qp_max_i", 127);
        mpp_enc_cfg_set_s32(cfg, "rc:qp_min_i", 0);
        mpp_enc_cfg_set_s32(cfg, "rc:qp_ip", 6);
    } break;
    case MPP_VIDEO_CodingMJPEG : {
        /* jpeg use special codec config to control qtable */
        mpp_enc_cfg_set_s32(cfg, "jpeg:q_factor", info->qp ? info->qp : 80);
        mpp_enc_cfg_set_s32(cfg, "jpeg:qf_max", 99);
        mpp_enc_cfg_set_s32(cfg, "jpeg:qf_min", 1);
    } break;
    default : {
    } break;
    }
    return MPP_OK;
}


RK_S32 RKHWEncApi::getEncoderDefaultStride(int32_t width, MppFrameFormat fmt)
{
    RK_S32 stride = 0;

    switch (fmt & MPP_FRAME_FMT_MASK) {
    case MPP_FMT_YUV420SP :
    case MPP_FMT_YUV420SP_VU : {
        stride = MPP_ALIGN(width, 8);
    } break;
    case MPP_FMT_YUV420P : {
        /* NOTE: 420P need to align to 16 so chroma can align to 8 */
        stride = MPP_ALIGN(width, 16);
    } break;
    case MPP_FMT_YUV422P:
    case MPP_FMT_YUV422SP:
    case MPP_FMT_YUV422SP_VU: {
        /* NOTE: 422 need to align to 8 so chroma can align to 16 */
        stride = MPP_ALIGN(width, 8);
    } break;
    case MPP_FMT_YUV444SP :
    case MPP_FMT_YUV444P : {
        stride = MPP_ALIGN(width, 8);
    } break;
    case MPP_FMT_RGB565:
    case MPP_FMT_BGR565:
    case MPP_FMT_RGB555:
    case MPP_FMT_BGR555:
    case MPP_FMT_RGB444:
    case MPP_FMT_BGR444:
    case MPP_FMT_YUV422_YUYV :
    case MPP_FMT_YUV422_YVYU :
    case MPP_FMT_YUV422_UYVY :
    case MPP_FMT_YUV422_VYUY : {
        /* NOTE: for vepu limitation */
        stride = MPP_ALIGN(width, 8) * 2;
    } break;
    case MPP_FMT_RGB888 :
    case MPP_FMT_BGR888 : {
        /* NOTE: for vepu limitation */
        stride = MPP_ALIGN(width, 8) * 3;
    } break;
    case MPP_FMT_RGB101010 :
    case MPP_FMT_BGR101010 :
    case MPP_FMT_ARGB8888 :
    case MPP_FMT_ABGR8888 :
    case MPP_FMT_BGRA8888 :
    case MPP_FMT_RGBA8888 : {
        /* NOTE: for vepu limitation */
        stride = MPP_ALIGN(width, 8) * 4;
    } break;
    default : {
        RLOGE("do not support type %d\n", fmt);
    } break;
    }

    return stride;
}
