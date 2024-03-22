#include "mediautils.h"
#include <memory>
#include "log.h"
#include <math.h>

using namespace std;
#define MODULE_TAG "MediaUtils"
#define MAX_NODE_COUNT 1024
shared_ptr<char>nodes[MAX_NODE_COUNT];
static int video_node_count = 0;

string fcc2s(__u32 val)
{
    std::string s;

    s += val & 0x7f;
    s += (val >> 8) & 0x7f;
    s += (val >> 16) & 0x7f;
    s += (val >> 24) & 0x7f;
    if (val & (1U << 31))
        s += "-BE";
    return s;
}

int printFps(const char *TAG)
{
    static int inFrameCount = 0;
    static int inLastFrameCount = 0;
    static long inLastFpsTimeUs = 0;
    static float inFps = 0;
    static long inDiff = 0;
    static long inNowUs = 0;

    struct timeval current_time;
    char time_str[30];
    gettimeofday(&current_time, NULL);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&current_time.tv_sec));
    inFrameCount++;

    struct timeval now;
    gettimeofday(&now, NULL);
    inNowUs = (long)now.tv_sec * 1000000 + (long)now.tv_usec;
    inDiff = inNowUs - inLastFpsTimeUs;

    if (!(inFrameCount & 0x1F) && inDiff > 2000000) {
        struct timeval now;
        gettimeofday(&now, NULL);
        inNowUs = (long)now.tv_sec * 1000000 + (long)now.tv_usec;
        inDiff = inNowUs - inLastFpsTimeUs;
        inFps = ((float)(inFrameCount - inLastFrameCount) * 1.0f) * 1000.0f * 1000.0f / (float)inDiff;
        inLastFpsTimeUs = inNowUs;
        inLastFrameCount = inFrameCount;
        RLOGD("%s frameCount = %d frameRate = %f HZ\n",TAG,inFrameCount, inFps);
    }
    return inFps;
}


int getVideoNodesCount(const char *directory)
{
    DIR *dir;
    struct dirent *entry;
    int count = 0;

    // 打开指定目录
    dir = opendir(directory);
    if (!dir) {
        RLOGE("Unable to open directory");
        return -1;
    }

    // 遍历目录中的文件
    while ((entry = readdir(dir)) != NULL) {
        // 检查文件名是否以 "video" 开头
        if (strncmp(entry->d_name, "video", 5) == 0) {

            char name[1024] = {0};
            sprintf(name,"%s/%s",directory,entry->d_name);
            nodes[count] = shared_ptr<char>(new char[strlen(name) + 1]);
            strcpy(nodes[count].get(),name);
            count++;
        }
    }

    closedir(dir);
    video_node_count = count;
    return count;
}

void getVideoNodesInfo(std::vector<VideoNodeInfo> &infos)
{
    int fd = 0;

    for(int i=0;i<video_node_count;i++){
        RLOGD("open node path %s",nodes[i].get());
        int fd = open(nodes[i].get(), O_RDWR);
        if (fd == -1) {
            RLOGE("Failed to open device");
            continue;
        }
        struct v4l2_capability caps;
        if (ioctl(fd, VIDIOC_QUERYCAP, &caps) == -1) {
            RLOGE("Failed to query device capabilities");
            close(fd);
            infos[i].node_path = nodes[i].get();
            infos[i].driver = "";
            infos[i].bus_info = "";
            infos[i].version = 0;
            infos[i].capabilities = 0;
            continue;
        }

        infos[i].node_path = nodes[i].get();
        infos[i].driver = (char *)caps.driver;
        infos[i].bus_info = (char *)caps.bus_info;
        infos[i].version = caps.version;
        infos[i].capabilities = caps.capabilities;
        close(fd);
    }
}

void getCurentTime(char *str,const char *format)
{
    struct timeval current_time;
    char time_str[30];
    char *str_fmt = "%Y-%m-%d_%H:%M:%S";
    if(format)
        str_fmt = (char *)format;
    gettimeofday(&current_time, NULL);
    strftime(time_str, sizeof(time_str), str_fmt, localtime(&current_time.tv_sec));

    strcpy(str,time_str);
}

string convertSecondToTime(int64_t seconds)
{
    char time_str[100] = {0};

    int hours, minutes, secs;

    // 计算小时、分钟和秒数
    hours = seconds / 3600;
    seconds %= 3600;
    minutes = seconds / 60;
    seconds %= 60;
    secs = seconds;

    sprintf(time_str,"%02d:%02d:%02d",hours,minutes,secs);

    return string(time_str);
}

string convertMillisecondsToTime(long long milliseconds)
{
    char time_str[100] = {0};
    long long seconds = ceil((double)milliseconds / (double)1000); // 将毫秒转换为秒
    int hours, minutes, secs;

    // 计算小时、分钟和秒数
    hours = seconds / 3600;
    seconds %= 3600;
    minutes = seconds / 60;
    seconds %= 60;
    secs = seconds;

    sprintf(time_str,"%02d:%02d:%02d",hours,minutes,secs);

    return string(time_str);
}

const char *getSuffix(Suffix_t suffix)
{
    switch (suffix) {
    case Suffix_MP4:
        return "mp4";
        break;
    case Suffix_AVI:
        return "avi";
        break;
    case Suffix_MKV:
        return "mkv";
        break;
    case Suffix_TS:
        return "ts";
        break;
    default:
        return "mp4";
    }
}
