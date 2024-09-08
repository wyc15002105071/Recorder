#ifndef VIDEOFRAMETOIMAGEUTILS_H
#define VIDEOFRAMETOIMAGEUTILS_H

#include <QImage>
#include <QVideoFrame>

class VideoFrameToImageUtils
{
public:
    static QImage videoFrameToImage(QVideoFrame frame);
};

#endif // VIDEOFRAMETOIMAGEUTILS_H
