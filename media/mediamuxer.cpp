#include "mediamuxer.h"
#include "log.h"
#include "utils/networkutils.h"

using namespace std;
#define MODULE_TAG "Muxer"

#define AV_PKT_FLAG_EOS (1 << 10)

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
    if(p->format_ctx) {
        av_write_trailer(p->format_ctx);
        avio_closep(&p->format_ctx->pb);
        avformat_free_context(p->format_ctx);
    }
    avformat_network_deinit();
}

void MediaMuxer::stopTask()
{
    mLock.lock();

    mLock.unlock();
    RThread::stopTask();
}

void MediaMuxer::run()
{
    MediaMuxCtx *p = &mMediaCtx;
    int ret = 0;

    RLOGD("mux thread start,is exit:%d",mThreadExit);
    while(!mThreadExit) {
	bool eos = false;
	mLock.lock();	
	if(mPackets.size() <= 0) {
	    mLock.unlock();
	    usleep(1*1000);
	    continue;
	}

	AVPacket *pkt = mPackets.front();
	if(!p->format_ctx || !pkt || !pkt->data || pkt->size <= 0) {
	    mPackets.pop_front();
	    mLock.unlock();
	    usleep(1*1000);
	    continue;
	}

	//AVPacket *dst_pkt;
	//av_copy_packet(dst_pkt, pkt);
        //av_packet_ref(dst_pkt,pkt);

	mPackets.pop_front();
	mLock.unlock();
	
	if(pkt->flags & AV_PKT_FLAG_EOS) {
	    RLOGD("recieve eos");
	    eos = true;	
	}
        p->format_ctx->duration = pkt->pts;
//	struct timespec start, end;
//	long long int diff;
//	clock_gettime(CLOCK_MONOTONIC, &start);
	ret = av_interleaved_write_frame(p->format_ctx, pkt);
//	clock_gettime(CLOCK_MONOTONIC, &end);
//	diff = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
//	double seconds = diff / 1e9;
//	double milliseconds = diff / 1e6;

//	RLOGD("list size %d write const time %.9f seconds %.3f milliseconds",mPackets.size(),seconds,milliseconds);

    	av_packet_unref(pkt);
    	av_packet_free(&pkt);


	if(eos) break;
    	usleep(1*1000);
    }

    if(p->format_ctx) {
    	ret = av_write_trailer(p->format_ctx);
    	ret = avio_closep(&p->format_ctx->pb);
    	avformat_free_context(p->format_ctx);
    }
 
    while (mPackets.size() > 0) {
    	AVPacket *pkt = mPackets.front();
	if(pkt) {
	    av_packet_unref(pkt);
            av_packet_free(&pkt);
	}
	mPackets.pop_front();
    }

    mPackets.clear();
    RLOGD("mux eos");
    memset(p,0,sizeof(MediaMuxCtx));
}

int MediaMuxer::prepare(MediaMuxer::MediaInfo info)
{
    MediaMuxCtx *p = &mMediaCtx;
    int32_t framerate = 0;
    int32_t ret = 0;

    char url[100] = {0};

    RLOGD("mux prepare start");
    memset(&p->info,0,sizeof(MediaMuxer::MediaInfo));
    p->info = info;

    if(p->info.push_stream) {
        char header[100] = {0};
        char ip_addr[100] = {0};
        char end[100] = "live";

        avformat_network_init();
        switch(p->info.push_type) {
        case Push_RTSP:
            strcpy(header,"rtsp");
            break;
        case Push_HTTP:
            strcpy(header,"https");
            break;
        case Push_RTMP:
            strcpy(header,"rtmp");
            break;
        default:
            strcpy(header,"rtsp");
            break;
        }
        // rtsp://192.168.26.31/live;
        strcpy(ip_addr,NetworkUtils::getOwnIpaddr());
        RLOGD("IP Address %s",ip_addr);
        sprintf(url,"%s://%s/%s",header,ip_addr,end);
        RLOGD("push stream url is %s",url);
        ret = avformat_alloc_output_context2(&p->format_ctx,nullptr,header,url);
        if(ret < 0) {
            char err[1024] = {0};
            av_strerror(ret,err,sizeof(err)-1);
            RLOGE("format ctx alloc failed!:%s,(%s)",url,err);
            return ret;
        }
    } else {
        char time_str[100] = {0};
        const char *suffix = getSuffix(info.suffix_type);

        if(info.file_path.length() == 0 || !info.file_path.c_str()) {
            getCurentTime(time_str,"%Y-%m-%d_%H-%M-%S");
            sprintf(url,"%s/%s.%s",VIDEOS_SAVE_DIR,time_str,suffix);
        } else {
            strcpy(url,info.file_path.c_str());
        }

        ret = avformat_alloc_output_context2(&p->format_ctx,nullptr,suffix,url);
        if(ret < 0) {
            char err[1024] = {0};
            av_strerror(ret,err,sizeof(err)-1);
            RLOGE("format ctx alloc failed!:%s,(%s)",url,err);
            return ret;
        }
    }

    p->output_fmt = p->format_ctx->oformat;

    ret = avio_open(&p->format_ctx->pb,url,AVIO_FLAG_WRITE);
    if(ret < 0) {
        char err[1024] = {0};
        av_strerror(ret,err,sizeof(err)-1);
        RLOGE("avio_open failed!:%s,(%s)",url,err);
        return ret;
    }

    AVStream *stream = avformat_new_stream(p->format_ctx, nullptr);
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

    p->format_ctx->streams[video_stream]->time_base = {1*100,framerate*100};
    p->format_ctx->streams[video_stream]->r_frame_rate = {framerate*100,1*100};

    codecpar = p->format_ctx->streams[video_stream]->codecpar;
    codecpar->extradata = (uint8_t*) av_mallocz(p->info.extra_size+AV_INPUT_BUFFER_PADDING_SIZE);
    memcpy(codecpar->extradata,p->info.extra_data,p->info.extra_size);
    codecpar->extradata_size = p->info.extra_size;

    ret = avformat_write_header(p->format_ctx, nullptr);
    if(ret < 0) {
        RLOGE("avformat_write_header failed");
    }
    return ret;
}

int MediaMuxer::writeData(MediaMuxer::MediaPacket *media_pkt)
{
    MediaMuxCtx *p = &mMediaCtx;
    int ret = 0;

    mLock.lock();
    if(!p->format_ctx) {
    	mLock.unlock();
	return -1;
    }

    uint8_t *data = (uint8_t *)av_malloc(media_pkt->size);
    AVPacket *avpkt = av_packet_alloc();
//    AVPacket *avpkt;
//    av_new_packet(avpkt,media_pkt->size);
    memcpy(data,media_pkt->data,media_pkt->size);
    av_packet_from_data(avpkt,data,media_pkt->size);
    avpkt->stream_index = media_pkt->is_video?p->video_stream:p->audio_stream;
//    avpkt->data = (uint8_t *)media_pkt->data;
//    memcpy(avpkt->data,media_pkt->data,media_pkt->size);
//    avpkt->size = media_pkt->size;
    int64_t calc_duration = (double)AV_TIME_BASE / av_q2d(p->format_ctx->streams[p->video_stream]->r_frame_rate);
    avpkt->pts = (double)(media_pkt->pts*calc_duration) / (double)(av_q2d(p->format_ctx->streams[p->video_stream]->time_base)*AV_TIME_BASE);
    avpkt->dts = avpkt->pts;
    avpkt->duration = (double)calc_duration / (double)(av_q2d(p->format_ctx->streams[p->video_stream]->time_base)*AV_TIME_BASE);
    if(media_pkt->flags & FLAG_END_OF_STREAM) {
/*        p->format_ctx->duration = avpkt->pts;
        ret = av_write_trailer(p->format_ctx);
        ret = avio_closep(&p->format_ctx->pb);
        avformat_free_context(p->format_ctx);
        RLOGD("mux eos");
        av_packet_unref(avpkt);
        memset(p,0,sizeof(MediaMuxCtx));
        return ret;*/
	avpkt->flags |= AV_PKT_FLAG_EOS;
    }


    if (media_pkt->flags & FLAG_OUTPUT_INTRA) {
        avpkt->flags |= AV_PKT_FLAG_KEY;
    }

    if(mPackets.size() < MAX_QUEUE_SIZE) {
    	mPackets.push_back(avpkt);
    }
    mLock.unlock();
//    ret = av_interleaved_write_frame(p->format_ctx, avpkt);
//    av_packet_unref(avpkt);
//    av_packet_free(&avpkt);
    return ret;
}
