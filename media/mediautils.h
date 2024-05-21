#ifndef MEDIA_UTILS_H
#define MEDIA_UTILS_H

#include <vector>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <errno.h>
#include <string>
#include <unistd.h>
#include <sys/ioctl.h>

//#define IMAGES_SAVE_DIR "/userdata/images"
//#define VIDEOS_SAVE_DIR "/userdata/videos"
//#define ROOT_DIR        "/userdata"
#define IMAGES_SAVE_DIR "/mnt/storage/images"
#define VIDEOS_SAVE_DIR "/mnt/storage/videos"
#define ROOT_DIR        "/mnt/storage"

typedef struct DmaBufferObject{
    int32_t     buf_fd;
    int32_t     buf_size;
    void       *vir_addr;
    int32_t     width;
    int32_t     height;
    int32_t     v4l2_format;
} DmaBufferObject_t;

typedef enum Video_CodingType{
    Video_CodingType_AVC,
    Video_CodingType_HEVC,
    Video_CodingType_VP8,
    Video_CodingType_VP9
} Video_CodingType_t;

typedef enum {
    MUX_FILE_TYPE_MP4,
    MUX_FILE_TYPE_AVI,
    MUX_FILE_TYPE_MKV,
}MUX_FILE_TYPE;

typedef enum Suffix_t {
    Suffix_MP4,
    Suffix_MKV,
    Suffix_AVI,
    Suffix_TS,
} Suffix;

typedef enum Push_t {
    Push_RTSP,
    Push_HTTP,
    Push_RTMP
} Push;

typedef enum {
    MUX_PUSH_TYPE_RTSP,
    MUX_PUSH_TYPE_RTMP,
    MUX_PUSH_TYPE_HTTP,
}MUX_PUSH_TYPE;

typedef struct _VideoNodeInfo {
    std::string node_path;
    std::string driver;
    std::string card;
    std::string bus_info;
    unsigned int version;
    int capabilities;
}VideoNodeInfo;

int  getVideoNodesCount(const char* directory);
void getVideoNodesInfo(std::vector<VideoNodeInfo> &infos);
int  printFps(const char *TAG);
void getCurentTime(char *str,const char *format);
std::string fcc2s(__u32 val);
std::string convertSecondToTime(int64_t seconds);
std::string convertMillisecondsToTime(long long milliseconds);
const char* getSuffix(Suffix_t suffix);
#endif // MEDIAUTILS_H
