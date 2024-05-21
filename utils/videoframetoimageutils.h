#ifndef VIDEOFRAMETOIMAGEUTILS_H
#define VIDEOFRAMETOIMAGEUTILS_H

#include "common/rthread.h"
#include <QMutex>
#include <QObject>
#include <QImage>
#include <QVideoFrame>

class VideoFrameToImageUtils: public RThread
{
    Q_OBJECT
public:
    VideoFrameToImageUtils();
    ~VideoFrameToImageUtils();


    virtual void run();
    static QImage videoFrameToImage(QVideoFrame frame);

    void saveFrame(QVideoFrame frame);
signals:
    void saveImageFinish();
private:
    QList<QVideoFrame> mFrames;
    QMutex mux;
};

#endif // VIDEOFRAMETOIMAGEUTILS_H
