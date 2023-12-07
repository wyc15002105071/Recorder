#ifndef HWENCODER_H
#define HWENCODER_H

#include "rk_mpi.h"
#include "mpp_mem.h"
#include "mpp_common.h"
#include "mpp_log.h"
#include "mpp_env.h"
#include "rthread.h"
#include "muxer.h"
#include <memory>

class HwEncoder : public RThread
{
public:
    HwEncoder();
    ~HwEncoder();
    virtual void run();
    virtual bool startTask();
    virtual void stopTask();
    void import_buf(int buf_fd,int buf_size,int index);
    void createHwEncoder(int width,int height,int fmt);

    void prepareSendframe(int buf_index,int fd,int size,void *ptr);
    void send_frame(int buf_index);
    void get_packet();

private:
    typedef enum {
        ENC_STATE_UNINITIALIZED,
        ENC_STATE_STOPPED,
        ENC_STATE_STOPING,
        ENC_STATE_INITIALIZING,
        ENC_STATE_INITIALIZED,
        ENC_STATE_RUNNING,
    }HwEncState;

    typedef struct {
        // base flow context
        MppCtx ctx;
        MppApi *mpi;
        RK_S32 chn;

        // global flow control flag
        RK_U32 frm_eos;
        RK_U32 pkt_eos;
        RK_U32 frm_pkt_cnt;
        RK_S32 frame_num;
        RK_S32 frame_count;
        RK_U64 stream_size;
        /* end of encoding flag when set quit the loop */
        volatile RK_U32 loop_end;

        // src and dst
        FILE *fp_input;
        FILE *fp_output;
        FILE *fp_verify;

        /* encoder config set */
        MppEncCfg       cfg;
        MppEncPrepCfg   prep_cfg;
        MppEncRcCfg     rc_cfg;
        MppEncCodecCfg  codec_cfg;
        MppEncSliceSplit split_cfg;

        // input / output
        MppBufferGroup buf_grp;
        MppBuffer frm_buf;
        MppBuffer pkt_buf;
        MppBuffer md_info;
        MppEncSeiMode sei_mode;
        MppEncHeaderMode header_mode;

        // paramter for resource malloc
        RK_U32 width;
        RK_U32 height;
        RK_U32 hor_stride;
        RK_U32 ver_stride;
        MppFrameFormat fmt;
        MppCodingType type;
        RK_S32 loop_times;

        // resources
        size_t header_size;
        size_t frame_size;
        size_t mdinfo_size;
        /* NOTE: packet buffer may overflow */
        size_t packet_size;

        RK_U32 split_mode;
        RK_U32 split_arg;
        RK_U32 split_out;

        RK_U32 user_data_enable;
        // rate control runtime parameter
        RK_S32 fps_in_flex;
        RK_S32 fps_in_den;
        RK_S32 fps_in_num;
        RK_S32 fps_out_flex;
        RK_S32 fps_out_den;
        RK_S32 fps_out_num;
        RK_S32 qp_init;
        RK_S32 bps;
        RK_S32 bps_max;
        RK_S32 bps_min;
        RK_S32 rc_mode;
        RK_S32 gop_mode;
        RK_S32 gop_len;
        RK_S32 vi_len;

        RK_S64 first_frm;
        RK_S64 first_pkt;
    } MpiEncData;

    MpiEncData *mEncData = nullptr;
    MppPollType timeout = MPP_POLL_BLOCK;
    MppBuffer mBuffer[4];
private:
    void init_ctx(int width,int height,int fmt);
    MPP_RET mpp_enc_cfg_setup();
    RK_S32 mpp_enc_with_default_stride(int width,MppFrameFormat fmt);
private:
    int mCurrBufIndex = -1;
    int buf_fd = 0;
    int buf_size = 0;
    void *vir_addr;
    const char *test_file = "/userdata/test.h264";
    const char *test_in_file = "/userdata/test.bin";
    FILE *outFile;
    FILE *inFile;
    HwEncState mEncState;
    std::shared_ptr<Muxer>mMuxer;
};

#endif // HWENCODER_H
