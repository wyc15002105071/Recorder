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

#define IMAGES_SAVE_DIR "/userdata/images"
#define VIDEOS_SAVE_DIR "/userdata/videos"

typedef enum {
    VIDEO_ENCODE_TYPE_AVC,
    VIDEO_ENCODE_TYPE_HEVC,
}VIDEO_ENCODE_TYPE;

typedef enum {
    MUX_FILE_TYPE_MP4,
    MUX_FILE_TYPE_AVI,
    MUX_FILE_TYPE_MKV,
}MUX_FILE_TYPE;

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

int getVideoNodesCount(const char* directory);
void getVideoNodesInfo(std::vector<VideoNodeInfo> &infos);
int printFps(const char *TAG);
void getCurentTime(char *str,const char *format);
std::string fcc2s(__u32 val);

#endif // MEDIAUTILS_H
