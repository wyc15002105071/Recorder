#ifndef CAPTUREHELPER_H
#define CAPTUREHELPER_H

#include <QImage>
#include <vector>

#include "imageconvert.h"

#include "common/rthread.h"
#include "common/log.h"

#include "media/mediautils.h"
#include "utils/mediapathutils.h"

#include "media/rkrgadef.h"

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

            void *rgb = nullptr;
            rgb = malloc(width * height * 3); // RGB数据
            bool success = true;
            switch (format) {
            case V4L2_PIX_FMT_NV12: {
                RKRgaDef::RgaInfo srcInfo, dstInfo;
                RKRgaDef::SetRgaInfo(&srcInfo, dmabuf.buf_fd, dmabuf.width, dmabuf.height,RK_FORMAT_YCbCr_420_SP,dmabuf.width, dmabuf.height);
                RKRgaDef::SetRgaInfo(&dstInfo, mDstBo.buf_fd, mDstBo.width, mDstBo.height, RK_FORMAT_RGBA_8888 ,mDstBo.width, mDstBo.height);
                if(!RKRgaDef::convertFormat(srcInfo,dstInfo)) {
                    RLOGE("NV12ToRGB failed\n");
                } else {
                    RLOGD("NV12ToRGB success\n");
                }

                QImage image((uchar *)mDstBo.vir_addr,width,height,QImage::Format_RGBA8888);
                image.save(MediaPathUtils::get_instance()->getImagePath());
            } break;
            case V4L2_PIX_FMT_NV16: {
                RKRgaDef::RgaInfo srcInfo, dstInfo;
                RKRgaDef::SetRgaInfo(&srcInfo, dmabuf.buf_fd, dmabuf.width, dmabuf.height,RK_FORMAT_YCbCr_422_SP,dmabuf.width, dmabuf.height);
                RKRgaDef::SetRgaInfo(&dstInfo, mDstBo.buf_fd, mDstBo.width, mDstBo.height, RK_FORMAT_RGBA_8888 ,mDstBo.width, mDstBo.height);
                if(!RKRgaDef::convertFormat(srcInfo,dstInfo)) {
                    RLOGE("NV12ToRGB failed\n");
                } else {
                    RLOGD("NV12ToRGB success\n");
                }

                QImage image((uchar *)mDstBo.vir_addr,width,height,QImage::Format_RGBA8888);
                image.save(MediaPathUtils::get_instance()->getImagePath());
            } break;
            case V4L2_PIX_FMT_NV24: {
                NV24ToRGB_Soft((unsigned char *)dmabuf.vir_addr,(unsigned char *)rgb,width,height);
                QImage image((uchar *)rgb,width,height,QImage::Format_BGR888);
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

    void setFrameParam(DmaBufferObject &srcbuf,DmaBufferObject &dstbuf, int format) {
        srcbuf.v4l2_format = format;
        mBuffers.push_back(srcbuf);

        mDstBo = dstbuf;
    }

private:
    std::vector<DmaBufferObject> mBuffers;
    DmaBufferObject mDstBo;
signals:
    void onFinished();
};

#endif // CAPTUREHELPER_H
