#ifndef DRM_ALLOCATOR_H
#define DRM_ALLOCATOR_H

#include "allocator.h"

class DrmAllocator:public Allocator
{
public:
    DrmAllocator();
    ~DrmAllocator();
    virtual int allocBuffers(int width,int height,int fourcc,bool is_contig,bool is_cachable,int count);
    virtual void getBuffer(int &buf_fd,int &buf_size,void **buf_vir_addr,int buf_index);
    virtual void destroyBuffers();
private:
    int drm_init();
    void drm_deinit();
    int fourcc2bpp(int fourcc);
private:
    int mDrmFd;
};

#endif // DRM_ALLOCATOR_H
