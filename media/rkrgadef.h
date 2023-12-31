#ifndef RKRGADEF_H
#define RKRGADEF_H


#include <stdio.h>
#include <string.h>
#include <stdint.h>

class RKRgaDef {
public:
    typedef struct RgaInfo {
        int32_t fd;
        int32_t width;
        int32_t height;
        int32_t wstride;
        int32_t hstride;
    } RgaInfo_t;
    static void SetRgaInfo(RgaInfo *param, int32_t fd,
                          int32_t width, int32_t height,
                          int32_t wstride = 0, int32_t hstride = 0);

    static bool RGBToNV12(RgaInfo srcInfo, RgaInfo dstInfo);
    static bool NV12ToNV12(RgaInfo srcInfo, RgaInfo dstInfo);
    static bool NV12ToRGB(RgaInfo srcInfo, RgaInfo dstInfo);
};
#endif // RKRGADEF_H
