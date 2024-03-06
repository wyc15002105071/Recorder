#include "drm_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h> // open function
#include <unistd.h>    // close function
#include <errno.h>
#include <sys/mman.h>
#include <linux/input.h>
#include "libdrm/drm_fourcc.h"
#include "xf86drm.h"
#include "rockchip_drm.h"
#include "common/log.h"
#include <sys/mman.h>

#define MAX_BUF_CNT 4
#define MODULE_TAG "DrmAllocator"

DrmAllocator::DrmAllocator()
    :mDrmFd(-1)
{

}

DrmAllocator::~DrmAllocator()
{
    destroyBuffers();
    drm_deinit();
}

int DrmAllocator::allocBuffers(int width, int height, int fourcc, bool is_contig,bool is_cachable,int count)
{
    if(count > MAX_BUF_CNT)
        count = MAX_BUF_CNT;

    int error = -1,ok = 0;;

    if(mBufferObjects.size() >= MAX_BUF_CNT) {
        RLOGE("buffer pool is full!");
        return -1;
    }

    RLOGD("start alloc dma buf from drm,buf count is %d,contig:%d,cachable:%d",count,is_contig,is_cachable);


    for(int i=0;i<count;i++) {

        BufferObject *bo;
        void *vir_addr = NULL;
        char *map = NULL;
        struct drm_mode_map_dumb mmap_arg;
        int ret;

        int bpp = fourcc2bpp(fourcc);
        uint64_t size = width * height * bpp/8;
        uint32_t flags = 0;
        if(is_contig)
            flags |= ROCKCHIP_BO_CONTIG;
        if(is_cachable)
            flags |= ROCKCHIP_BO_CACHABLE;
        struct drm_rockchip_gem_create req = {
            .size = size,
                    .flags = flags,
        };
        if (mDrmFd < 0) {
            mDrmFd = drm_init();
            if(mDrmFd <= 0)
                return error;
        }
        if (drmIoctl(mDrmFd, DRM_IOCTL_ROCKCHIP_GEM_CREATE, &req)){
            RLOGE("failed to create gem object[%s].\n",
                  strerror(errno));
            return error;
        }

        bo = (BufferObject *)malloc(sizeof(BufferObject));
        RLOGD("DRM_IOCTL_ROCKCHIP_GEM_CREATE success,handle is %d\,size is %d,buf index is %d\n"
              ,req.handle,size,i);
        bo->drm_buffer_handle = req.handle;
        bo->actual_size = size;

        struct drm_prime_handle args;

        memset(&args,0,sizeof(struct drm_prime_handle));
        args.fd = -1;
        args.handle = req.handle;
        args.flags = 0;
        ret = drmIoctl(mDrmFd, DRM_IOCTL_PRIME_HANDLE_TO_FD, &args);
        if (ret) {
            RLOGE("failed to DRM_IOCTL_PRIME_HANDLE_TO_FD[%s].\n",
                  strerror(errno));
            return error;
        }

        bo->drm_buffer_fd = args.fd;
        RLOGD("DRM_IOCTL_PRIME_HANDLE_TO_FD success,fd is %d\n",args.fd);

        //获取虚拟地址
        memset(&mmap_arg, 0, sizeof(mmap_arg));
        mmap_arg.handle = args.handle;

        ret = drmIoctl(mDrmFd, DRM_IOCTL_MODE_MAP_DUMB, &mmap_arg);
        if (ret)
        {
            printf("failed to create map dumb: %s\n", strerror(errno));
            vir_addr = NULL;
            goto destory_gem;
        }
        RLOGD("drmIoctl DRM_IOCTL_MODE_MAP_DUMB success!\n");
        vir_addr = map = (char *)mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, mDrmFd, mmap_arg.offset);
        if (map == MAP_FAILED)
        {
            RLOGE("failed to mmap buffer: %s\n", strerror(errno));
            vir_addr = NULL;
            goto destory_gem;
        }

        RLOGD("mmap success!\n");
        bo->drm_buf = (uint8_t *) vir_addr;

        mBufferObjects.push_back(bo);
//        continue;
destory_gem:
        if (bo->drm_buffer_handle) {
            struct drm_gem_close req = {
                .handle = bo->drm_buffer_handle,
            };
            drmIoctl(mDrmFd, DRM_IOCTL_GEM_CLOSE, &req);
        }
    }

    return ok;
}

void DrmAllocator::getBuffer(int &buf_fd, int &buf_size, void **buf_vir_addr,int buf_index)
{
    if(buf_index < 0 || mBufferObjects.size() <= 0 || !mBufferObjects[buf_index]) {
        buf_fd = 0;
        buf_size = 0;
        *buf_vir_addr = nullptr;
        return;
    }
    buf_fd = mBufferObjects[buf_index]->drm_buffer_fd;
    buf_size = mBufferObjects[buf_index]->actual_size;
    *buf_vir_addr = mBufferObjects[buf_index]->drm_buf;
}

void DrmAllocator::destroyBuffers()
{
    int buf_count = getBufferCounts();
    if(buf_count <= 0)
        return;
    for(auto bo:mBufferObjects) {
        if (bo->drm_buffer_handle) {
            struct drm_gem_close req = {
                .handle = bo->drm_buffer_handle,
            };
            drmIoctl(mDrmFd, DRM_IOCTL_GEM_CLOSE, &req);

            if(bo->drm_buf) {
                munmap(bo->drm_buf,bo->actual_size);
            }
            if(bo->drm_buffer_fd > 0) {
                close(bo->drm_buffer_fd);
            }

            free(bo);
        }
    }
    mBufferObjects.clear();
    drm_deinit();
}

int DrmAllocator::drm_init()
{
    static const char *card = "/dev/dri/card0";
    int flag = O_RDWR;
    int drm_fd = -1;

    drm_fd = open(card, flag);
    if (drm_fd < 0)
    {
        RLOGE("failed to open %s\n", card);
        return -1;
    }
    return drm_fd;
}

void DrmAllocator::drm_deinit()
{
    if (mDrmFd > 0)
    {
        close(mDrmFd);
        mDrmFd = -1;
    }
}

int DrmAllocator::fourcc2bpp(int fourcc)
{
    switch (fourcc)
    {
    case DRM_FORMAT_NV12:
    case DRM_FORMAT_NV21:
        return 8*3/2;
    case DRM_FORMAT_RGB888:
    case DRM_FORMAT_BGR888:
        return 8*3;
    case DRM_FORMAT_RGBA8888:
    case DRM_FORMAT_ARGB8888:
    case DRM_FORMAT_ABGR8888:
    case DRM_FORMAT_BGRA8888:
    case DRM_FORMAT_XRGB8888:
    case DRM_FORMAT_XBGR8888:
    case DRM_FORMAT_RGBX8888:
    case DRM_FORMAT_BGRX8888:
        return 8*4;
    default:
        RLOGE("unknown fourcc:%d\n", fourcc);
        break;
    }

    return 0;
}
