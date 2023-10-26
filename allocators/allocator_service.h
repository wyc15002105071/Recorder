#ifndef ALLOCATORSERVICE_H
#define ALLOCATORSERVICE_H

#include "drm_allocator.h"

class AllocatorService
{
public:
    static Allocator *getDrmAllocator() {
        static DrmAllocator drm_alloc;
        return &drm_alloc;
    }
private:
    AllocatorService();
};

#endif // ALLOCATORSERVICE_H
