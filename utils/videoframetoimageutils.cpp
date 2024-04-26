#include "videoframetoimageutils.h"
#include <QDebug>
#include <cstring>
QImage convertNV12ToRGB(QVideoFrame &NV12Frame) {
    // 获取视频帧的大小
    QSize size = NV12Frame.size();
    int width = NV12Frame.width();
    int height = NV12Frame.height();

    // Get the Y and UV planes
    uchar *yPlane = NV12Frame.bits();
    uchar *uvPlane = yPlane + width * height;

    // 创建RGB图像
    QImage rgbImage(width,height, QImage::Format_RGB888);

    // 进行YUV到RGB的颜色空间转换
    for (int y = 0; y < size.height(); ++y) {
        for (int x = 0; x < size.width(); ++x) {
            int Y = yPlane[y * width + x];
            int U = uvPlane[(y / 2) * width + (x / 2) * 2];
            int V = uvPlane[(y / 2) * width + (x / 2) * 2 + 1];

            // Convert YUV to RGB (accurate formula)
            int C = Y - 16;
            int D = U - 128;
            int E = V - 128;
            int R = qBound(0, (298 * C + 409 * E + 128) >> 8, 255);
            int G = qBound(0, (298 * C - 100 * D - 208 * E + 128) >> 8, 255);
            int B = qBound(0, (298 * C + 516 * D + 128) >> 8, 255);
            // 裁剪RGB值
            //R = qBound(0, R, 255);
            //G = qBound(0, G, 255);
            //B = qBound(0, B, 255);
            // 设置像素值
            rgbImage.setPixel(x, y, qRgb(R, G, B));
        }
    }
    return  rgbImage;
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


QImage VideoFrameToImageUtils::videoFrameToImage(QVideoFrame frame)
{
    // 确保视频帧格式可以转换为QImage
    if (!frame.isValid())return QImage();
    // 映射视频帧到内存
    if (!frame.map(QAbstractVideoBuffer::ReadOnly))return QImage();
    QImage image;
    qDebug()<<frame.pixelFormat();
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
    case QVideoFrame::Format_YV12:{image = convertYV12ToRGB(frame);}
        break;
        default:
        frame.unmap();
        return QImage();
    }

    // 解除视频帧映射
    frame.unmap();

    return image;

}
