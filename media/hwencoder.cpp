#include "hwencoder.h"
#include "log.h"

#define MODULE_TAG "HwEncoder"
using namespace std;

HwEncoder::HwEncoder():
    mEncState(ENC_STATE_UNINITIALIZED)
{
    mEncData = mpp_malloc(MpiEncData,1);
}

HwEncoder::~HwEncoder()
{
    MPP_FREE(mEncData);
    mEncState = ENC_STATE_STOPPED;
}

void HwEncoder::run()
{
    MpiEncData *p = mEncData;
    int frameIndex = 0;
    MppApi *mpi = p->mpi;
    MppCtx ctx = p->ctx;
    MPP_RET ret = MPP_OK;
    MppFrame frame = nullptr;
    RK_U32 eos = 0;
    mpp_frame_init(&frame);
    p->frm_eos = 0;

    mEncState = ENC_STATE_RUNNING;
    while(!mThreadExit) {
        mLock.lock();
        if(mCurrBufIndex < 0) {
            mLock.unlock();
            msleep(10);
            continue;
        }
        MppBufferInfo info;
        MppBuffer buffer;
        memset(&info, 0, sizeof(MppBufferInfo));
        info.type = MPP_BUFFER_TYPE_EXT_DMA;
        info.fd =  buf_fd;
        info.size = buf_size & 0x07ffffff;
        info.index = (buf_size & 0xf8000000) >> 27;
        mpp_buffer_import(&buffer, &info);

        mpp_frame_set_buffer(frame, buffer);
        mpp_buffer_put(buffer);
        mpp_frame_set_width(frame, p->width);
        mpp_frame_set_height(frame, p->height);
        mpp_frame_set_hor_stride(frame, p->hor_stride);
        mpp_frame_set_ver_stride(frame, p->ver_stride);
        mpp_frame_set_pts(frame, frameIndex);
        mpp_frame_set_fmt(frame, p->fmt);

        eos = p->frm_eos;
        mCurrBufIndex = -1;
        mpp_frame_set_eos(frame,eos);
        mLock.unlock();

        ret = mpi->encode_put_frame(ctx, frame);
        if (ret) {
            RLOGE("failed to put_frame, err %d", ret);
            mpp_frame_deinit(&frame);
            return;
        }

        MppPacket packet = nullptr;

        ret = mpi->encode_get_packet(ctx, &packet);
        if (ret) {
            continue;
        } else {
            int64_t  pts = mpp_packet_get_pts(packet);
            size_t   len = mpp_packet_get_length(packet);
            unsigned int eos = mpp_packet_get_eos(packet);
            void   *data = mpp_packet_get_data(packet);
            frameIndex++;
            int isIntra = 0;
            MppMeta meta = mpp_packet_get_meta(packet);
            mpp_meta_get_s32(meta, KEY_OUTPUT_INTRA, &isIntra);

            if(isIntra) {
                RLOGD("IDR frame produced");
            }
//            if (outFile)
//                fwrite(data, 1, len, outFile);

            //            mpp_packet_deinit(&packet);

            if(mMuxer) {
                mMuxer->sendPacket(packet);
            }
            if(eos) {
                mEncState = ENC_STATE_STOPING;
                break;
            }
        }
    }

    mpp_frame_deinit(&frame);

    mEncState = ENC_STATE_STOPPED;
}

bool HwEncoder::startTask()
{
    MpiEncData *p = mEncData;
    if(mEncState == ENC_STATE_STOPING ||
            mEncState == ENC_STATE_UNINITIALIZED ||
            mEncState == ENC_STATE_RUNNING ||
            mEncState == ENC_STATE_INITIALIZING) {
        RLOGE("HW Encoder state is err");
        return false;
    }
    RLOGD("HW Encoder start work");
    mThreadExit = false;
    start();

    {
        VIDEO_ENCODE_TYPE video_type = VIDEO_ENCODE_TYPE_AVC;
        switch (p->type) {
        case MPP_VIDEO_CodingAVC:
            video_type = VIDEO_ENCODE_TYPE_AVC;
            break;
        case MPP_VIDEO_CodingHEVC:
            video_type = VIDEO_ENCODE_TYPE_HEVC;
            break;
        default:
            break;
        };

        MppPacket hdrPkt = nullptr;
        void *hdrBuf = nullptr;
        uint32_t hdrBufSize = 1024;

        hdrBuf = malloc(hdrBufSize * sizeof(uint8_t));
        if (hdrBuf)
            mpp_packet_init(&hdrPkt, hdrBuf, hdrBufSize);

        if (hdrPkt) {
            p->mpi->control(p->ctx, MPP_ENC_GET_HDR_SYNC, hdrPkt);
        }

        if(!mMuxer)
            mMuxer = shared_ptr<Muxer>(new Muxer);
        mMuxer->createMuxCtx(p->width,p->height,video_type,p->fps_out_num,hdrPkt);

        free(hdrBuf);
        mpp_packet_deinit(&hdrPkt);
    }

    mMuxer->setMux(true,MUX_FILE_TYPE_MP4);
    mMuxer->startTask();

    return true;
}

void HwEncoder::stopTask()
{
    mLock.lock();
    mThreadExit = true;
    MpiEncData *p = mEncData;
    p->frm_eos = 1;
    mLock.unlock();
    if(isRunning()) {
        quit();
        wait(QUIT_TIMEOUT);
    }
    mCurrBufIndex = -1;
    mEncState = ENC_STATE_STOPPED;
    mMuxer->stopTask();
}

void HwEncoder::import_buf(int buf_fd, int buf_size, int index)
{
    mpp_log("get dma buf(%d)-fd: %d\n", index, buf_fd);
    MppBufferInfo info;
    memset(&info, 0, sizeof(MppBufferInfo));
    info.type = MPP_BUFFER_TYPE_EXT_DMA;
    info.fd =  buf_fd;
    info.size = buf_size & 0x07ffffff;
    info.index = (buf_size & 0xf8000000) >> 27;
    mpp_buffer_import(&buffer[index], &info);
}

void HwEncoder::createHwEncoder(int width, int height, int fmt)
{
    MPP_RET ret = MPP_OK;
    MpiEncData *p = mEncData;

    p->hor_stride = mpp_enc_with_default_stride(width,(MppFrameFormat)fmt);
    p->ver_stride = height;

    init_ctx(width,height,fmt);

    RLOGD("%p encoder test start w %d h %d type %d format %d hor_stride %d ver_stride %d\n",
          p->ctx, p->width, p->height, p->type,p->fmt,p->hor_stride,p->ver_stride);
    mEncState = ENC_STATE_INITIALIZING;

    ret = mpp_create(&p->ctx, &p->mpi);
    if (ret) {
        RLOGE("mpp_create failed ret %d\n", ret);
        goto error;
    }

    ret = p->mpi->control(p->ctx, MPP_SET_OUTPUT_TIMEOUT, &timeout);
    if (MPP_OK != ret) {
        RLOGE("mpi control set output timeout %d ret %d\n", timeout, ret);
        goto error;
    }

    ret = mpp_init(p->ctx, MPP_CTX_ENC, p->type);
    if (ret) {
        RLOGE("mpp_init failed ret %d\n", ret);
        goto error;
    }

    ret = mpp_enc_cfg_init(&p->cfg);
    if (ret) {
        RLOGE("mpp_enc_cfg_init failed ret %d\n", ret);
        goto error;
    }

    ret = p->mpi->control(p->ctx, MPP_ENC_GET_CFG, p->cfg);
    if (ret) {
        RLOGE("get enc cfg failed ret %d\n", ret);
        goto error;
    }

    ret = mpp_enc_cfg_setup();
    if(ret != MPP_OK) {
        RLOGE("mpp_enc_cfg_setup failed!\n");
        goto error;
    }

    outFile = fopen(test_file,"wb");
    if(!outFile) {
        mpp_err("out file open failed%s",strerror(errno));
    }
    inFile = fopen(test_in_file,"wb");
    if(!inFile) {
        mpp_err("in file open failed%s",strerror(errno));
    }
    mEncState = ENC_STATE_INITIALIZED;
    RLOGD("create Hw Encoder success!\n");
    return;
error:
    RLOGE("create Hw Encoder failed!\n");
    mEncState = ENC_STATE_STOPPED;
    return;
}

void HwEncoder::prepareSendframe(int buf_index,int fd,int size,void *ptr)
{
    if(buf_index < 0)
        return;
    mLock.lock();
    mCurrBufIndex = buf_index;
    buf_fd = fd;
    buf_size = size;
    vir_addr = ptr;
    //    pthread_cond_broadcast(&p_cond);
    mLock.unlock();
}

void HwEncoder::send_frame(int buf_index)
{
    if(buf_index < 0)
        return;

}

void HwEncoder::get_packet()
{
    MppPacket packet = nullptr;
    MPP_RET ret = MPP_OK;
    MpiEncData *p = mEncData;
    MppApi *mpi = p->mpi;

    MppCtx ctx = p->ctx;
    ret = mpi->encode_get_packet(ctx, &packet);
    if (ret) {
        return;
    } else {
        int64_t  pts = mpp_packet_get_pts(packet);
        size_t   len = mpp_packet_get_length(packet);
        unsigned int eos = mpp_packet_get_eos(packet);
        void   *data = mpp_packet_get_data(packet);
        if (outFile)
            fwrite(data, 1, len, outFile);

        mpp_packet_deinit(&packet);
    }
}

void HwEncoder::init_ctx(int width,int height,int fmt)
{
    MPP_RET ret = MPP_OK;
    MpiEncData *p = mEncData;
    // get paramter from cmd
    p->width        = width;
    p->height       = height;
    p->hor_stride   = (p->hor_stride) ? (p->hor_stride):(MPP_ALIGN(p->width, 16));
    p->ver_stride   = (p->ver_stride) ? (p->ver_stride):(MPP_ALIGN(p->height, 16));;
    p->fmt          = (MppFrameFormat)fmt;
    p->type         = MPP_VIDEO_CodingAVC;
    p->frm_eos      = 0;
    p->pkt_eos      = 0;
    p->rc_mode      = MPP_ENC_RC_MODE_CBR;
    p->qp_init      = 26;
    p->fps_in_den   = 1;
    p->fps_in_num   = 60;
    p->fps_out_den  = 1;
    p->fps_out_num  = 60;
    p->gop_len      = 60;
    p->mdinfo_size  = (MPP_VIDEO_CodingHEVC == p->type) ?
                (MPP_ALIGN(p->hor_stride, 32) >> 5) *
                (MPP_ALIGN(p->ver_stride, 32) >> 5) * 16 :
                (MPP_ALIGN(p->hor_stride, 64) >> 6) *
                (MPP_ALIGN(p->ver_stride, 16) >> 4) * 16;
    p->bps          = p->width * p->height / 8 * (p->fps_out_num / p->fps_out_den);
    p->bps_min      = p->bps * 15 / 16;
    p->bps_max      = p->bps * 17 / 16;
    if (p->type == MPP_VIDEO_CodingMJPEG && p->frame_num == 0) {
        RLOGD("jpege default encode only one frame. Use -n [num] for rc case\n");
        p->frame_num = 1;
    }
}

MPP_RET HwEncoder::mpp_enc_cfg_setup()
{
    MPP_RET ret;
    MpiEncData *p = mEncData;
    MppApi *mpi = p->mpi;
    MppCtx ctx = p->ctx;
    MppEncCfg cfg = p->cfg;

    mpp_enc_cfg_set_s32(cfg, "prep:width", p->width);
    mpp_enc_cfg_set_s32(cfg, "prep:height", p->height);
    mpp_enc_cfg_set_s32(cfg, "prep:hor_stride", p->hor_stride);
    mpp_enc_cfg_set_s32(cfg, "prep:ver_stride", p->ver_stride);
    mpp_enc_cfg_set_s32(cfg, "prep:format", p->fmt);

    mpp_enc_cfg_set_s32(cfg, "rc:mode", p->rc_mode);

    /* fix input / output frame rate */
    mpp_enc_cfg_set_s32(cfg, "rc:fps_in_flex", p->fps_in_flex);
    mpp_enc_cfg_set_s32(cfg, "rc:fps_in_num", p->fps_in_num);
    mpp_enc_cfg_set_s32(cfg, "rc:fps_in_denorm", p->fps_in_den);
    mpp_enc_cfg_set_s32(cfg, "rc:fps_out_flex", p->fps_out_flex);
    mpp_enc_cfg_set_s32(cfg, "rc:fps_out_num", p->fps_out_num);
    mpp_enc_cfg_set_s32(cfg, "rc:fps_out_denorm", p->fps_out_den);

    /* drop frame or not when bitrate overflow */
    mpp_enc_cfg_set_u32(cfg, "rc:drop_mode", MPP_ENC_RC_DROP_FRM_DISABLED);
    mpp_enc_cfg_set_u32(cfg, "rc:drop_thd", 20);        /* 20% of max bps */
    mpp_enc_cfg_set_u32(cfg, "rc:drop_gap", 1);         /* Do not continuous drop frame */

    /* setup bitrate for different rc_mode */
    mpp_enc_cfg_set_s32(cfg, "rc:bps_target", p->bps);
    switch (p->rc_mode) {
    case MPP_ENC_RC_MODE_FIXQP : {
        /* do not setup bitrate on FIXQP mode */
    } break;
    case MPP_ENC_RC_MODE_CBR : {
        /* CBR mode has narrow bound */
        mpp_enc_cfg_set_s32(cfg, "rc:bps_max", p->bps_max ? p->bps_max : p->bps * 17 / 16);
        mpp_enc_cfg_set_s32(cfg, "rc:bps_min", p->bps_min ? p->bps_min : p->bps * 15 / 16);
    } break;
    case MPP_ENC_RC_MODE_VBR :
    case MPP_ENC_RC_MODE_AVBR : {
        /* VBR mode has wide bound */
        mpp_enc_cfg_set_s32(cfg, "rc:bps_max", p->bps_max ? p->bps_max : p->bps * 17 / 16);
        mpp_enc_cfg_set_s32(cfg, "rc:bps_min", p->bps_min ? p->bps_min : p->bps * 1 / 16);
    } break;
    default : {
        /* default use CBR mode */
        mpp_enc_cfg_set_s32(cfg, "rc:bps_max", p->bps_max ? p->bps_max : p->bps * 17 / 16);
        mpp_enc_cfg_set_s32(cfg, "rc:bps_min", p->bps_min ? p->bps_min : p->bps * 15 / 16);
    } break;
    }

    /* setup qp for different codec and rc_mode */
    switch (p->type) {
    case MPP_VIDEO_CodingAVC :
    case MPP_VIDEO_CodingHEVC : {
        switch (p->rc_mode) {
        case MPP_ENC_RC_MODE_FIXQP : {
            RK_S32 fix_qp = p->qp_init;

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
            mpp_enc_cfg_set_s32(cfg, "rc:qp_init", p->qp_init ? p->qp_init : -1);
            mpp_enc_cfg_set_s32(cfg, "rc:qp_max", 51);
            mpp_enc_cfg_set_s32(cfg, "rc:qp_min", 10);
            mpp_enc_cfg_set_s32(cfg, "rc:qp_max_i", 51);
            mpp_enc_cfg_set_s32(cfg, "rc:qp_min_i", 10);
            mpp_enc_cfg_set_s32(cfg, "rc:qp_ip", 2);
        } break;
        default : {
            RLOGD("unsupport encoder rc mode %d\n", p->rc_mode);
        } break;
        }
    } break;
    case MPP_VIDEO_CodingVP8 : {
        /* vp8 only setup base qp range */
        mpp_enc_cfg_set_s32(cfg, "rc:qp_init", p->qp_init ? p->qp_init : 40);
        mpp_enc_cfg_set_s32(cfg, "rc:qp_max",  127);
        mpp_enc_cfg_set_s32(cfg, "rc:qp_min",  0);
        mpp_enc_cfg_set_s32(cfg, "rc:qp_max_i", 127);
        mpp_enc_cfg_set_s32(cfg, "rc:qp_min_i", 0);
        mpp_enc_cfg_set_s32(cfg, "rc:qp_ip", 6);
    } break;
    case MPP_VIDEO_CodingMJPEG : {
        /* jpeg use special codec config to control qtable */
        mpp_enc_cfg_set_s32(cfg, "jpeg:q_factor", p->qp_init ? p->qp_init : 80);
        mpp_enc_cfg_set_s32(cfg, "jpeg:qf_max", 99);
        mpp_enc_cfg_set_s32(cfg, "jpeg:qf_min", 1);
    } break;
    default : {
    } break;
    }

    /* setup codec  */
    mpp_enc_cfg_set_s32(cfg, "codec:type", p->type);
    switch (p->type) {
    case MPP_VIDEO_CodingAVC : {
        RK_U32 constraint_set;
        /*
            * H.264 profile_idc parameter
            * 66  - Baseline profile
            * 77  - Main profile
            * 100 - High profile
            */
        mpp_enc_cfg_set_s32(cfg, "h264:profile", 100);
        /*
            * H.264 level_idc parameter
            * 10 / 11 / 12 / 13    - qcif@15fps / cif@7.5fps / cif@15fps / cif@30fps
            * 20 / 21 / 22         - cif@30fps / half-D1@@25fps / D1@12.5fps
            * 30 / 31 / 32         - D1@25fps / 720p@30fps / 720p@60fps
            * 40 / 41 / 42         - 1080p@30fps / 1080p@30fps / 1080p@60fps
            * 50 / 51 / 52         - 4K@30fps
            */
        mpp_enc_cfg_set_s32(cfg, "h264:level", 42);
        mpp_enc_cfg_set_s32(cfg, "h264:cabac_en", 1);
        mpp_enc_cfg_set_s32(cfg, "h264:cabac_idc", 0);
        mpp_enc_cfg_set_s32(cfg, "h264:trans8x8", 1);

        mpp_env_get_u32("constraint_set", &constraint_set, 0);
        if (constraint_set & 0x3f0000)
            mpp_enc_cfg_set_s32(cfg, "h264:constraint_set", constraint_set);
    } break;
    case MPP_VIDEO_CodingHEVC :
    case MPP_VIDEO_CodingMJPEG :
    case MPP_VIDEO_CodingVP8 : {
    } break;
    default : {
        RLOGD("unsupport encoder coding type %d\n", p->type);
    } break;
    }

    p->split_mode = 0;
    p->split_arg = 0;
    p->split_out = 0;

    mpp_env_get_u32("split_mode", &p->split_mode, MPP_ENC_SPLIT_NONE);
    mpp_env_get_u32("split_arg", &p->split_arg, 0);
    mpp_env_get_u32("split_out", &p->split_out, 0);

    if (p->split_mode) {
        RLOGD("%p split mode %d arg %d out %d\n", ctx,
              p->split_mode, p->split_arg, p->split_out);
        mpp_enc_cfg_set_s32(cfg, "split:mode", p->split_mode);
        mpp_enc_cfg_set_s32(cfg, "split:arg", p->split_arg);
        mpp_enc_cfg_set_s32(cfg, "split:out", p->split_out);
    }


    // config gop_len and ref cfg
    mpp_enc_cfg_set_s32(cfg, "rc:gop", p->gop_len ? p->gop_len : p->fps_out_num * 2);
    ret = mpi->control(ctx, MPP_ENC_SET_CFG, cfg);
    if (ret) {
        RLOGE("mpi control enc set cfg failed ret %d\n", ret);
        goto RET;
    }


    if (p->type == MPP_VIDEO_CodingAVC || p->type == MPP_VIDEO_CodingHEVC) {
        p->header_mode = MPP_ENC_HEADER_MODE_EACH_IDR;
        ret = mpi->control(ctx, MPP_ENC_SET_HEADER_MODE, &p->header_mode);
        if (ret) {
            RLOGE("mpi control enc set header mode failed ret %d\n", ret);
            goto RET;
        }
    }

    /* setup test mode by env */
    mpp_env_get_u32("user_data_enable", &p->user_data_enable, 0);

RET:
    return ret;
}

RK_S32 HwEncoder::mpp_enc_with_default_stride(int width, MppFrameFormat fmt)
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
        mpp_err_f("do not support type %d\n", fmt);
    } break;
    }

    return stride;

}
