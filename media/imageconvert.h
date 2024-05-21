#ifndef IMAGECONVERT_H
#define IMAGECONVERT_H

#include <stdio.h>
#include <stdlib.h>

// 定义最大值和最小值的宏
#define CLAMP(value, min, max) ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))

// NV12转RGB函数
static
void NV12ToRGB_Soft(unsigned char *nv12, unsigned char *rgb, int width, int height) {
    int frameSize = width * height;
    int chromaSize = frameSize / 2;

    unsigned char *yPlane = nv12;
    unsigned char *uvPlane = nv12 + frameSize;

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int yIndex = j * width + i;
            int uvIndex = (j / 2) * width + (i & ~1);

            unsigned char Y = yPlane[yIndex];
            unsigned char U = uvPlane[uvIndex + 1];
            unsigned char V = uvPlane[uvIndex];

            // YUV to RGB conversion
            int C = Y - 16;
            int D = U - 128;
            int E = V - 128;

            int R = CLAMP((298 * C + 409 * E + 128) >> 8, 0, 255);
            int G = CLAMP((298 * C - 100 * D - 208 * E + 128) >> 8, 0, 255);
            int B = CLAMP((298 * C + 516 * D + 128) >> 8, 0, 255);

            // 设置RGB值
            int rgbIndex = yIndex * 3;
            rgb[rgbIndex] = R;
            rgb[rgbIndex + 1] = G;
            rgb[rgbIndex + 2] = B;
        }
    }
}

static // NV16转RGB函数
void NV16ToRGB_Soft(unsigned char *nv16, unsigned char *rgb, int width, int height) {
    int frameSize = width * height;
    unsigned char *yPlane = nv16;
    unsigned char *uvPlane = nv16 + frameSize;

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int yIndex = j * width + i;
            int uvIndex = (j * width) + (i & ~1); // UV平面相对于Y平面每个2个Y采样有1组UV值

            unsigned char Y = yPlane[yIndex];
            unsigned char U = uvPlane[uvIndex];
            unsigned char V = uvPlane[uvIndex + 1];

            // YUV to RGB conversion
            int C = Y - 16;
            int D = U - 128;
            int E = V - 128;

            int R = CLAMP((298 * C + 409 * E + 128) >> 8, 0, 255);
            int G = CLAMP((298 * C - 100 * D - 208 * E + 128) >> 8, 0, 255);
            int B = CLAMP((298 * C + 516 * D + 128) >> 8, 0, 255);

            // 设置RGB值
            int rgbIndex = yIndex * 3;
            rgb[rgbIndex] = R;
            rgb[rgbIndex + 1] = G;
            rgb[rgbIndex + 2] = B;
        }
    }
}

static // NV24转RGB函数
void NV24ToRGB_Soft(unsigned char *nv24, unsigned char *rgb, int width, int height) {
    int frameSize = width * height;

    unsigned char *yPlane = nv24;
    unsigned char *uvPlane = nv24 + frameSize;

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int yIndex = j * width + i;
            int uvIndex = yIndex * 2;

            unsigned char Y = yPlane[yIndex];
            unsigned char U = uvPlane[uvIndex];
            unsigned char V = uvPlane[uvIndex + 1];

            // YUV to RGB conversion
            int C = Y - 16;
            int D = U - 128;
            int E = V - 128;

            int R = CLAMP((298 * C + 409 * E + 128) >> 8, 0, 255);
            int G = CLAMP((298 * C - 100 * D - 208 * E + 128) >> 8, 0, 255);
            int B = CLAMP((298 * C + 516 * D + 128) >> 8, 0, 255);

            // 设置RGB值
            int rgbIndex = yIndex * 3;
            rgb[rgbIndex] = R;
            rgb[rgbIndex + 1] = G;
            rgb[rgbIndex + 2] = B;
        }
    }
}

#endif // IMAGECONVERT_H
