#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <vector>

class Allocator
{
public:
    Allocator();
    virtual ~Allocator();
    virtual int allocBuffers(int width,int height,int fourcc,bool is_contig,bool is_cachable,int count) = 0;
    virtual void getBuffer(int &buf_fd, int &buf_size,void **buf_vir_addr,int buf_index) = 0;
    virtual void destroyBuffers() = 0;
    virtual int getBufferCounts() { return mBufferObjects.size();}
protected:
    typedef enum {
        BUFFER_TYPE_DRM,
        //ToDo
    }BufferType;

    typedef struct {
    public:
        int drm_buffer_fd;
        unsigned int drm_buffer_handle;
        int actual_size;
        unsigned char *drm_buf;
        int width;
        int height;
        int fourcc;
        int stride;
    }BufferObject;

    std::vector<BufferObject *>mBufferObjects;
};

#endif // ALLOCATOR_H
