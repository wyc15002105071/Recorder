#define GL_GLEXT_PROTOTYPES 1
#include "videowidget.h"
#include "log.h"
#include <QResource>
#include <GLES2/gl2ext.h>
#include "drm_fourcc.h"
#include "mediautils.h"

#define MODULE_TAG "VideoWidget"

VideoWidget::VideoWidget(QWidget *parent)
    :QOpenGLWidget(parent)
    ,mProgram(0)
    ,mVbo(0)
    ,mBufId(-1)
    ,mSnapShotFrameCount(0)
    ,mIsShowSnapShotEffect(false)
    ,mAllocator(nullptr)
    ,mUseExternalOES(true)
    ,mDisplay(0)

{

}

VideoWidget::~VideoWidget()
{
    if(mAllocator)
        mAllocator->destroyBuffers();

    if (mProgram)
    {
        glDeleteProgram(mProgram);
    }

//    if (m_FboProgramObj)
//    {
//        glDeleteProgram(m_FboProgramObj);
//    }

    for(int i = 0;i< MAX_BUF_CNT;i++) {
        if (mImageTextureId[i])
        {
            glDeleteTextures(1, &mImageTextureId[i]);
        }
    }

//    if (m_FboTextureId)
//    {
//        glDeleteTextures(1, &m_FboTextureId);
//    }

    if (mVbo)
    {
        glDeleteBuffers(1, &mVbo);
    }

//    if (m_VaoIds[0])
//    {
//        glDeleteVertexArrays(1, m_VaoIds);
//    }

//    if (m_FboId)
//    {
//        glDeleteFramebuffers(1, &m_FboId);
//    }

    for(int i = 0;i < MAX_BUF_CNT;i++) {
        if (mEGLimage[i]) {
            eglDestroyImageKHR(mDisplay, mEGLimage[i]);
            mEGLimage[i] = nullptr;
        }
    }

//    if (m_dstEglImage) {
//        eglDestroyImageKHR(eglGetCurrentDisplay(), m_dstEglImage);
//    }
}

void VideoWidget::initializeGL()
{
    initializeOpenGLFunctions();

    for(int i=0;i<MAX_BUF_CNT;i++) {
        memset(&mImageTextureId[i],0,sizeof(GLuint));
        memset(&mTexts[i],0,sizeof(GLuint));
        memset(&mEGLimage[i],0,sizeof(EGLImageKHR));
    }

    QResource vsh_res(":/resources/shaders/vertex.sh");
    QResource fsh_res(":/resources/shaders/fragment.sh");

    char sources[1024] = {0};
    const char *DEF_VAL;

    if(mUseExternalOES) {
        DEF_VAL     = "SHADER_VARIANT_EXTERNAL";
        mTextarget  = GL_TEXTURE_EXTERNAL_OES;
    } else {
        DEF_VAL     = "SHADER_VARIANT_RGB";
        mTextarget  = GL_TEXTURE_2D;
    }

    sprintf(sources,"#define DEF_VARIANT %s\n",DEF_VAL);
    char f_shader[1024*4] = {0};
    strcat(f_shader,sources);
    strcat(f_shader,(const char*)fsh_res.data());

    GLuint vsh = CompileShader(GL_VERTEX_SHADER,(char *)vsh_res.data());
    if(vsh == 0)
    {
        RLOGE("CompileShader GL_VERTEX_SHADER failed!");
        return ;
    }

    GLuint fsh = CompileShader(GL_FRAGMENT_SHADER,f_shader);
    if(fsh == 0)
    {
        RLOGE("CompileShader GL_FRAGMENT_SHADER failed!");
        return;
    }

    RLOGD("CompileShader success!");
    mProgram = 0;
    mProgram = glCreateProgram();
    if(mProgram <= 0)
    {
        RLOGE("glCreateProgram failed!");
        return;
    }
    glAttachShader(mProgram,vsh);
    glAttachShader(mProgram,fsh);
    glLinkProgram(mProgram);
    GLint state;
    glGetProgramiv(mProgram,GL_LINK_STATUS,&state);
    if(state == 0)
    {
        RLOGE("glLinkProgram failed!");
        return;
    }
    RLOGE("glLinkProgram success!");
    glDeleteShader(vsh);
    glDeleteShader(fsh);

    float vertexes[] = {
        -1.0f,-1.0f,0.0f,0.0f,1.0f,
        1.0f,-1.0f,0.0f,1.0f,1.0f,
        -1.0f,1.0f,0.0f,0.0f,0.0f,
        1.0f,1.0f,0.0f,1.0f,0.0f
    };
    glGenBuffers(1,&mVbo);
    glBindBuffer(GL_ARRAY_BUFFER,mVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes),vertexes,GL_STATIC_DRAW);

    GLint index = glGetAttribLocation(mProgram,"aPos");
    glVertexAttribPointer(index,3,GL_FLOAT,GL_FALSE, sizeof(GL_FLOAT)*5,(void *)0);
    glEnableVertexAttribArray(index);

    index = glGetAttribLocation(mProgram,"ayTexCoords");

    glVertexAttribPointer(index,2,GL_FLOAT,GL_FALSE, sizeof(GL_FLOAT)*5,(void *)(3* sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(index);
    glUseProgram(mProgram);
    glClearColor(0.0f,0.0f,0.0f,1.0f);

    connect(this,SIGNAL(onStartUpdate()),this,SLOT(update()),Qt::UniqueConnection);
    onVideoWidgetCreated();
}

void VideoWidget::paintGL()
{
    mLock.lock();
    if(mBufId < 0 || (mBo[mBufId].width < 0 || mBo[mBufId].height < 0)) {
        mLock.unlock();
        return;
    }

    printFps("Display");
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if(mIsShowSnapShotEffect && mSnapShotFrameCount < 5) {
        mSnapShotFrameCount++;
        if(mSnapShotFrameCount >= 5) {
            mIsShowSnapShotEffect = false;
            mSnapShotFrameCount = 0;
        }
        glClearColor(0.5f,0.5f,0.5f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        mLock.unlock();
        return;
    }else {
        if(mUseExternalOES) {
            glUniform1i(glGetUniformLocation(mProgram, "s_TextureFmt"),8);
            glUniform1i(glGetUniformLocation(mProgram, "tex"),0);
            updateTexture(0,mBo[mBufId].width,mBo[mBufId].height,nullptr,GL_RGB);
            glBindTexture(GL_TEXTURE_EXTERNAL_OES, mImageTextureId[mBufId]);
        }
        else if(mBo[mBufId].fourcc == DRM_FORMAT_RGB888 || mBo[mBufId].fourcc == DRM_FORMAT_BGR888) {
            glUniform1i(glGetUniformLocation(mProgram, "s_TextureFmt"),2);
            glUniform1i(glGetUniformLocation(mProgram, "tex"),0);
            updateTexture(0,mBo[mBufId].width,mBo[mBufId].height,(uchar *)mBo[mBufId].vir_addr,GL_RGB);
        }
        else if(mBo[mBufId].fourcc == DRM_FORMAT_NV12 || mBo[mBufId].fourcc == DRM_FORMAT_NV21) {
            glUniform1i(glGetUniformLocation(mProgram, "tex1"),1);
            glUniform1i(glGetUniformLocation(mProgram, "s_TextureFmt"),0);
            updateTexture(0,mBo[mBufId].width,mBo[mBufId].height,(uchar *)mBo[mBufId].vir_addr,GL_LUMINANCE);

            updateTexture(1,mBo[mBufId].width/2,mBo[mBufId].height/2,(uchar *)mBo[mBufId].vir_addr+mBo[mBufId].width*mBo[mBufId].height,GL_LUMINANCE_ALPHA);
        }
    }
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    mLock.unlock();
    usleep(1*350);
    glFinish();
//    if(mUseExternalOES) {
//        eglDestroyImageKHR(eglGetCurrentDisplay(), mEGLimage[mBufId]);
//    }
}

void VideoWidget::resizeGL(int w, int h)
{

}

int VideoWidget::allocBuffers(int w, int h, int fourcc,int is_contig,int cachable,int count)
{
    int ok = 0,error = -1;
    int ret;
    if(!mAllocator)
        mAllocator = AllocatorService::getDrmAllocator();
    if(!mAllocator) {
        RLOGE("get Drm Allocator failed!");
        return error;
    }

    ret = mAllocator->allocBuffers(w,h,fourcc,is_contig,cachable,count);
    if(ret != ok) {
        RLOGE("allocBuffers failed!");
        return -1;
    }

    int stride = 0;
    switch(fourcc) {
    case DRM_FORMAT_RGB888:
    case DRM_FORMAT_BGR888:
        stride = w*3;
        break;
    case DRM_FORMAT_RGBA8888:
    case DRM_FORMAT_BGRA8888:
        stride = w*4;
        break;
    case DRM_FORMAT_NV12:
    case DRM_FORMAT_NV21:
        stride = w*3/2;
        break;
    default:
        stride = w;
    }

    for(int i = 0;i<MAX_BUF_CNT;i++) {
        memset(&mBo[i],0,sizeof(BufferObject));
        mBo[i].width = w;
        mBo[i].height = h;
        mBo[i].fourcc = fourcc;
        mBo[i].stride = stride;
        if(fourcc == DRM_FORMAT_NV12 || fourcc == DRM_FORMAT_NV21)
            mBo[i].stride = w;
        mAllocator->getBuffer(mBo[i].dmabuf_fd,mBo[i].buf_size,(void **)&mBo[i].vir_addr,i);
        RLOGD("get buffer%d info,width:%d,height:%d,size:%d",i,mBo[i].width,mBo[i].height,mBo[i].buf_size);
    }

    mDisplay = eglGetCurrentDisplay();
    for(int i=0;i<MAX_BUF_CNT;i++) {
        //        mEGLimage[i] = createEGLImage(mBo[i].dmabuf_fd, mBo[i].fourcc,
        //                                      mBo[i].width, mBo[i].height,
        //                                      mBo[i].stride, 0);
        //        if(mEGLimage[i] == GL_NONE) {
        //            RLOGE("createEGLImage failed");
        //            continue;
        //        }

        //        RLOGE("createEGLImage success %d",i);

        glGenTextures(1, &mImageTextureId[i]);
        glBindTexture(mTextarget, mImageTextureId[i]);
        glTexParameterf(mTextarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(mTextarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(mTextarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(mTextarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(mTextarget, 0);
        //        glEGLImageTargetTexture2DOES(GL_TEXTURE_EXTERNAL_OES, (GLeglImageOES)mEGLimage[i]);
    }

    return ok;
}

void VideoWidget::getBufferInfo(int &buf_fd, int &buf_size,void **buf_vir_addr,int buf_index)
{
    if(!mAllocator)
        mAllocator = AllocatorService::getDrmAllocator();
    return mAllocator->getBuffer(buf_fd,buf_size,buf_vir_addr,buf_index);
}

void VideoWidget::PrepareUpdate(int buf_index)
{
    //    glBindTexture(GL_TEXTURE_EXTERNAL_OES, mImageTextureId[buf_index]);
    mLock.lock();
    mBufId = buf_index;
    mLock.unlock();
    onStartUpdate();
//    update();
}

void VideoWidget::showSnapShotEffect()
{
    if(mIsShowSnapShotEffect)
        return;
    mIsShowSnapShotEffect = true;

}

void VideoWidget::importDmaBuffer(int buf_id, int buf_fd, int width, int height, uint32_t format, int buf_size, void *vir_addr)
{
    if(buf_id < 0 || buf_fd < 0)
        return;
    mBo[buf_id].dmabuf_fd   = buf_fd;
    mBo[buf_id].width       = width;
    mBo[buf_id].height      = height;
    mBo[buf_id].fourcc      = getDrmFourcc(format);
    mBo[buf_id].buf_size    = buf_size;
    mBo[buf_id].vir_addr    = vir_addr;
    mBo[buf_id].stride      = getStride(mBo[buf_id].width,mBo[buf_id].height,mBo[buf_id].fourcc);
}

void VideoWidget::reset()
{
    for(int i = 0;i<MAX_BUF_CNT;i++) {
        memset(&mBo[i],0,sizeof(BufferObject));
    }
    for(int i = 0;i< MAX_BUF_CNT;i++) {
        if (mImageTextureId[i])
        {
            glDeleteTextures(1, &mImageTextureId[i]);
        }
    }
    for(int i = 0;i < MAX_BUF_CNT;i++) {
        if (mEGLimage[i]) {
            eglDestroyImageKHR(mDisplay, mEGLimage[i]);
            mEGLimage[i] = nullptr;
        }
    }
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

GLuint VideoWidget::CompileShader(GLenum ShaderType, const char *shaderCode)
{
    GLuint shader = glCreateShader(ShaderType);
    if (shader == 0)
    {
        printf("glCreateShader failed\n");
        return 0;
    }

    if (!shaderCode)
    {
        printf("Load shader failed\n");
        glDeleteShader(shader);
        return 0;
    }
    glShaderSource(shader, 1, &shaderCode, nullptr);
    glCompileShader(shader);
    GLint compileResult = GL_TRUE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
    if (compileResult == GL_FALSE)
    {
        char LogInfo[1024] = { 0 };
        GLsizei logLen;
        glGetShaderInfoLog(shader, 1024, &logLen, LogInfo);
        RLOGE("%s",LogInfo);
        glDeleteShader(shader);
        shader = 0;
    }

    return shader;
}

void VideoWidget::updateTexture(unsigned int index, int width, int height,
                                unsigned char *buf, GLenum format)
{
    if(mUseExternalOES) {
        if(mBufId < 0 || mBo->dmabuf_fd <= 0)
            return;
        if(!mDisplay)
            mDisplay = eglGetCurrentDisplay();
        int32_t i = mBufId;
        glBindTexture(mTextarget, mImageTextureId[i]);
        if(!mEGLimage[i]) {
            RLOGD("dma %d width %d height %d fourcc %d stride %d",mBo[i].dmabuf_fd,mBo[i].width
                                                                ,mBo[i].height,mBo[i].fourcc,mBo[i].stride);
            mEGLimage[i] = createEGLImage(mBo[i].dmabuf_fd, mBo[i].fourcc,
                                          mBo[i].width, mBo[i].height,
                                          mBo[i].stride, 0);
            if(mEGLimage[i] == GL_NONE) {
                RLOGE("createEGLImage %d failed",i);
                return;
            }
        }

        glEGLImageTargetTexture2DOES(mTextarget, (GLeglImageOES)mEGLimage[i]);
        glBindTexture(mTextarget, 0);
    } else {
        if(mTexts[index] == 0)
        {
            glGenTextures(1,&mTexts[index]);

            glBindTexture(GL_TEXTURE_2D,mTexts[index]);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         format,
                         width,height,
                         0,
                         format,
                         GL_UNSIGNED_BYTE,
                         NULL);

        }

        glActiveTexture(GL_TEXTURE0+index);
        glBindTexture(GL_TEXTURE_2D,mTexts[index]);
        glTexSubImage2D(GL_TEXTURE_2D,0,0,0,width,height,format,GL_UNSIGNED_BYTE,buf);
    }
}

EGLImageKHR VideoWidget::createEGLImage(int fd, int fourcc, int w, int h,int stride, int offset)
{
    EGLint error;
    EGLImageKHR img;
    EGLint attr_packed[] = {
        EGL_WIDTH, w,
        EGL_HEIGHT, h,
        EGL_LINUX_DRM_FOURCC_EXT, fourcc,
        EGL_DMA_BUF_PLANE0_FD_EXT, fd,
        EGL_DMA_BUF_PLANE0_OFFSET_EXT, offset,
        EGL_DMA_BUF_PLANE0_PITCH_EXT, stride,
        EGL_NONE
    };

    EGLint attr_nv12[] = {
        EGL_WIDTH, w,
        EGL_HEIGHT, h,
        EGL_LINUX_DRM_FOURCC_EXT, fourcc,
        EGL_DMA_BUF_PLANE0_FD_EXT, fd,
        EGL_DMA_BUF_PLANE0_OFFSET_EXT, offset,
        EGL_DMA_BUF_PLANE0_PITCH_EXT, stride,
        EGL_DMA_BUF_PLANE1_FD_EXT, fd,
        EGL_DMA_BUF_PLANE1_OFFSET_EXT, offset + h * stride,
        EGL_DMA_BUF_PLANE1_PITCH_EXT, stride,
        EGL_NONE
    };

    EGLint attr_nv16[] = {
          EGL_WIDTH, w,
          EGL_HEIGHT, h,
          EGL_LINUX_DRM_FOURCC_EXT, fourcc,
          EGL_DMA_BUF_PLANE0_FD_EXT, fd,
          EGL_DMA_BUF_PLANE0_OFFSET_EXT, offset,
          EGL_DMA_BUF_PLANE0_PITCH_EXT, stride,
          EGL_DMA_BUF_PLANE1_FD_EXT, fd,
          EGL_DMA_BUF_PLANE1_OFFSET_EXT, offset + h * stride,
          EGL_DMA_BUF_PLANE1_PITCH_EXT, stride,
          EGL_NONE
      };
    EGLint attr_nv24[] = {
        EGL_WIDTH, w,
        EGL_HEIGHT, h,
        EGL_LINUX_DRM_FOURCC_EXT, fourcc,
        EGL_DMA_BUF_PLANE0_FD_EXT, fd,
        EGL_DMA_BUF_PLANE0_OFFSET_EXT, offset,
        EGL_DMA_BUF_PLANE0_PITCH_EXT, stride,
        EGL_DMA_BUF_PLANE1_FD_EXT, fd,
        EGL_DMA_BUF_PLANE1_OFFSET_EXT, offset + h * stride,
        EGL_DMA_BUF_PLANE1_PITCH_EXT, stride,
        EGL_NONE
    };

    EGLint attr_yuv420[] = {
        EGL_WIDTH, w,
        EGL_HEIGHT, h,
        EGL_LINUX_DRM_FOURCC_EXT, fourcc,
        EGL_DMA_BUF_PLANE0_FD_EXT, fd,
        EGL_DMA_BUF_PLANE0_OFFSET_EXT, offset,
        EGL_DMA_BUF_PLANE0_PITCH_EXT, stride,
        EGL_DMA_BUF_PLANE1_FD_EXT, fd,
        EGL_DMA_BUF_PLANE1_OFFSET_EXT, offset + h * stride,
        EGL_DMA_BUF_PLANE1_PITCH_EXT, stride,
        EGL_DMA_BUF_PLANE2_FD_EXT, fd,
        EGL_DMA_BUF_PLANE2_OFFSET_EXT, offset + h * stride + w / 2,
        EGL_DMA_BUF_PLANE2_PITCH_EXT, stride,
        EGL_NONE
    };

    EGLint *attr;
    switch (fourcc) {
    case DRM_FORMAT_NV12:
    case DRM_FORMAT_NV21:
        attr = attr_nv12;
        break;
    case DRM_FORMAT_NV61:
    case DRM_FORMAT_NV16:
        attr = attr_nv16;
        break;
    case DRM_FORMAT_NV24:
    case DRM_FORMAT_NV42:
        attr = attr_nv24;
        break;
    case DRM_FORMAT_YUV420:
    case DRM_FORMAT_YVU420:
        attr = attr_yuv420;
        break;
    default:
        attr = attr_packed;
        break;
    }

    PFNEGLCREATEIMAGEKHRPROC eglCreateImageKHR = (PFNEGLCREATEIMAGEKHRPROC)eglGetProcAddress("eglCreateImageKHR");
    img = eglCreateImageKHR(mDisplay, EGL_NO_CONTEXT,
                            EGL_LINUX_DMA_BUF_EXT, (EGLClientBuffer)0, attr);
    EGLint err = eglGetError();
    getEGLError(err);
    if(img == GL_NONE) {
        RLOGE("img is null");
    }
    return img;
}

int VideoWidget::getStride(int width, int height, int fourcc)
{
    if(width * height == 0)
        return width;
    int stride = width;
    switch(fourcc) {
    case DRM_FORMAT_NV12:
    case DRM_FORMAT_NV21:
    case DRM_FORMAT_NV16:
    case DRM_FORMAT_NV61:
    case DRM_FORMAT_NV24:
    case DRM_FORMAT_NV42:
        break;
    case DRM_FORMAT_BGR888:
    case DRM_FORMAT_RGB888:
        stride = width * 3;
        break;
    case DRM_FORMAT_BGRA8888:
    case DRM_FORMAT_RGBA8888:
        stride = width * 4;
        break;
    default:break;
    }

    return stride;
}

int VideoWidget::getDrmFourcc(uint32_t format)
{
    int drm_fmt = DRM_FORMAT_RGB888;
    const char *pixel_fmt = fcc2s(format).c_str();
    if(!strcmp(pixel_fmt,"BGR3") ||  !strcmp(pixel_fmt,"RGB3")) {
        drm_fmt = DRM_FORMAT_RGB888;
    }
    else if(!strcmp(pixel_fmt,"NV12")) {
        drm_fmt = DRM_FORMAT_NV12;
    }
    else if(!strcmp(pixel_fmt,"NV21")) {
        drm_fmt = DRM_FORMAT_NV21;
    }
    else if(!strcmp(pixel_fmt,"NV16")) {
        drm_fmt = DRM_FORMAT_NV16;
    }
    else if(!strcmp(pixel_fmt,"NV61")) {
        drm_fmt = DRM_FORMAT_NV61;
    }
    else if(!strcmp(pixel_fmt,"NV24")) {
        drm_fmt = DRM_FORMAT_NV24;
    }
    else if(!strcmp(pixel_fmt,"NV42")) {
        drm_fmt = DRM_FORMAT_NV42;
    }

    return drm_fmt;
}

void VideoWidget::getEGLError(EGLint error)
{
    if (error != EGL_SUCCESS) {
        switch (error) {
        case EGL_NOT_INITIALIZED:
            // EGL 尚未初始化
            RLOGE("EGL_NOT_INITIALIZED");
            break;
        case EGL_BAD_ACCESS:
            // 无效的访问，通常是因为权限不足
            RLOGE("EGL_BAD_ACCESS");
            break;
        case EGL_BAD_ALLOC:
            // 内存分配失败
            RLOGE("EGL_BAD_ALLOC");
            break;
        case EGL_BAD_ATTRIBUTE:
            // 无效的属性参数
            RLOGE("EGL_BAD_ATTRIBUTE");
            break;
        case EGL_BAD_CONTEXT:
            // 无效的 EGL 上下文
            RLOGE("EGL_BAD_CONTEXT");
            break;
        case EGL_BAD_CONFIG:
            // 无效的配置
            RLOGE("EGL_BAD_CONFIG");
            break;
        case EGL_BAD_CURRENT_SURFACE:
            // 当前表面无效
            RLOGE("EGL_BAD_CURRENT_SURFACE");
            break;
        case EGL_BAD_DISPLAY:
            // 无效的显示连接
            RLOGE("EGL_BAD_DISPLAY");
            break;
        case EGL_BAD_SURFACE:
            // 无效的表面
            RLOGE("EGL_BAD_SURFACE");
            break;
        case EGL_BAD_MATCH:
            // 无效的匹配，通常是因为 EGLConfig 不匹配
            RLOGE("EGL_BAD_MATCH");
            break;
        case EGL_BAD_PARAMETER:
            // 无效的参数
            RLOGE("EGL_BAD_PARAMETER");
            break;
        case EGL_BAD_NATIVE_PIXMAP:
            // 无效的本地 Pixmap
            RLOGE("EGL_BAD_NATIVE_PIXMAP");
            break;
        case EGL_BAD_NATIVE_WINDOW:
            // 无效的本地窗口
            RLOGE("EGL_BAD_NATIVE_WINDOW");
            break;
        case EGL_CONTEXT_LOST:
            // 上下文丢失
            RLOGE("EGL_CONTEXT_LOST");
            break;
        default:
            // 未知错误
            RLOGE("unknown");
            break;
        }
    }
}
