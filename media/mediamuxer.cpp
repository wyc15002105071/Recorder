#include "mediamuxer.h"
#include "log.h"

using namespace std;
#define MODULE_TAG "Muxer"

static double r2d(AVRational r)
{
    return r.den == 0 ? 0 : (double)r.num / (double)r.den;
}

MediaMuxer::MediaMuxer()
{
    memset(&mMediaCtx,0,sizeof(MediaMuxCtx));
}

MediaMuxer::~MediaMuxer()
{
    MediaMuxCtx *p = &mMediaCtx;
    if(p->file_ctx) {
        av_write_trailer(p->file_ctx);
        avio_closep(&p->file_ctx->pb);
        avformat_free_context(p->file_ctx);
    }

    if(p->push_ctx) {
        av_write_trailer(p->push_ctx);
        avio_closep(&p->push_ctx->pb);
        avformat_free_context(p->push_ctx);
    }
}

//void MediaMuxer::run()
//{
//    //    char suffix[10] = {0};
//    //    char file_name[100] = {0};
//    //    char file_path[100] = {0};
//    //    char time_str[100] = {0};
//    //    AVFormatContext *out_ctx;
//    //    AVOutputFormat *out_fmt;
//    //    int extra_data_size = 0;
//    //    void *extra_data = nullptr;
//    //    int ret;
//    //    int framerate;
//    //    int64_t duration = 0;
//    //    switch (mMuxCtx->file_type) {
//    //    case MUX_FILE_TYPE_MP4:
//    //        strcpy(suffix,"mp4");
//    //        break;
//    //    case MUX_FILE_TYPE_AVI:
//    //        strcpy(suffix,"avi");
//    //        break;
//    //    case MUX_FILE_TYPE_MKV:
//    //        strcpy(suffix,"mkv");
//    //        break;
//    //    default:
//    //        strcpy(suffix,"mp4");
//    //    }

//    //    getCurentTime(time_str,"%Y-%m-%d_%H-%M-%S");

//    //    sprintf(file_name,"%s/%s.%s",VIDEOS_SAVE_DIR,time_str,suffix);
//    //    avformat_alloc_output_context2(&out_ctx,nullptr,suffix,file_name);
//    //    out_fmt = out_ctx->oformat;

//    //    avio_open(&out_ctx->pb,file_name,AVIO_FLAG_WRITE);

//    //    AVStream *stream = avformat_new_stream(out_ctx, nullptr);
//    //    int video_stream=stream->index;
//    //    AVCodecParameters *codecpar = stream[video_stream].codecpar;
//    //    codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
//    //    if(mMuxCtx->video_type == VIDEO_ENCODE_TYPE_AVC)
//    //        codecpar->codec_id = AV_CODEC_ID_H264;
//    //    else if(mMuxCtx->video_type == VIDEO_ENCODE_TYPE_HEVC)
//    //        codecpar->codec_id = AV_CODEC_ID_HEVC;
//    //    codecpar->width = mMuxCtx->width;
//    //    codecpar->height = mMuxCtx->height;

//    //    framerate = mMuxCtx->framerate;

//    //    out_ctx->streams[video_stream]->time_base = {1*100,framerate*100};
//    //    out_ctx->streams[video_stream]->r_frame_rate = {framerate*100,1*100};
//    //    extra_data_size = mpp_packet_get_length(mMuxCtx->hdr_pkt);
//    //    extra_data = mpp_packet_get_data(mMuxCtx->hdr_pkt);

//    //    codecpar = out_ctx->streams[video_stream]->codecpar;
//    //    codecpar->extradata = (uint8_t*) av_mallocz(extra_data_size+AV_INPUT_BUFFER_PADDING_SIZE);
//    //    memcpy(codecpar->extradata, extra_data, extra_data_size);
//    //    codecpar->extradata_size = extra_data_size;

//    //    AVDictionary*dict=nullptr;
//    //    ret = avformat_write_header(out_ctx, nullptr);
//    //    RLOGD("Muxer Thread start");
//    //    while(!mThreadExit) {
//    //        mLock.lock();
//    //        if(mPackets.size() <= 0) {
//    //            mLock.unlock();
//    //            msleep(1);
//    //            continue;
//    //        }
//    //        MppPacket packet = mPackets.front();
//    //        int isIntra = 0;
//    //        MppMeta meta = mpp_packet_get_meta(packet);
//    //        mpp_meta_get_s32(meta, KEY_OUTPUT_INTRA, &isIntra);
//    //        void   *data = mpp_packet_get_data(packet);
//    //        size_t  len  = mpp_packet_get_length(packet);
//    //        uint64_t pts = mpp_packet_get_pts(packet);
//    //        uint64_t dts = mpp_packet_get_dts(packet);
//    //        RK_U32 eos = mpp_packet_get_eos(packet);
//    //        AVPacket *pkt = av_packet_alloc();
//    //        pkt->stream_index = video_stream;
//    //        pkt->data = (uint8_t *)data;
//    //        pkt->size = len;
//    //        int64_t calc_duration = (double)AV_TIME_BASE / av_q2d(out_ctx->streams[video_stream]->r_frame_rate);
//    //        pkt->pts = (double)(pts*calc_duration) / (double)(av_q2d(out_ctx->streams[video_stream]->time_base)*AV_TIME_BASE);
//    //        pkt->dts = pkt->pts;
//    //        pkt->duration = (double)calc_duration / (double)(av_q2d(out_ctx->streams[video_stream]->time_base)*AV_TIME_BASE);
//    //        if (isIntra)
//    //            pkt->flags |= AV_PKT_FLAG_KEY;
//    //        ret = av_interleaved_write_frame(out_ctx, pkt);

//    ////        {
//    ////            static FILE *fp = fopen("test_mux.h264","wb");
//    ////            fwrite(data,1,len,fp);
//    ////        }
//    //        av_packet_unref(pkt);
//    //        av_packet_free(&pkt);
//    //        mpp_packet_deinit(&packet);

//    //        mPackets.pop_front();
//    //        mLock.unlock();
//    //        if(eos)
//    //            break;
//    //    }
//    //    out_ctx->duration = duration;

//    //    av_write_trailer(out_ctx);
//    //    avio_closep(&out_ctx->pb);
//    //    avformat_free_context(out_ctx);
//}

int MediaMuxer::prepare(MediaMuxer::MediaInfo info)
{
    MediaMuxCtx *p = &mMediaCtx;
    int32_t framerate = 0;
    int32_t ret = 0;
    char time_str[100] = {0};
    char file_name[100] = {0};
    char suffix[100] = {0};
    memset(&p->info,0,sizeof(MediaMuxer::MediaInfo));
    p->info = info;

    switch (p->info.suffix_type) {
    case Suffix_MP4:
        strcpy(suffix,"mp4");
        break;
    case Suffix_AVI:
        strcpy(suffix,"avi");
        break;
    case Suffix_MKV:
        strcpy(suffix,"mkv");
        break;
    case Suffix_TS:
        strcpy(suffix,"mkv");
        break;
    default:
        strcpy(suffix,"mp4");
    }

    getCurentTime(time_str,"%Y-%m-%d_%H-%M-%S");

    sprintf(file_name,"%s/%s.%s",VIDEOS_SAVE_DIR,time_str,suffix);
    avformat_alloc_output_context2(&p->file_ctx,nullptr,suffix,file_name);
    p->output_fmt = p->file_ctx->oformat;

    avio_open(&p->file_ctx->pb,file_name,AVIO_FLAG_WRITE);

    AVStream *stream = avformat_new_stream(p->file_ctx, nullptr);
    int video_stream=stream->index;
    AVCodecParameters *codecpar = stream[video_stream].codecpar;
    codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
    switch(p->info.coding_type) {
    case Video_CodingType_AVC:
        codecpar->codec_id = AV_CODEC_ID_H264;
        break;
    case Video_CodingType_HEVC:
        codecpar->codec_id = AV_CODEC_ID_HEVC;
        break;
    case Video_CodingType_VP8:
        codecpar->codec_id = AV_CODEC_ID_VP8;
        break;
    case Video_CodingType_VP9:
        codecpar->codec_id = AV_CODEC_ID_VP9;
        break;
    default:
        codecpar->codec_id = AV_CODEC_ID_H264;
    }

    codecpar->width = p->info.width;
    codecpar->height = p->info.height;

    framerate = p->info.framerate;

    p->file_ctx->streams[video_stream]->time_base = {1*100,framerate*100};
    p->file_ctx->streams[video_stream]->r_frame_rate = {framerate*100,1*100};

    codecpar = p->file_ctx->streams[video_stream]->codecpar;
    codecpar->extradata = (uint8_t*) av_mallocz(p->info.extra_size+AV_INPUT_BUFFER_PADDING_SIZE);
    memcpy(codecpar->extradata,p->info.extra_data,p->info.extra_size);
    codecpar->extradata_size = p->info.extra_size;

    ret = avformat_write_header(p->file_ctx, nullptr);

    return ret;
}

int MediaMuxer::writeData(MediaMuxer::MediaPacket *media_pkt)
{
    MediaMuxCtx *p = &mMediaCtx;
    int isIntra = 0;
    int ret = 0;

    AVPacket *avpkt = av_packet_alloc();
    avpkt->stream_index = media_pkt->is_video?p->video_stream:p->audio_stream;
    avpkt->data = (uint8_t *)media_pkt->data;
    avpkt->size = media_pkt->size;
    int64_t calc_duration = (double)AV_TIME_BASE / av_q2d(p->file_ctx->streams[p->video_stream]->r_frame_rate);
    avpkt->pts = (double)(media_pkt->pts*calc_duration) / (double)(av_q2d(p->file_ctx->streams[p->video_stream]->time_base)*AV_TIME_BASE);
    avpkt->dts = avpkt->pts;
    avpkt->duration = (double)calc_duration / (double)(av_q2d(p->file_ctx->streams[p->video_stream]->time_base)*AV_TIME_BASE);
    if(media_pkt->flags & FLAG_END_OF_STREAM) {
        p->file_ctx->duration = avpkt->pts;
        ret = av_write_trailer(p->file_ctx);
        ret = avio_closep(&p->file_ctx->pb);
        avformat_free_context(p->file_ctx);
        RLOGD("mux eos");
        av_packet_unref(avpkt);
        memset(p,0,sizeof(MediaMuxCtx));
        return ret;
    }

    if (media_pkt->flags & FLAG_OUTPUT_INTRA) {
        avpkt->flags |= AV_PKT_FLAG_KEY;
    }

    ret = av_interleaved_write_frame(p->file_ctx, avpkt);
    av_packet_unref(avpkt);
    av_packet_free(&avpkt);
    return ret;
}

//void MediaMuxer::createMuxCtx(int width, int height, Video_CodingType video_type, int framerate,MppPacket hdr_pkt)
//{
//    //    mMuxCtx = shared_ptr<MuxContext>(new MuxContext);
//    //    mMuxCtx->width      = width;

//    //    mMuxCtx->height     = height;
//    //    mMuxCtx->video_type = video_type;
//    //    mMuxCtx->framerate  = framerate;

//    //    mpp_packet_copy_init(&mMuxCtx->hdr_pkt,hdr_pkt);
//    //    RLOGD("create mux ctx width:%d,height:%d,videotype:%d,framerate:%d",width,height,video_type,framerate);
//    //    mpp_packet_deinit(&hdr_pkt);

//}

//void MediaMuxer::sendPacket(MppPacket packet)
//{
//    //    mLock.lock();
//    //    if(!mMuxCtx->need_mux && !mMuxCtx->need_push) {
//    //        mLock.unlock();
//    //        return;
//    //    }

//    //    if(mPackets.size() < MAX_QUEUE_SIZE) {
//    //        mPackets.push_back(packet);
//    //    }
//    //    mLock.unlock();
//}

//void MediaMuxer::setMux(bool need_mux,MUX_FILE_TYPE file_type)
//{
//    //    mLock.lock();
//    //    mMuxCtx->need_mux = need_mux;
//    //    mMuxCtx->file_type = file_type;
//    //    mLock.unlock();
//}

//void MediaMuxer::setPush(bool need_push,MUX_PUSH_TYPE push_type)
//{
//    //    mLock.lock();
//    //    mMuxCtx->need_push = need_push;
//    //    mMuxCtx->push_type = push_type;
//    //    mLock.unlock();
//}

//bool MediaMuxer::startTask()
//{
//    mThreadExit = false;
//    start();

//    return true;
//}

//void MediaMuxer::stopTask()
//{
//    mLock.lock();
//    mThreadExit = true;
//    if(isRunning()) {
//        wait(QUIT_TIMEOUT);
//    }
//    RLOGD("MediaMuxer Thread finish");
//    mLock.unlock();
//}
