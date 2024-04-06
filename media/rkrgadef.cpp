#include "rkrgadef.h"
#include <string.h>
#include "im2d.h"
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
                          int32_t width, int32_t height,int32_t format,
                          int32_t wstride, int32_t hstride) {
    memset(info, 0, sizeof(RgaInfo));

    info->fd = fd;
    info->width = width;
    info->height = height;
    info->format = format;
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

bool RKRgaDef::ProcessOSD(int32_t src_width, int32_t src_height, void *vir_addr, RKRgaDef::RgaInfo dstInfo,int32_t dst_format)
{
    int ret = 0;
    bool result = true;
    rga_buffer_t fg_img, bg_img, output_img;
    im_rect fg_rect, bg_rect, output_rect;
    rga_buffer_handle_t fg_handle, bg_handle, output_handle;
    int fg_width, fg_height, fg_format;
    int bg_width, bg_height, bg_format;
    int output_width, output_height, output_format;
    memset(&fg_img, 0, sizeof(fg_img));
    memset(&bg_img, 0, sizeof(bg_img));
    memset(&output_img, 0, sizeof(output_img));
    memset(&fg_rect, 0, sizeof(fg_rect));
    memset(&bg_rect, 0, sizeof(bg_rect));
    memset(&output_rect, 0, sizeof(output_rect));

    fg_width = src_width;
    fg_height = src_height;
    fg_format = RK_FORMAT_RGBA_8888;

    bg_width = dstInfo.width;
    bg_height = dstInfo.height;
    bg_format = dst_format;

    fg_handle = importbuffer_virtualaddr(vir_addr, fg_width*fg_height*4);

    bg_handle = importRgaBuffer(&dstInfo, RK_FORMAT_RGBA_8888);

    fg_img = wrapbuffer_handle(fg_handle, fg_width, fg_height, fg_format);
    bg_img = wrapbuffer_handle(bg_handle, bg_width, bg_height, bg_format);

    fg_rect.x = 0;
    fg_rect.y = 0;
    fg_rect.width = fg_width;
    fg_rect.height = fg_height;

    bg_rect.x = 0;
    bg_rect.y = 0;
    bg_rect.width = fg_rect.width;
    bg_rect.height = fg_rect.height;

    int usage = IM_SYNC | IM_ALPHA_BLEND_SRC_OVER | IM_ALPHA_BLEND_PRE_MUL;
    ret = improcess(fg_img, bg_img, {}, fg_rect, bg_rect, {}, -1, NULL, NULL, usage);

    if (ret == IM_STATUS_SUCCESS) {
        RLOGD("improcess running success!\n");
        result = true;
    } else {
        RLOGE("improcess running failed:%s\n", imStrError((IM_STATUS)ret));
        result = false;
    }

    if (fg_handle)
        releasebuffer_handle(fg_handle);
    if (bg_handle)
        releasebuffer_handle(bg_handle);

    return result;
}

bool RKRgaDef::ProcessOSD(RKRgaDef::RgaInfo srcInfo, RKRgaDef::RgaInfo dstInfo)
{
    int ret = 0;
    bool result = true;
    rga_buffer_t fg_img, bg_img, output_img;
    im_rect fg_rect, bg_rect, output_rect;
    rga_buffer_handle_t fg_handle, bg_handle, output_handle;
    int fg_width, fg_height, fg_format;
    int bg_width, bg_height, bg_format;
    int output_width, output_height, output_format;
    int usage;
    memset(&fg_img, 0, sizeof(fg_img));
    memset(&bg_img, 0, sizeof(bg_img));
    memset(&output_img, 0, sizeof(output_img));
    memset(&fg_rect, 0, sizeof(fg_rect));
    memset(&bg_rect, 0, sizeof(bg_rect));
    memset(&output_rect, 0, sizeof(output_rect));

    fg_width = dstInfo.width;
    fg_height = dstInfo.height;
    fg_format = dstInfo.format;

    bg_width = srcInfo.width;
    bg_height = srcInfo.height;
    bg_format = RK_FORMAT_RGBA_8888;

    output_width = dstInfo.width;
    output_height = dstInfo.height;
    output_format = dstInfo.format;

    //    fg_handle = importRgaBuffer(&dstInfo, fg_format);
    //    bg_handle = importRgaBuffer(&srcInfo, bg_format);

    //    fg_img = wrapbuffer_handle(fg_handle, fg_width, fg_height, fg_format);
    //    bg_img = wrapbuffer_handle(bg_handle, bg_width, bg_height, bg_format);
    //    output_img = wrapbuffer_handle(fg_handle, output_width, output_height, output_format);

    fg_img = wrapbuffer_fd_t(dstInfo.fd,dstInfo.width,dstInfo.height,dstInfo.wstride,dstInfo.hstride,fg_format);
    bg_img = wrapbuffer_fd_t(srcInfo.fd, srcInfo.width, srcInfo.height, srcInfo.wstride,srcInfo.hstride,bg_format);
    output_img = wrapbuffer_fd_t(dstInfo.fd,dstInfo.width,dstInfo.height,dstInfo.wstride,dstInfo.hstride,fg_format);
    fg_rect.x = 0;
    fg_rect.y = 0;
    fg_rect.width = bg_width;
    fg_rect.height = bg_height;

    bg_rect.x = 0;
    bg_rect.y = 0;
    bg_rect.width = fg_rect.width;
    bg_rect.height = fg_rect.height;

    output_rect.x = fg_width - bg_width-20;
    output_rect.y = 20;
    output_rect.width = fg_rect.width;
    output_rect.height = fg_rect.height;

    usage = IM_SYNC | IM_ALPHA_BLEND_DST_OVER | IM_ALPHA_BLEND_PRE_MUL;
    ret = improcess(fg_img, output_img, bg_img, fg_rect, output_rect, bg_rect, -1, NULL, NULL, usage);
    if (ret == IM_STATUS_SUCCESS) {
        result = true;
    } else {
        RLOGE("improcess running failed:%s\n", imStrError((IM_STATUS)ret));
        result = false;
    }

    //    freeRgaBuffer(fg_handle);
    //    freeRgaBuffer(bg_handle);

    return result;
}

