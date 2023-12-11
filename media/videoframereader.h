#ifndef VIDEOFRAMEREADER_H
#define VIDEOFRAMEREADER_H

#include <QObject>
#include <QAbstractVideoSurface>
#include <QMediaPlayer>
#include <QLabel>
#include <QListWidget>
#include <mutex>

class VideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    VideoSurface(QObject *parent = Q_NULLPTR);
    ~VideoSurface();

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const;
    bool present(const QVideoFrame &frame);

signals:
    void frameAvailable(QVideoFrame &frame);
};

class VideoFrameReader:public QObject
{
    Q_OBJECT
public:
    static VideoFrameReader *Instance()
    {
        static VideoFrameReader vr;
        return &vr;
    }
    void showFirstFrame(QList<QString> videos);
    void stop();
public slots:
    void ProcessFrame(QVideoFrame &frame);
    void onError(QMediaPlayer::Error error);
private:
    VideoFrameReader();
signals:
    void imagesAvailable(QList<QImage> image_list);
    void imageAvailable(QImage image);
private:
    QMediaPlayer *mp_ = nullptr;
    VideoSurface *vs_ = nullptr;
    std::mutex mux_;
    QList<QString> videos_;
    QList<QImage>icon_imgs_;
    int curentIndex_ = 0;
};

#endif // VIDEOFRAMEREADER_H
