#include "ffmpeg_mediametadataretriever.h"
#include "log.h"
#include "ffmpeg_utils.h"

using namespace std;

FFmpegMediaMetadataRetriever::FFmpegMediaMetadataRetriever()
{
    state = nullptr;
    meta.size = 0;
    meta.pData = nullptr;
    last_d_size = 0;
}

FFmpegMediaMetadataRetriever::~FFmpegMediaMetadataRetriever()
{
//    if(meta.pData) {
//        delete[] meta.pData;
//        meta.pData = nullptr;
//    }
}

void FFmpegMediaMetadataRetriever::setDataSource(const char *path)
{
    char uri[strlen(path)];
    char *headers = NULL;

    strcpy(uri, path);

    setDataSource(uri,headers);
}

void FFmpegMediaMetadataRetriever::setDataSource(const char *srcUrl,const char *headers)
{
    unique_lock<mutex> l(mLock);
    freeMediaDataSource();
    set_data_source_uri(&state, srcUrl, headers);
}

Metadata FFmpegMediaMetadataRetriever::getFrameAtTime(long timeUs, int option)
{
    AVPacket *pkt;
    pkt = av_packet_alloc();

    get_frame_at_time(&state,timeUs, option, pkt);

    if(last_d_size < pkt->size)
        last_d_size = pkt->size * 1000;
    if(meta.pData == nullptr)
        meta.pData = shared_ptr<uint8_t>(new uint8_t[pkt->size]);

    memset(meta.pData.get(),0,pkt->size);
    meta.size = pkt->size;
//    meta.pData = pkt->data;
//    meta.data.reset(pkt->data);
    memcpy(meta.pData.get(),pkt->data,pkt->size);
    av_packet_unref(pkt);
    return meta;
}

void FFmpegMediaMetadataRetriever::freeMediaDataSource()
{

}

void FFmpegMediaMetadataRetriever::init(State **ps)
{
    State *state = *ps;

    if (state && state->pFormatCtx) {
        avformat_close_input(&state->pFormatCtx);
    }

//    if (state && state->fd != -1) {
//        close(state->fd);
//    }

    if (!state) {
        state = (State *)av_mallocz(sizeof(State));
    }

    state->pFormatCtx = NULL;
    state->audio_stream = -1;
    state->video_stream = -1;
    state->audio_st = NULL;
    state->video_st = NULL;
//    state->fd = -1;
    state->offset = 0;
    state->headers = NULL;
    state->callback_data_source = NULL;
    state->media_data_source_callback = NULL;
    state->media_data_source_seek_callback = NULL;
    state->position = 0;

    *ps = state;
}

void FFmpegMediaMetadataRetriever::decode_frame(State *state, AVPacket *pkt, int *got_frame, int64_t desired_frame_number, int width, int height)
{
    // Allocate video frame
    AVFrame *frame = av_frame_alloc();
    AVPacket *p = av_packet_alloc();
    int max_count = 5;
    int try_num = 0;
    *got_frame = 0;

    if (!frame) {
        return;
    }

    // Read frames and return the first one found
    while (av_read_frame(state->pFormatCtx, p) >= 0) {
        // Is this a packet from the video stream?
        if (p->stream_index == state->video_stream) {
            int codec_id = state->video_st->codecpar->codec_id;
            int pix_fmt = state->video_st->codecpar->format;

            // If the image isn't already in a supported format convert it to one
            if (!is_supported_format(codec_id, pix_fmt)) {
                *got_frame = 0;

                // Decode video frame
//                if (avcodec_decode_video2(state->video_st->codec, frame, got_frame, pkt) <= 0) {
//                    *got_frame = 0;
//                    break;
//                }
                int ret = avcodec_send_packet(state->codecCtx, p);
                if(ret != 0) {
                    char buf[1024] = {0};
                    av_strerror(ret,buf,sizeof(buf));
                    RLOGE("avcodec_send_packet err:%s",buf);
                }
                ret = avcodec_receive_frame(state->codecCtx, frame);
                if (ret != 0) {
                    *got_frame = 0;
                    if(try_num++ > max_count)
                        break;
                    continue;
                } else {
                    *got_frame = 1;
                }

                // Did we get a video frame?
                if (*got_frame) {
                    if (desired_frame_number == -1 ||
                            (desired_frame_number != -1 && frame->pts >= desired_frame_number)) {
                        if (p->data) {
                            av_packet_unref(p);
                        }
//                        pkt = av_packet_alloc();

                        convert_image(state, state->codecCtx, frame, pkt, got_frame, frame->width, frame->height);

                        break;
                    }
                }
            } else {
                *got_frame = 1;
                break;
            }
        }
    }

    // Free the frame
    av_frame_unref(frame);
}

int FFmpegMediaMetadataRetriever::set_data_source_uri(State **ps, const char *path, const char *headers)
{
    State *state = *ps;

    init(&state);

    state->headers = headers;

    *ps = state;

    return set_data_source_l(ps, path);
}

int FFmpegMediaMetadataRetriever::set_data_source_l(State **ps, const char *path)
{
    int audio_index = -1;
    int video_index = -1;
    int i;

    State *state = *ps;

    AVDictionary *options = nullptr;
    av_dict_set(&options, "icy", "1", 0);
    av_dict_set(&options, "user-agent", "FFmpegMediaMetadataRetriever", 0);

    if (state->headers) {
        av_dict_set(&options, "headers", state->headers, 0);
    }

    if (state->offset > 0) {
        state->pFormatCtx = avformat_alloc_context();
        state->pFormatCtx->skip_initial_bytes = state->offset;
    }

    if (state->media_data_source_callback) {
        int size = 64 * 1024;
        uint8_t *buffer = (uint8_t *)malloc(size);

        AVIOContext* avioCtx = avio_alloc_context(buffer,
                size,
                0,
                state,
                state->media_data_source_callback,
                nullptr,
                state->media_data_source_seek_callback);

        state->pFormatCtx = avformat_alloc_context();
        state->pFormatCtx->pb = avioCtx;
        state->pFormatCtx->flags = AVFMT_FLAG_CUSTOM_IO;
        state->position = 0;

        if (avformat_open_input(&state->pFormatCtx, NULL, NULL, &options) != 0) {
            RLOGE("Metadata could not be retrieved\n");
            *ps = nullptr;
            return FAILURE;
        }
    } else {
        if (avformat_open_input(&state->pFormatCtx, path, NULL, &options) != 0) {
            RLOGE("Metadata could not be retrieved\n");
            *ps = nullptr;
            return FAILURE;
        }
    }

    if (avformat_find_stream_info(state->pFormatCtx, NULL) < 0) {
        RLOGE("Metadata could not be retrieved\n");
        avformat_close_input(&state->pFormatCtx);
        *ps = nullptr;
        return FAILURE;
    }

    set_duration(state->pFormatCtx);

    set_shoutcast_metadata(state->pFormatCtx);

//    av_dump_format(state->pFormatCtx, 0, path, 0);

    // Find the first audio and video stream
    for (i = 0; i < state->pFormatCtx->nb_streams; i++) {
        if (state->pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && video_index < 0) {
            video_index = i;
            state->video_stream = video_index;
        }

        if (state->pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO && audio_index < 0) {
            audio_index = i;
            state->audio_stream = audio_index;
        }

        set_codec(state->pFormatCtx, i);
    }

    if (audio_index >= 0) {
//        stream_component_open(state, audio_index);
    }

    if (video_index >= 0) {
        stream_component_open(state, video_index);
    }
    RLOGD("Found metadata0\n");
    if(state->video_stream < 0 && state->audio_stream < 0) {
        RLOGD("v:%d,a:%d\n",state->video_stream,state->audio_stream);
        avformat_close_input(&state->pFormatCtx);
        *ps = NULL;
        return FAILURE;
    }
    RLOGD("Found metadata1\n");
    set_rotation(state->pFormatCtx, state->audio_st, state->video_st);
    set_framerate(state->pFormatCtx, state->audio_st, state->video_st);
    set_filesize(state->pFormatCtx);
    set_chapter_count(state->pFormatCtx);
    set_video_dimensions(state->pFormatCtx, state->video_st);

    RLOGD("Found metadata2\n");
    AVDictionaryEntry *tag = NULL;
    while ((tag = av_dict_get(state->pFormatCtx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
        RLOGD("Key %s: \n", tag->key);
        RLOGD("Value %s: \n", tag->value);
    }

    *ps = state;
    return SUCCESS;
}

int FFmpegMediaMetadataRetriever::stream_component_open(State *s, int stream_index)
{
    AVFormatContext *pFormatCtx = s->pFormatCtx;
    AVCodecContext *codecCtx = s->codecCtx;
    AVCodec *codec;

    if (stream_index < 0 || stream_index >= pFormatCtx->nb_streams) {
        return FAILURE;
    }

//    s->video_stream = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
//    s->audio_stream = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);

    // Get a pointer to the codec context for the stream
//    codecCtx = pFormatCtx->streams[stream_index]->codec;
    AVCodecID id = pFormatCtx->streams[s->video_stream]->codecpar->codec_id;
    codec = avcodec_find_decoder(id);

    s->codecCtx = avcodec_alloc_context3(codec);
    int ret = avcodec_parameters_to_context(s->codecCtx,pFormatCtx->streams[s->video_stream]->codecpar);
    if(ret < 0) {
        RLOGE("avcodec_parameters_to_context failed");
        return FAILURE;
    }

    const AVCodecDescriptor *codesc = avcodec_descriptor_get(s->codecCtx->codec_id);
    if (codesc) {
        RLOGD("avcodec_find_decoder %s\n", codesc->name);
    }

    // Find the decoder for the audio stream
//    codec = avcodec_find_decoder(s->codecCtx->codec_id);

//    if (codec == NULL) {
//        printf("avcodec_find_decoder() failed to find audio decoder\n");
//        return FAILURE;
//    }

    // Open the codec
    if (!codec || (avcodec_open2(s->codecCtx, codec, NULL) < 0)) {
        RLOGD("avcodec_open2() failed\n");
        return FAILURE;
    }

    switch(s->codecCtx->codec_type) {
        case AVMEDIA_TYPE_AUDIO:
            s->audio_stream = stream_index;
            s->audio_st = pFormatCtx->streams[stream_index];
            break;
        case AVMEDIA_TYPE_VIDEO:
        {
            s->video_stream = stream_index;
            s->video_st = pFormatCtx->streams[stream_index];
            av_opt_set(s->codecCtx->priv_data, "tune", "zerolatency", 0);
//            AVCodec *targetCodec = avcodec_find_encoder((AVCodecID)TARGET_IMAGE_CODEC);
//            if (!targetCodec) {
//                printf("avcodec_find_decoder() failed to find encoder\n");
//                return FAILURE;
//            }

//            s->codecCtx = avcodec_alloc_context3(targetCodec);
//            if (!s->codecCtx) {
//                printf("avcodec_alloc_context3 failed\n");
//                return FAILURE;
//            }

//            s->codecCtx->bit_rate = s->video_st->codecpar->bit_rate;
//            s->codecCtx->width = s->video_st->codecpar->width;
//            s->codecCtx->height = s->video_st->codecpar->height;
//            s->codecCtx->pix_fmt = (AVPixelFormat)TARGET_IMAGE_FORMAT;
//            s->codecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
//            s->codecCtx->time_base.num = s->video_st->time_base.num;
//            s->codecCtx->time_base.den = s->video_st->time_base.den;

//            if (!targetCodec || avcodec_open2(s->codecCtx, targetCodec, NULL) < 0) {
//                printf("avcodec_open2() failed\n");
//                return FAILURE;
//            }

            s->sws_ctx = sws_getContext(s->video_st->codecpar->width,
                    s->video_st->codecpar->height,
                    (AVPixelFormat)s->video_st->codecpar->format,
                    s->video_st->codecpar->width,
                    s->video_st->codecpar->height,
                    (AVPixelFormat)TARGET_IMAGE_FORMAT,
                    SWS_BILINEAR,
                    NULL,
                    NULL,
                    NULL);
            break;
        }
        default:
            break;
    }

    RLOGD("stream_component_open success!");
    return SUCCESS;
}

int FFmpegMediaMetadataRetriever::get_frame_at_time(State **ps, int64_t timeUs, int option, AVPacket *pkt)
{
    unique_lock<mutex> l(mLock);
    get_scaled_frame_at_time(ps,timeUs,option,pkt,-1,-1);
}

int FFmpegMediaMetadataRetriever::get_scaled_frame_at_time(State **ps, int64_t timeUs, int option, AVPacket *pkt, int width, int height)
{
    int got_packet = 0;
    int64_t desired_frame_number = -1;

    State *state = *ps;

    Options opt = (Options)option;

    if (!state || !state->pFormatCtx || state->video_stream < 0) {
        return FAILURE;
    }

    if (timeUs > -1) {
        int stream_index = state->video_stream;
        int64_t seek_time = av_rescale_q(timeUs, AV_TIME_BASE_Q, state->pFormatCtx->streams[stream_index]->time_base);
        int64_t seek_stream_duration = state->pFormatCtx->streams[stream_index]->duration;

        AVRational r = state->pFormatCtx->streams[stream_index]->time_base;

        int flags = 0;
        int ret = -1;

        // For some reason the seek_stream_duration is sometimes a negative value,
        // make sure to check that it is greater than 0 before adjusting the
        // seek_time
        if (seek_stream_duration > 0 && seek_time > seek_stream_duration) {
            seek_time = seek_stream_duration;
        }

        if (seek_time < 0) {
            return FAILURE;
        }

        if (opt == OPTION_CLOSEST) {
            desired_frame_number = seek_time;
            flags = AVSEEK_FLAG_BACKWARD;
        } else if (opt == OPTION_CLOSEST_SYNC) {
            flags = 0;
        } else if (opt == OPTION_NEXT_SYNC) {
            flags = 0;
        } else if (opt == OPTION_PREVIOUS_SYNC) {
            flags = AVSEEK_FLAG_BACKWARD;
        }

        ret = av_seek_frame(state->pFormatCtx, stream_index, seek_time, flags);

        if (ret < 0) {
            RLOGE("av_seek_frame failed!\n");
            return FAILURE;
        } else {
            avcodec_flush_buffers(state->codecCtx);
        }
    }

    decode_frame(state, pkt, &got_packet, desired_frame_number, width, height);

//    if (got_packet) {
//        const char *filename = "./one.png";
//        FILE *picture = fopen(filename, "wb+");
//        fwrite(pkt->data, pkt->size, 1, picture);
//        fclose(picture);
//    }

    if (got_packet) {
        return SUCCESS;
    } else {
        return FAILURE;
    }
}

int FFmpegMediaMetadataRetriever::is_supported_format(int codec_id, int pix_fmt)
{
    if ((codec_id == AV_CODEC_ID_PNG ||
         codec_id == AV_CODEC_ID_MJPEG ||
         codec_id == AV_CODEC_ID_BMP) &&
        pix_fmt == AV_PIX_FMT_RGBA) {
        return 1;
    }

    return 0;
}

void FFmpegMediaMetadataRetriever::convert_image(State *state, AVCodecContext *pCodecCtx, AVFrame *pFrame, AVPacket *avpkt, int *got_packet_ptr, int width, int height)
{
    AVCodecContext *codecCtx;
    struct SwsContext *scalerCtx;
    AVFrame *frame;

    *got_packet_ptr = 0;
//    qDebug()<<"width:"<<width;
//    qDebug()<<"height:"<<height;
    if (width != -1 && height != -1) {
        if (state->scaled_codecCtx == NULL ||
                state->scaled_sws_ctx == NULL) {
            get_scaled_context(state, pCodecCtx, width, height);
        }

        codecCtx = state->scaled_codecCtx;
        scalerCtx = state->scaled_sws_ctx;
    } else {
        codecCtx = state->codecCtx;
        scalerCtx = state->sws_ctx;
    }

    if (width == -1) {
        width = pCodecCtx->width;
    }

    if (height == -1) {
        height = pCodecCtx->height;
    }

    frame = av_frame_alloc();

    // Determine required buffer size and allocate buffer
    int numBytes = av_image_get_buffer_size((AVPixelFormat)TARGET_IMAGE_FORMAT, codecCtx->width, codecCtx->height,1);
    //int numBytes = avpicture_get_size((AVPixelFormat)TARGET_IMAGE_FORMAT, codecCtx->width, codecCtx->height);
    void * buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));

    // set the frame parameters
    frame->format = TARGET_IMAGE_FORMAT;
    frame->width = codecCtx->width;
    frame->height = codecCtx->height;

    av_image_fill_arrays(frame->data,frame->linesize,(const uint8_t *)buffer,
                         (AVPixelFormat)TARGET_IMAGE_FORMAT,
                         codecCtx->width,
                         codecCtx->height,
                         1);

//    avpicture_fill(((AVPicture *)frame),
//                   (const uint8_t *)buffer,
//                   (AVPixelFormat)TARGET_IMAGE_FORMAT,
//                   codecCtx->width,
//                   codecCtx->height);


    sws_scale(scalerCtx,
              (const uint8_t * const *) pFrame->data,
              pFrame->linesize,
              0,
              pFrame->height,
              frame->data,
              frame->linesize);

//    int ret = avcodec_encode_video2(codecCtx, avpkt, frame, got_packet_ptr);
    int ret = avcodec_send_frame(codecCtx, frame);
    ret = avcodec_receive_packet(codecCtx,avpkt);
    if (ret != 0) {
        *got_packet_ptr = 0;
    } else {
        *got_packet_ptr = 1;
    }

    av_frame_free(&frame);

    if (buffer) {
        av_free(buffer);
    }

    if (ret < 0 || !*got_packet_ptr) {
        av_packet_unref(avpkt);
    }
}

int FFmpegMediaMetadataRetriever::get_scaled_context(State *s, AVCodecContext *pCodecCtx, int width, int height)
{
    AVCodec *targetCodec = avcodec_find_encoder((AVCodecID)TARGET_IMAGE_CODEC);
    if (!targetCodec) {
        RLOGE("avcodec_find_decoder() failed to find encoder\n");
        return FAILURE;
    }

    s->scaled_codecCtx = avcodec_alloc_context3(targetCodec);
    if (!s->scaled_codecCtx) {
        RLOGE("avcodec_alloc_context3 failed\n");
        return FAILURE;
    }

    s->scaled_codecCtx->bit_rate = s->video_st->codecpar->bit_rate;
    s->scaled_codecCtx->width = width;
    s->scaled_codecCtx->height = height;
    s->scaled_codecCtx->pix_fmt = (AVPixelFormat)TARGET_IMAGE_FORMAT;
    s->scaled_codecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    s->scaled_codecCtx->time_base.num = s->video_st->time_base.num;
    s->scaled_codecCtx->time_base.den = s->video_st->time_base.den;

    if (!targetCodec || avcodec_open2(s->scaled_codecCtx, targetCodec, NULL) < 0) {
        RLOGE("avcodec_open2() failed\n");
        return FAILURE;
    }

    s->scaled_sws_ctx = sws_getContext(s->video_st->codecpar->width,
            s->video_st->codecpar->height,
            (AVPixelFormat)s->video_st->codecpar->format,
            width,
            height,
            (AVPixelFormat)TARGET_IMAGE_FORMAT,
            SWS_BILINEAR,
            0,
            0,
            0);

    return SUCCESS;
}

