#include "streampusher.h"

StreamPusher::StreamPusher()
    : mRtspServer(sp<RtspServer>(new RtspServer))
{

}

bool StreamPusher::prepare(int width,int height,int format,int type)
{
    RtspServer::MetaInfo info;
    memset(&info,0,sizeof(RtspServer::MetaInfo));
    info.width  = width;
    info.height = height;
    info.format = format;
    info.type   = type;

    return mRtspServer->prepare(&info);
}

void StreamPusher::sendData(int dma_fd,int size,int width,int height,void *vir_addr)
{
    mLock.lock();
    mDmaBo.buf_fd = dma_fd;
    mDmaBo.width = width;
    mDmaBo.height = height;
    mDmaBo.buf_size = size;
    mDmaBo.vir_addr = vir_addr;
    mRtspServer->setData(mDmaBo);
    mLock.unlock();
}

bool StreamPusher::start()
{
    return mRtspServer->start();
}

void StreamPusher::stop()
{
    mRtspServer->stop();
}

std::string StreamPusher::getUrl()
{
    if(mRtspServer) {
        return mRtspServer->getUrl();
    }

    return "";
}
