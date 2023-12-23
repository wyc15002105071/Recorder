#ifndef RKHWDECAPI_H
#define RKHWDECAPI_H

#include "rk_mpi.h"
#include "mpp_mem.h"
#include "mpp_log.h"
#include "mpp_env.h"

class RKHWDecApi
{
public:
    RKHWDecApi();

    void prepare(int32_t width,int32_t height,MppCodingType type);
    void release();
    void sendPacket(char *data, int32_t size, int64_t pts, int32_t flag);
    void getOutFrame();

private:

};

#endif // RKHWDECAPI_H
