#include "videoframereader.h"
#include <QVideoSurfaceFormat>
#include <QDebug>
#include <QThread>
#include "log.h"

#define MODULE_TAG "VideoFrameReader"

using namespace std;

VideoSurface::VideoSurface(QObject *parent):QAbstractVideoSurface (parent)
{
}

VideoSurface::~VideoSurface()
{
}

QList<QVideoFrame::PixelFormat> VideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    QList<QVideoFrame::PixelFormat> listPixelFormats;

    listPixelFormats.append(QVideoFrame::Format_ARGB32);
    listPixelFormats.append(QVideoFrame::Format_ARGB32_Premultiplied);
    listPixelFormats.append(QVideoFrame::Format_RGB32);
    listPixelFormats.append(QVideoFrame::Format_RGB24);
    listPixelFormats.append(QVideoFrame::Format_RGB565);
    listPixelFormats.append(QVideoFrame::Format_RGB555);
    listPixelFormats.append(QVideoFrame::Format_ARGB8565_Premultiplied);
    listPixelFormats.append(QVideoFrame::Format_BGRA32);
    listPixelFormats.append(QVideoFrame::Format_BGRA32_Premultiplied);
    listPixelFormats.append(QVideoFrame::Format_BGR32);
    listPixelFormats.append(QVideoFrame::Format_BGR24);
    listPixelFormats.append(QVideoFrame::Format_BGR565);
    listPixelFormats.append(QVideoFrame::Format_BGR555);
    listPixelFormats.append(QVideoFrame::Format_BGRA5658_Premultiplied);

    listPixelFormats.append(QVideoFrame::Format_Jpeg);

    listPixelFormats.append(QVideoFrame::Format_CameraRaw);
    listPixelFormats.append(QVideoFrame::Format_AdobeDng);

    return listPixelFormats;
}

bool VideoSurface::present(const QVideoFrame &frame)
{
    if (this->surfaceFormat().pixelFormat() != frame.pixelFormat() || this->surfaceFormat().frameSize() != frame.size())
    {
        setError(IncorrectFormatError);
        stop();
        RLOGE("error format");
        return false;
    }
    QVideoFrame cloneFrame(frame);
    emit frameAvailable(cloneFrame);
    return true;
}

VideoFrameReader::VideoFrameReader()
{
    qRegisterMetaType<QVideoFrame>("QVideoFrame&");
    mp_  = new QMediaPlayer;
    vs_ = new VideoSurface();
    connect(vs_, SIGNAL(frameAvailable(QVideoFrame &)), this, SLOT(ProcessFrame(QVideoFrame &)), Qt::QueuedConnection);

}

void VideoFrameReader::showFirstFrame(QList<QString> videos)
{
    unique_lock<mutex>lock(mux_);
    this->videos_.clear();
    this->icon_imgs_.clear();
    this->videos_ = videos;
    if(videos_.size() <= 0)
        return;
    mp_->stop();

    curentIndex_ = 0;
    if(videos_.size() <= 0)
        return;
    static bool is_first = true;
    if(is_first)
    {
        mp_->setVideoOutput(vs_);
        is_first = false;
    }
    QString video = "file://"+videos_[curentIndex_++];
    mp_->setMedia(QUrl(video));
    mp_->setMuted(true);
    mp_->play();

}

void VideoFrameReader::ProcessFrame(QVideoFrame &frame)
{
    static int count=0;
    count++;

    if(count %2 == 0)
    {
        unique_lock<mutex>lock(mux_);
        mp_->pause();
        frame.map(QAbstractVideoBuffer::ReadOnly);
        //        QImage recvImage(frame.bits(), frame.width(), frame.height(), QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat()));
        QImage recvImage(frame.width(), frame.height(), QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat()));

        for(int i = 0;i<frame.height();i++)
        {
            memcpy(recvImage.scanLine(i),frame.bits()+frame.bytesPerLine()*i,frame.bytesPerLine());
        }

        icon_imgs_.push_back(recvImage);
        if(!recvImage.isNull())
        {
            //recvImage.save(QString("./images/%1.jpg").arg(rand()), "JPG");

        }
        frame.unmap();
        if(curentIndex_ >= videos_.size())
        {

            mp_->stop();
            mp_->setMedia(QUrl());
            emit Icon_image_available(icon_imgs_);
            return;
        }
        //mp_->setVideoOutput(vs_);
        if(videos_.size() <= 0)
            return;
        mp_->stop();
        mp_->setMedia(QUrl("file://"+videos_[curentIndex_++]));
        mp_->setMuted(true);
        mp_->play();
        count = 0;
    }

}
