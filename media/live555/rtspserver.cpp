#define LOG_TAG "RtspServer"
#include <stdlib.h>
#include <stdio.h>
#include "h264livevideoservermediasubsession.h"
#include "rtspserver.h"
#include "common/log.h"

RtspServer::RtspServer() :
    mEnv(NULL),
    mSms(NULL),
    mRtspServer(NULL),
    mProcThread(NULL),
    mStart(false)
{
    RLOGD("RtspServer enter");
}

RtspServer::~RtspServer()
{
    RLOGD("~RtspServer enter");
    if (mProcThread != NULL) {
        delete mProcThread;
        mProcThread = NULL;
    }

}

bool RtspServer::prepare(RtspServer::MetaInfo *meta)
{
    if (meta == NULL) {
        RLOGE("Failed to get metaData");
        return false;
    }

    RKHWEncApi::EncCfgInfo cfg;
    memset(&cfg,0,sizeof(RKHWEncApi::EncCfgInfo));
    cfg.width = meta->width;
    cfg.height = meta->height;
    cfg.format = meta->format;
    cfg.type = meta->type;
    cfg.bitRate = cfg.width * cfg.height / 30 * 60;
    if(mVideoEncoder.prepare(&cfg) != MPP_OK) {
        RLOGE("Failed to prepare video encoder");
        return false;
    }

    if (mEnv == NULL) {
        // begin by setting up our usage environment.
        TaskScheduler* scheduler = BasicTaskScheduler::createNew();
        mEnv = BasicUsageEnvironment::createNew(*scheduler);

        OutPacketBuffer::maxSize = meta->width * meta->height * 2;
        if(meta->port_num == 0) {
            meta->port_num = 8554;
        }

        mRtspServer = RTSPServer::createNew(*mEnv, meta->port_num);
        if (mRtspServer == NULL) {
            RLOGE("Failed to create RTSP server: %s", mEnv->getResultMsg());
            return false;
        }
    }

    return true;
}

bool RtspServer::init(MetaInfo *meta)
{
    if (meta == NULL) {
        printf("Failed to get metaData\n");
        return false;
    }

    if (mEnv == NULL) {
        // begin by setting up our usage environment.
        TaskScheduler* scheduler = BasicTaskScheduler::createNew();
        mEnv = BasicUsageEnvironment::createNew(*scheduler);

        OutPacketBuffer::maxSize = meta->width * meta->height * 2;
        if(meta->port_num == 0) {
            meta->port_num = 8554;
        }
        mRtspServer = RTSPServer::createNew(*mEnv, meta->port_num);
        if (mRtspServer == NULL) {
            printf("Failed to create RTSP server: %s\n", mEnv->getResultMsg());
            return false;
        }
    }


    return initOther(meta);
}

bool RtspServer::initOther(MetaInfo *meta)
{
    return true;
}

bool RtspServer::start()
{
    RLOGD("Start streaming...\n");
    resetDoneFlag();
    mProcThread = new std::thread(&RtspServer::doEventLoop, this);

    mSms = ServerMediaSession::createNew(*mEnv, STREAM_NAME, 0,0);
    mSms->addSubsession(H264LiveVideoServerMediaSubsession::createNew(*mEnv, True, this,&mVideoEncoder));
    OutPacketBuffer::maxSize = 1920*1080*2;;
    mRtspServer->addServerMediaSession(mSms);

    char* url = mRtspServer->rtspURL(mSms);
    RLOGD("Play this stream using the URL: %s", url);
    delete[] url;

    mStart = true;
    return true;
}

bool RtspServer::stop()
{
    RLOGD("Stop streaming... Exit scheduler eventLoop\n");
    setDoneFlag();

    RLOGD("Exit procThread\n");
    if (mProcThread != NULL) {
        mProcThread->join();
        delete mProcThread;
        mProcThread = NULL;
    }

    if (mRtspServer != NULL) {
        mRtspServer->deleteServerMediaSession(STREAM_NAME);
    }

    mVideoEncoder.release();
    mStart = false;
    return true;
}

void RtspServer::setData(DmaBufferObject &dmabo)
{
    mLock.lock();
    this->mDmaBo = std::move(dmabo);
    mLock.unlock();
}

void RtspServer::getData(DmaBufferObject &dmabo)
{
    mLock.lock();
    dmabo = std::move(this->mDmaBo);
    mLock.unlock();
}

std::string RtspServer::getUrl()
{
    if(mStart)
        return mRtspServer->rtspURL(mSms);
    else
        return "";
}

void RtspServer::doEventLoop()
{
    RLOGD("Env doEventLoop\n");
    mEnv->taskScheduler().doEventLoop(&fDoneFlag);
    RLOGD("Env doEventLoop exit\n");
}

static RtspServer _g_rtsp_server;

int init_rtsp(const char *video_dev, int width, int height)
{
    RtspServer::MetaInfo info;
    info.width = width;
    info.height = height;
    info.port_num = 8554;
    strcat(info.dev_name, video_dev);

    printf("init_rtsp: video_dev %s w %d h %d\n", video_dev, width, height);

    if (!_g_rtsp_server.init(&info)) {
        printf("Failed to init _g_rtsp_server\n");
        return -1;
    }
    _g_rtsp_server.start();

    return 0;
}

void deinit_rtsp()
{
    printf("deinit_rtsp enter\n");
    _g_rtsp_server.stop();
}
