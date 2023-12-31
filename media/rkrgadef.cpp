#include "rkrgadef.h"
#include <string.h>
#include "im2d.h"
#include "RockchipRga.h"
#include "common/log.h"
using namespace std;

#define MODULE_TAG "RKRgaDef"

rga_buffer_handle_t importRgaBuffer(RKRgaDef::RgaInfo *info, int32_t format) {
    im_handle_param_t imParam;

    memset(&imParam, 0, sizeof(im_handle_param_t));

    imParam.width  = info->wstride;
    imParam.height = info->hstride;
    imParam.format = format;

    return importbuffer_fd(info->fd, &imParam);
}

void freeRgaBuffer(rga_buffer_handle_t handle) {
    releasebuffer_handle(handle);
}

void RKRgaDef::SetRgaInfo(RgaInfo *info, int32_t fd,
                           int32_t width, int32_t height,
                           int32_t wstride, int32_t hstride) {
    memset(info, 0, sizeof(RgaInfo));

    info->fd = fd;
    info->width = width;
    info->height = height;
    info->wstride = (wstride > 0) ? wstride : width;
    info->hstride = (hstride > 0) ? hstride : height;
}

bool RKRgaDef::RGBToNV12(RgaInfo srcInfo, RgaInfo dstInfo) {
    bool ret = true;

    rga_info_t src;
    rga_info_t dst;
    rga_buffer_handle_t srcHdl;
    rga_buffer_handle_t dstHdl;

    RockchipRga& rkRga(RockchipRga::get());

    RLOGD("rga src fd %d rect[%d, %d, %d, %d]", srcInfo.fd,
             srcInfo.width, srcInfo.height, srcInfo.wstride, srcInfo.hstride);
    RLOGD("rga dst fd %d rect[%d, %d, %d, %d]", dstInfo.fd,
             dstInfo.width, dstInfo.height, dstInfo.wstride, dstInfo.hstride);

    if ((srcInfo.wstride % 4) != 0) {
        RLOGE("err yuv not align to 4");
        return true;
    }

    memset((void*)&src, 0, sizeof(rga_info_t));
    memset((void*)&dst, 0, sizeof(rga_info_t));

    srcHdl = importRgaBuffer(&srcInfo, RK_FORMAT_RGBA_8888);
    dstHdl = importRgaBuffer(&dstInfo, RK_FORMAT_YCbCr_420_SP);
    if (!srcHdl || !dstHdl) {
        RLOGE("failed to import rga buffer\n");
        return false;
    }

    src.handle = srcHdl;
    dst.handle = dstHdl;
    rga_set_rect(&src.rect, 0, 0, srcInfo.width, srcInfo.height,
                 srcInfo.wstride, srcInfo.hstride, RK_FORMAT_RGBA_8888);
    rga_set_rect(&dst.rect, 0, 0, dstInfo.width, dstInfo.height,
                 dstInfo.wstride, dstInfo.hstride, RK_FORMAT_YCbCr_420_SP);

    if (rkRga.RkRgaBlit(&src, &dst, NULL)) {
        RLOGE("RgaBlit fail, rgbToNv12\n");
        ret = false;
    }

    freeRgaBuffer(srcHdl);
    freeRgaBuffer(dstHdl);

    return ret;
}

bool RKRgaDef::NV12ToNV12(RgaInfo srcInfo, RgaInfo dstInfo) {
    bool ret = true;

    rga_info_t src;
    rga_info_t dst;
    rga_buffer_handle_t srcHdl;
    rga_buffer_handle_t dstHdl;

    RockchipRga& rkRga(RockchipRga::get());

    RLOGD("rga src fd %d rect[%d, %d, %d, %d\n", srcInfo.fd,
             srcInfo.width, srcInfo.height, srcInfo.wstride, srcInfo.hstride);
    RLOGD("rga dst fd %d rect[%d, %d, %d, %d]\n", dstInfo.fd,
             dstInfo.width, dstInfo.height, dstInfo.wstride, dstInfo.hstride);

    if ((srcInfo.wstride % 4) != 0) {
        RLOGE("err yuv not align to 4\n");
        return true;
    }

    memset((void*)&src, 0, sizeof(rga_info_t));
    memset((void*)&dst, 0, sizeof(rga_info_t));

    srcHdl = importRgaBuffer(&srcInfo, RK_FORMAT_YCbCr_420_SP);
    dstHdl = importRgaBuffer(&dstInfo, RK_FORMAT_YCbCr_420_SP);
    if (!srcHdl || !dstHdl) {
        RLOGE("failed to import rga buffer\n");
        return false;
    }

    src.handle = srcHdl;
    dst.handle = dstHdl;
    rga_set_rect(&src.rect, 0, 0, srcInfo.width, srcInfo.height,
                 srcInfo.wstride, srcInfo.hstride, RK_FORMAT_YCbCr_420_SP);
    rga_set_rect(&dst.rect, 0, 0, dstInfo.width, dstInfo.height,
                 dstInfo.wstride, dstInfo.hstride, RK_FORMAT_YCbCr_420_SP);

    if (rkRga.RkRgaBlit(&src, &dst, NULL)) {
        RLOGE("RgaBlit fail, nv12Copy\n");
        ret = false;
    }

    freeRgaBuffer(srcHdl);
    freeRgaBuffer(dstHdl);

    return ret;
}

bool RKRgaDef::NV12ToRGB(RKRgaDef::RgaInfo srcInfo, RKRgaDef::RgaInfo dstInfo)
{
    bool ret = true;

    rga_info_t src;
    rga_info_t dst;
    rga_buffer_handle_t srcHdl;
    rga_buffer_handle_t dstHdl;

    RockchipRga& rkRga(RockchipRga::get());

    RLOGD("rga src fd %d rect[%d, %d, %d, %d\n", srcInfo.fd,
             srcInfo.width, srcInfo.height, srcInfo.wstride, srcInfo.hstride);
    RLOGD("rga dst fd %d rect[%d, %d, %d, %d]\n", dstInfo.fd,
             dstInfo.width, dstInfo.height, dstInfo.wstride, dstInfo.hstride);

    if ((srcInfo.wstride % 4) != 0) {
        RLOGE("err yuv not align to 4\n");
        return true;
    }

    memset((void*)&src, 0, sizeof(rga_info_t));
    memset((void*)&dst, 0, sizeof(rga_info_t));

    srcHdl = importRgaBuffer(&srcInfo, RK_FORMAT_YCbCr_420_SP);
    dstHdl = importRgaBuffer(&dstInfo, RK_FORMAT_RGBA_8888);
    if (!srcHdl || !dstHdl) {
        RLOGE("failed to import rga buffer [%d:%d]\n",srcHdl,dstHdl);
        return false;
    }

    src.handle = srcHdl;
    dst.handle = dstHdl;
    rga_set_rect(&src.rect, 0, 0, srcInfo.width, srcInfo.height,
                 srcInfo.wstride, srcInfo.hstride, RK_FORMAT_YCbCr_420_SP);
    rga_set_rect(&dst.rect, 0, 0, dstInfo.width, dstInfo.height,
                 dstInfo.wstride, dstInfo.hstride, RK_FORMAT_RGBA_8888);

    if (rkRga.RkRgaBlit(&src, &dst, NULL)) {
        RLOGE("RgaBlit fail, nv12Copy\n");
        ret = false;
    }

    freeRgaBuffer(srcHdl);
    freeRgaBuffer(dstHdl);

    return ret;
}

