#ifndef CAPTUREHELPER_H
#define CAPTUREHELPER_H

#include <QImage>
#include <vector>

#include "imageconvert.h"

#include "common/rthread.h"
#include "common/log.h"

#include "media/mediautils.h"
#include "utils/mediapathutils.h"

class CaptureHelper : public RThread {
    Q_OBJECT
public:
    virtual void run() {
        while (!mThreadExit) {
            if(mBuffers.size() <= 0) {
                RLOGE("no task doing...");
                return;
            }

            DmaBufferObject dmabuf = mBuffers[0];
            int format = dmabuf.v4l2_format;

            int width = dmabuf.width;
            int height = dmabuf.height;

            if(width * height <= 0) {
                RLOGD("invaild resolution [%dx%d]",width,height);
                return;
            }
            unsigned char *rgb = nullptr;
            rgb = (unsigned char *)malloc(width * height * 3); // RGB数据
            bool success = true;
            switch (format) {
            case V4L2_PIX_FMT_NV12: {
                NV12ToRGB_Soft((unsigned char *)dmabuf.vir_addr,rgb,width,height);
                QImage image(rgb,width,height,QImage::Format_BGR888);
                image.save(MediaPathUtils::get_instance()->getImagePath());
            } break;
            case V4L2_PIX_FMT_NV16: {
                NV16ToRGB_Soft((unsigned char *)dmabuf.vir_addr,rgb,width,height);
                QImage image(rgb,width,height,QImage::Format_BGR888);
                image.save(MediaPathUtils::get_instance()->getImagePath());
            } break;
            case V4L2_PIX_FMT_NV24: {
                NV24ToRGB_Soft((unsigned char *)dmabuf.vir_addr,rgb,width,height);
                QImage image(rgb,width,height,QImage::Format_BGR888);
                image.save(MediaPathUtils::get_instance()->getImagePath());
            } break;
            case V4L2_PIX_FMT_RGB24: {
                QImage image((uchar *)dmabuf.vir_addr,width,height,QImage::Format_BGR888);
                image.save(MediaPathUtils::get_instance()->getImagePath());
            } break;
            case V4L2_PIX_FMT_BGR24: {
                QImage image((uchar *)dmabuf.vir_addr,width,height,QImage::Format_BGR888);
                image.save(MediaPathUtils::get_instance()->getImagePath());
            } break;
            default: {
                RLOGE("format %s not support",fcc2s(format).c_str());
                success = false;
            } break;
            }

            if(success) {
                RLOGD("capture success...");
                onFinished();
            }

            if(rgb) {
                free(rgb);
                rgb = nullptr;
            }
            break;

        }

        mBuffers.clear();
        mThreadExit = true;
    }

    void setFrameParam(DmaBufferObject &dmabuf,int format) {
        dmabuf.v4l2_format = format;
        mBuffers.push_back(dmabuf);
    }

private:
    std::vector<DmaBufferObject> mBuffers;
signals:
    void onFinished();
};

#endif // CAPTUREHELPER_H
