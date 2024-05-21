#include "videoframetoimageutils.h"
#include "common/log.h"
#include "utils/mediapathutils.h"
#include "utils/configutils.h"

QImage convertNV12ToRGB(QVideoFrame &frame) {
    if (!frame.isValid()) {
        //qDebug() << "Invalid video frame";
        return QImage();
    }

    int width = frame.width();
    int height = frame.height();

    QImage image(width, height, QImage::Format_RGB888);

    // Convert NV12 to RGB888
    const uchar *yPlane = frame.bits(0);
    const uchar *uvPlane = frame.bits(1);
    uchar *rgbData = image.bits();
    int uvRowStride = frame.bytesPerLine(1);
    int yRowStride = frame.bytesPerLine();

    for (int y = 0; y < height; ++y) {
        const uchar *yLine = yPlane + y * yRowStride;
        const uchar *uvLine = uvPlane + (y / 2) * uvRowStride;

        for (int x = 0; x < width; ++x) {
            int yValue = yLine[x];
            int u = uvLine[x & ~1] - 128;
            int v = uvLine[x | 1] - 128;

            int r = qBound(0, (298 * yValue + 409 * v + 128) >> 8, 255);
            int g = qBound(0, (298 * yValue - 100 * u - 208 * v + 128) >> 8, 255);
            int b = qBound(0, (298 * yValue + 516 * u + 128) >> 8, 255);

            rgbData[0] = r;
            rgbData[1] = g;
            rgbData[2] = b;
            rgbData += 3;
        }
    }

    return image;
}


QImage convertYV12ToRGB(QVideoFrame &yv12Frame) {
    // 获取视频帧的大小
    QSize size = yv12Frame.size();

    // 获取YUV分量的数据指针
    uchar *yuvData = yv12Frame.bits();
    int yPlaneBytes = size.width() * size.height();
    int uvPlaneBytes = yPlaneBytes / 4;
    uchar *yPlane = yuvData;
    uchar *uPlane = yuvData + yPlaneBytes + uvPlaneBytes;
    uchar *vPlane = yuvData + yPlaneBytes;

    // 创建RGB图像
    QImage rgbImage(size, QImage::Format_RGB888);

    // 进行YUV到RGB的颜色空间转换
    for (int y = 0; y < size.height(); ++y) {
        for (int x = 0; x < size.width(); ++x) {
            int Y = yPlane[y * size.width() + x];
            int U = uPlane[(y / 2) * (size.width() / 2) + (x / 2)];
            int V = vPlane[(y / 2) * (size.width() / 2) + (x / 2)];

            // YUV到RGB转换
            int R = (int)(Y + 1.370705 * (V - 128));
            int G = (int)(Y - 0.698001 * (V - 128) - 0.337633 * (U - 128));
            int B = (int)(Y + 1.732446 * (U - 128));

            // 裁剪RGB值
            R = qBound(0, R, 255);
            G = qBound(0, G, 255);
            B = qBound(0, B, 255);

            // 设置像素值
            rgbImage.setPixel(x, y, qRgb(R, G, B));
        }
    }
    return  rgbImage;
}


VideoFrameToImageUtils::VideoFrameToImageUtils()
{

}

VideoFrameToImageUtils::~VideoFrameToImageUtils()
{

}

void VideoFrameToImageUtils::run()
{
    while(!mThreadExit){
        if(mFrames.length() <= 0) {
            RLOGE("no task doing...");
            return;
        }
        mux.lock();
        QVideoFrame frame = mFrames.takeFirst();
        mux.unlock();

        // 确保视频帧格式可以转换为QImage
        if (!frame.isValid())return;
        // 映射视频帧到内存
        if (!frame.map(QAbstractVideoBuffer::ReadOnly))return;

        QImage image = videoFrameToImage(frame);
        if(!image.isNull()){
            if(image.save(MediaPathUtils::get_instance()->getImagePath())){
                if(ConfigUtils::isUsbMedia)
                    system("sync");
                emit saveImageFinish();
            }
        }
    }

    mux.lock();
    mFrames.clear();
    mux.unlock();
    mThreadExit = true;
}

QImage VideoFrameToImageUtils::videoFrameToImage(QVideoFrame frame)
{
    QImage image;

    // 根据视频帧创建QImage对象
    switch (frame.pixelFormat()) {
        case QVideoFrame::Format_ARGB32:
        case QVideoFrame::Format_ARGB32_Premultiplied:
        case QVideoFrame::Format_RGB32:
            image = QImage(frame.bits(), frame.width(), frame.height(), QImage::Format_ARGB32).copy();
            break;
        case QVideoFrame::Format_RGB565:
            image = QImage(frame.bits(), frame.width(), frame.height(), QImage::Format_RGB16).copy();
            break;
        case QVideoFrame::Format_RGB24:
            image = QImage(frame.bits(), frame.width(), frame.height(), QImage::Format_RGB888).copy();
            break;
    case QVideoFrame::Format_NV12:{image = convertNV12ToRGB(frame);}break;
    case QVideoFrame::Format_NV21:{image = convertNV12ToRGB(frame);}break;
    case QVideoFrame::Format_YV12:{image = convertYV12ToRGB(frame);}break;
        default:
        frame.unmap();
        return QImage();
    }

    // 解除视频帧映射
    frame.unmap();

    return image;

}

void VideoFrameToImageUtils::saveFrame(QVideoFrame frame)
{
    mux.lock();
    mFrames.push_back(frame);
    mux.unlock();
}
