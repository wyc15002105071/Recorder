#ifndef H264LIVEVIDEOSOURCE_H
#define H264LIVEVIDEOSOURCE_H

#include "FramedSource.hh"
#include "media/rkmpp_enc_api.h"
#include "common/common.h"

class H264LiveVideoSource: public FramedSource
{
public:
    static H264LiveVideoSource* createNew(UsageEnvironment& env, void *listener,void *vencoder);
protected:
    H264LiveVideoSource(UsageEnvironment& env, void *listener,void *vencoder);
    // called only by createNew()

    virtual ~H264LiveVideoSource();

    virtual unsigned maxFrameSize() const
    {
        return 512*1024; //根据实际情况设置一个合理的值
    }

private:
    // redefined virtual functions:
    virtual void doGetNextFrame();

private:
    Boolean	fHasTriggerKeyFrame;
    int fSendHeaderCount;
    //char *fTruncatedBytes;
    unsigned int fTruncatedBytesNum;

    void *fListener;
    void *venc;
    int  mVideoStream;
    bool mFirst_stream = true;
    FILE *test = nullptr;
};
#endif // H264LIVEVIDEOSOURCE_H
