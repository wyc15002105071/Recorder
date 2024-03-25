#include "thumbnailutils.h"
#include "utils/fileutils.h"

ThumbnailUtils::ThumbnailUtils() :
    mThumbSize(QSize(0,0))
   ,mFormatCtx(nullptr)
{

}

ThumbnailUtils::~ThumbnailUtils()
{
    mDecoder.release();
    if(mFormatCtx) {
        avformat_close_input(&mFormatCtx);
        avformat_free_context(mFormatCtx);
        mFormatCtx = nullptr;
    }
}

void ThumbnailUtils::run()
{
    int count = mFileslist.count();
    if(count == 0) {
        return;
    }
    int i = 0;
    while(!mThreadExit) {
        for(i = 0;i < count; i++) {
            QString file_path = mFileslist.at(i);
            const char *suffix = FileUtils::getFileSuffix(file_path);
            Type type = getType(suffix);
            if(type == Type_Unkown) {
                usleep(1*1000);
                continue;
            }

            getThumbnail(file_path,type);
            usleep(1*1000);

            if(mThreadExit)
                break;
        }
        if(i >= count)
            break;
    }
}

void ThumbnailUtils::setDataSource(QList<QString> files)
{
    this->mFileslist = files;
}

void ThumbnailUtils::setThumbSize(int width, int height)
{
    RLOGD("thumb size is [%d:%d]",width,height);
    mThumbSize.setWidth(width < 0?0:width);
    mThumbSize.setHeight(height < 0?0:height);
}

ThumbnailUtils::Type ThumbnailUtils::getType(const char *suffix)
{
    if(!suffix)
        return Type_Unkown;
    if(!strcmp(suffix,"mp4")
            || !strcmp(suffix,"mkv")
            || !strcmp(suffix,"avi")
            || !strcmp(suffix,"ts")) {
        return Type_Video;
    } else if(!strcmp(suffix,"jpeg")
              || !strcmp(suffix,"jpg")
              || !strcmp(suffix,"png")) {
        return Type_Image;
    } else
        return Type_Unkown;
}

void ThumbnailUtils::getThumbnail(QString file_path, ThumbnailUtils::Type type)
{
    QPixmap thumbnail;
    int video_index = 0;
    int ret = 0;
    QImage img;
    if(type == Type_Image) {
        img = QImage(file_path);
    } else if(type == Type_Video) {
        AVPacket packet;
        uint8_t *extra_data;
        int width,height,size,extra_sz = 0;
        int64_t pts,dts = 0;
        AVCodecID codecid = AV_CODEC_ID_NONE;
        RKHWDecApi::DecCfgInfo info;
        RKHWDecApi::OutputFrame out_frame;
        const char *file_p = file_path.toLatin1().data();
        RLOGD("file path is %s",file_p);
        if (avformat_open_input(&mFormatCtx, file_p, NULL, NULL) != 0) {
            RLOGE("Failed to open input file:%s",file_p);
            goto FINISH;
        }

        if (avformat_find_stream_info(mFormatCtx, NULL) < 0) {
            RLOGE("Failed to retrieve stream information:%s",file_p);
            goto FINISH;
        }

        // 查找视频流
        for (uint32_t n = 0; n < mFormatCtx->nb_streams; n++) {
            if (mFormatCtx->streams[n]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
                video_index = n;
                break;
            }
        }

        if (video_index == -1) {
            RLOGE("Failed to find video stream:%s",file_p);
            goto FINISH;
        }

REDO:
        ret = av_read_frame(mFormatCtx, &packet);
        if(ret) {
            RLOGE("read video frame error,skip...[%s]]",file_p);
            av_packet_unref(&packet);
            goto FINISH;
        }
        if(packet.stream_index != video_index) {
            goto REDO;
        }
        extra_data = mFormatCtx->streams[video_index]->codecpar->extradata;
        extra_sz   = mFormatCtx->streams[video_index]->codecpar->extradata_size;

        width   = mFormatCtx->streams[video_index]->codecpar->width;
        height  = mFormatCtx->streams[video_index]->codecpar->height;
        size    = packet.size;
        pts     = packet.pts;
        dts     = packet.dts;
        codecid = mFormatCtx->streams[video_index]->codecpar->codec_id;
        memset(&info,0,sizeof (RKHWDecApi::DecCfgInfo));
        info.width  = width;
        info.height = height;
        info.type = mDecoder.formatFF2Mpp(codecid);
        info.thumbnail = true;
        mDecoder.prepare(info);
        ret = mDecoder.sendPacket(extra_data,extra_sz,0,0,RKHWDecApi::FLAG_CODEC_CONFIG);
        ret = mDecoder.sendPacket(packet.data,size,pts,dts,0);

        memset(&out_frame,0,sizeof(RKHWDecApi::OutputFrame));
        ret = mDecoder.getOutputFrame(&out_frame);
        if(ret == MPP_OK) {
            RKHWDecApi::OutputFrame rgb_frame;
            mDecoder.convertNV12ToRGB(&out_frame,&rgb_frame);
            img = QImage((uchar *)rgb_frame.vir_addr,rgb_frame.width,rgb_frame.height,QImage::Format_RGBA8888);
            mDecoder.deinitOutputFrame(&out_frame);
            mDecoder.deinitOutputFrame(&rgb_frame);
        }
        av_packet_unref(&packet);

    } else if(type == Type_Unkown) {
        goto FINISH;
    }

FINISH:
    if(mThumbSize.width() > 0 && mThumbSize.height() > 0) {
        img = img.scaled(mThumbSize,Qt::KeepAspectRatio);
    }else
        img = img.scaled(img.size(),Qt::KeepAspectRatio);
    
    onGetOneImage(img,file_path);
    if(type == Type_Video) {
        mDecoder.release();
        if(mFormatCtx) {
            avformat_close_input(&mFormatCtx);
            avformat_free_context(mFormatCtx);
            mFormatCtx = nullptr;
        }
    }

    img.detach();
}
