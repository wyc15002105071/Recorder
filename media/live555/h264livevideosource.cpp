#include "h264livevideosource.h"
#include "rtspserver.h"
#include "common/log.h"

FILE *test = nullptr;

H264LiveVideoSource* H264LiveVideoSource::createNew(UsageEnvironment& env, void *listener, void *vencoder)
{
    return new H264LiveVideoSource(env, listener,vencoder);
}

H264LiveVideoSource::H264LiveVideoSource(UsageEnvironment& env, void *listener,void *vencoder)
    : FramedSource(env) {
    env << "H264LiveVideoSource::H264LiveVideoSource" << "\n";
    fHasTriggerKeyFrame = False;
    fSendHeaderCount = 1;
    fTruncatedBytesNum = 0;
    mFirst_stream = true;
    // setup callback listener
    fListener = listener;
    venc = vencoder;

    if(test) {
        fclose(test);
        test = nullptr;
    }
    test = fopen("/userdata/test.264","wb");

}

H264LiveVideoSource::~H264LiveVideoSource()
{
    printf("destructor\n");
}

void H264LiveVideoSource::doGetNextFrame()
{
    RtspServer *server = (RtspServer*)fListener;
    RKHWEncApi *vencoder = (RKHWEncApi *)venc;
    int ret = 0;
    RKHWEncApi::EncoderOut encOut;
    DmaBufferObject dmabo;
    void *data;
    memset(&dmabo,0,sizeof(DmaBufferObject));
    server->getData(dmabo);

    RKHWEncApi::DmaBuffer_t dma_buffer;
    dma_buffer.dma_fd = dmabo.buf_fd;
    dma_buffer.size   = dmabo.buf_size;
    dma_buffer.width  = dmabo.width;
    dma_buffer.height = dmabo.height;

    if(dma_buffer.dma_fd <= 0 || dma_buffer.size <= 0) {
        goto END;
    }
    vencoder->sendFrame(dma_buffer,0,0);
    ret = vencoder->getOutPacket(&encOut);

    data = mpp_packet_get_data(encOut.packet);

    fFrameSize = encOut.size;
    if (fFrameSize > fMaxSize) {
        RLOGD("size %d,max size %d",fFrameSize,fMaxSize);
        fNumTruncatedBytes = fFrameSize - fMaxSize;
        fFrameSize = fMaxSize;

    } else {
        fNumTruncatedBytes = 0;
    }

    memcpy(fTo, data, fFrameSize);

    if(test) {
        fwrite(fTo,1,fFrameSize,test);
    }

    gettimeofday(&fPresentationTime, NULL);
    encOut.release();
//    usleep(17*1000);
END:
    FramedSource::afterGetting(this);
}

