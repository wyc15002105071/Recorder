#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#define EGL_EGLEXT_PROTOTYPES 1
#include "allocators/allocator_service.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include "common/Mutex.h"

class VideoWidget:public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT
public:
    VideoWidget(QWidget *parent = nullptr);

    virtual ~VideoWidget();

    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);

    int     allocBuffers(int w,int h,int fourcc,int is_contig,int cachable,int count);
    void    getBufferInfo(int &buf_fd,int &buf_size,void **buf_vir_addr,int buf_index);
    void    PrepareUpdate(int buf_index);
    void    showSnapShotEffect();
    void    importDmaBuffer(int buf_id,int buf_fd,int width,int height,uint32_t format,int buf_size,void *vir_addr);
    void    reset();
private:
#define MAX_BUF_CNT 4
    void        getEGLError(EGLint error);
    GLuint      CompileShader(GLenum ShaderType, const char *shaderCode);
    void        updateTexture(unsigned int index, int width, int height,
                                unsigned char *buf, GLenum format);
    EGLImageKHR createEGLImage(int fd, int fourcc, int w, int h,int stride, int offset);
    int         getStride(int width,int height,int fourcc);
    int         getDrmFourcc(uint32_t format);
private:
    EGLImageKHR mEGLimage[MAX_BUF_CNT];
    int         mProgram;
    uint32_t    mVbo;
    uint32_t    mImageTextureId[MAX_BUF_CNT];
    uint32_t    mTexts[MAX_BUF_CNT];
    uint32_t    mTextarget;
    int32_t     mBufId;

    typedef struct {
        int    dmabuf_fd;
        int    buf_size;
        int    fourcc;
        int    width;
        int    height;
        int    stride;
        void  *vir_addr;
    }BufferObject;

    BufferObject mBo[MAX_BUF_CNT];
    EGLDisplay   mDisplay;
    int          mSnapShotFrameCount;
    bool         mIsShowSnapShotEffect;
    Allocator   *mAllocator;
    bool         mUseExternalOES;
    FILE*        inFile;
    Mutex        mLock;
signals:
    void onVideoWidgetCreated();
    void onStartUpdate();
};

#endif // VIDEOWIDGET_H
