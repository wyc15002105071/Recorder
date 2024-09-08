#ifndef STREAMPUSHER_H
#define STREAMPUSHER_H

#include "live555/rtspserver.h"
#include "common/Mutex.h"
#include "common/common.h"
#include "media/mediautils.h"

class StreamPusher
{
public:
    StreamPusher();

    bool prepare(int width,int height,int format,int type);

    void sendData(int dma_fd,int size,int width,int height,void *vir_addr);
    bool start();
    void stop();
    std::string getUrl();
private:
    DmaBufferObject mDmaBo;
    RMutex mLock;
    sp<RtspServer> mRtspServer;
};

#endif // STREAMPUSHER_H
