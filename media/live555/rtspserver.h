#ifndef RTSPSERVER_H
#define RTSPSERVER_H

#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "GroupsockHelper.hh"
#include <thread>
#include "media/mediautils.h"
#include "common/Mutex.h"
#include "media/rkmpp_enc_api.h"

class RtspServer
{
public:
    RtspServer();
    ~RtspServer();
#define STREAM_NAME "stream"
    typedef struct {
        char dev_name[64];    // camera v4l2 device name
        int  width;           // camera v4l2 vfmt width
        int  height;          // camera v4l2 vfmt height
        int  format;
        int  type;
        int  port_num;        // rtsp port number
    } MetaInfo;

    bool prepare(MetaInfo *meta);
    bool init(MetaInfo *meta);
    void setDoneFlag() { fDoneFlag = ~0; }
    void resetDoneFlag() { fDoneFlag = 0; }
    void doEventLoop();

    bool start();
    bool stop();

    void setData(DmaBufferObject &dmabo);
    void getData(DmaBufferObject &dmabo);

    std::string getUrl();
private:
    UsageEnvironment* mEnv;
    ServerMediaSession* mSms;
    RTSPServer* mRtspServer;

    char mStreamName[64];

    char fDoneFlag;

    std::thread *mProcThread;
    DmaBufferObject mDmaBo;
    RMutex mLock;
    RKHWEncApi mVideoEncoder;
    bool mStart;
    bool initOther(MetaInfo *meta);
    char* url = "";
};

int init_rtsp(const char* video_dev, int width, int height);
void deinit_rtsp();

#endif // RTSPSERVER_H
