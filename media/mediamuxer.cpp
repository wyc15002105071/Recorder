#include "muxer.h"
#include "log.h"

using namespace std;
#define MODULE_TAG "Muxer"

static double r2d(AVRational r)
{
    return r.den == 0 ? 0 : (double)r.num / (double)r.den;
}

Muxer::Muxer():
    mMuxCtx(nullptr)
{

}

void Muxer::run()
{
    char suffix[10] = {0};
    char file_name[100] = {0};
    char file_path[100] = {0};
    char time_str[100] = {0};
    AVFormatContext *out_ctx;
    AVOutputFormat *out_fmt;
    int extra_data_size = 0;
    void *extra_data = nullptr;
    int ret;
    int framerate;
    int64_t duration = 0;
    switch (mMuxCtx->file_type) {
    case MUX_FILE_TYPE_MP4:
        strcpy(suffix,"mp4");
        break;
    case MUX_FILE_TYPE_AVI:
        strcpy(suffix,"avi");
        break;
    case MUX_FILE_TYPE_MKV:
        strcpy(suffix,"mkv");
        break;
    default:
        strcpy(suffix,"mp4");
    }

    getCurentTime(time_str,NULL);

    sprintf(file_name,"%s/%s.%s",VIDEOS_SAVE_DIR,time_str,suffix);
    avformat_alloc_output_context2(&out_ctx,nullptr,suffix,file_name);
    out_fmt = out_ctx->oformat;

    avio_open(&out_ctx->pb,file_name,AVIO_FLAG_WRITE);

    AVStream *stream = avformat_new_stream(out_ctx, nullptr);
    int video_stream=stream->index;
    RLOGD("video stream is %d",video_stream);
    AVCodecParameters *codecpar = stream[video_stream].codecpar;
    codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
    if(mMuxCtx->video_type == VIDEO_ENCODE_TYPE_AVC)
        codecpar->codec_id = AV_CODEC_ID_H264;
    else if(mMuxCtx->video_type == VIDEO_ENCODE_TYPE_HEVC)
        codecpar->codec_id = AV_CODEC_ID_HEVC;
    codecpar->width = mMuxCtx->width;
    codecpar->height = mMuxCtx->height;

    framerate = mMuxCtx->framerate;

    out_ctx->streams[video_stream]->time_base = {1*100,framerate*100};
    out_ctx->streams[video_stream]->r_frame_rate = {framerate*100,1*100};
    extra_data_size = mpp_packet_get_length(mMuxCtx->hdr_pkt);
    extra_data = mpp_packet_get_data(mMuxCtx->hdr_pkt);

    codecpar = out_ctx->streams[video_stream]->codecpar;
    codecpar->extradata = (uint8_t*) av_mallocz(extra_data_size+AV_INPUT_BUFFER_PADDING_SIZE);
    memcpy(codecpar->extradata, extra_data, extra_data_size);
    codecpar->extradata_size = extra_data_size;

    AVDictionary*dict=nullptr;
    ret = avformat_write_header(out_ctx, nullptr);
    RLOGD("framerate is %d",framerate);
    while(!mThreadExit) {
        mLock.lock();
        if(mPackets.size() <= 0) {
            mLock.unlock();
            msleep(1);
            continue;
        }
        MppPacket packet = mPackets.front();
        int isIntra = 0;
        MppMeta meta = mpp_packet_get_meta(packet);
        mpp_meta_get_s32(meta, KEY_OUTPUT_INTRA, &isIntra);
        void   *data = mpp_packet_get_data(packet);
        size_t  len  = mpp_packet_get_length(packet);
        uint64_t pts = mpp_packet_get_pts(packet);
        uint64_t dts = mpp_packet_get_dts(packet);
        RK_U32 eos = mpp_packet_get_eos(packet);
        AVPacket *pkt = av_packet_alloc();
        pkt->stream_index = video_stream;
        pkt->data = (uint8_t *)data;
        pkt->size = len;

        int64_t calc_duration = (double)AV_TIME_BASE / av_q2d(out_ctx->streams[video_stream]->r_frame_rate);
        pkt->pts = (double)(pts*calc_duration) / (double)(av_q2d(out_ctx->streams[video_stream]->time_base)*AV_TIME_BASE);
        pkt->dts = pkt->pts;
        pkt->duration = (double)calc_duration / (double)(av_q2d(out_ctx->streams[video_stream]->time_base)*AV_TIME_BASE);
        if (isIntra)
            pkt->flags |= AV_PKT_FLAG_KEY;
        ret = av_interleaved_write_frame(out_ctx, pkt);

//        {
//            static FILE *fp = fopen("test_mux.h264","wb");
//            fwrite(data,1,len,fp);
//        }
        av_packet_unref(pkt);
        av_packet_free(&pkt);

        mPackets.pop_front();
        mLock.unlock();
        mpp_packet_deinit(&packet);

        if(eos)
            break;
    }
    out_ctx->duration = duration;

    av_write_trailer(out_ctx);
    avio_closep(&out_ctx->pb);
    avformat_free_context(out_ctx);

    for(int i = 0;i<mPackets.size();i++) {
        MppPacket packet = mPackets.front();

        mPackets.pop_front();
        mpp_packet_deinit(&packet);
    }

    if(mMuxCtx->hdr_pkt) {
        mpp_packet_deinit(&mMuxCtx->hdr_pkt);
    }
}

void Muxer::createMuxCtx(int width, int height, VIDEO_ENCODE_TYPE video_type, int framerate,MppPacket hdr_pkt)
{
    mMuxCtx = shared_ptr<MuxContext>(new MuxContext);
    mMuxCtx->width      = width;

    mMuxCtx->height     = height;
    mMuxCtx->video_type = video_type;
    mMuxCtx->framerate  = framerate;

    mpp_packet_copy_init(&mMuxCtx->hdr_pkt,hdr_pkt);
    RLOGD("create mux ctx width:%d,height:%d,videotype:%d,framerate:%d",width,height,video_type,framerate);

}

void Muxer::sendPacket(MppPacket packet)
{
    mLock.lock();
    if(!mMuxCtx->need_mux && !mMuxCtx->need_push) {
        mLock.unlock();
        return;
    }

    if(mPackets.size() < MAX_QUEUE_SIZE) {
        mPackets.push_back(packet);
    }
    mLock.unlock();
}

void Muxer::setMux(bool need_mux,MUX_FILE_TYPE file_type)
{
    mLock.lock();
    mMuxCtx->need_mux = need_mux;
    mMuxCtx->file_type = file_type;
    mLock.unlock();
}

void Muxer::setPush(bool need_push,MUX_PUSH_TYPE push_type)
{
    mLock.lock();
    mMuxCtx->need_push = need_push;
    mMuxCtx->push_type = push_type;
    mLock.unlock();
}

bool Muxer::startTask()
{
    mThreadExit = false;
    start();

    return true;
}

void Muxer::stopTask()
{
    mThreadExit = true;
    if(isRunning()) {
        quit();
        wait(QUIT_TIMEOUT);
    }
    for(int i = 0;i<mPackets.size();i++) {
        MppPacket packet = mPackets.front();

        mPackets.pop_front();
        mpp_packet_deinit(&packet);
    }

    if(mMuxCtx->hdr_pkt) {
        mpp_packet_deinit(&mMuxCtx->hdr_pkt);
    }
}
