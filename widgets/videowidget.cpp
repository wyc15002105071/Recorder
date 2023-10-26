#define GL_GLEXT_PROTOTYPES 1
#include "videowidget.h"
#include "log.h"
#include <QResource>
#include <GLES2/gl2ext.h>
#include "drm_fourcc.h"
#include "media_utils.h"

#define MODULE_TAG "VideoWidget"

VideoWidget::VideoWidget(QWidget *parent)
    :QOpenGLWidget(parent)
    ,mProgram(0)
    ,mVbo(0)
    ,mCurrentPreviewIndex(0)
    ,mSnapShotFrameCount(0)
    ,mIsShowSnapShotEffect(false)
    ,mAllocator(nullptr)
    ,useExternalOES(false)

{

}

VideoWidget::~VideoWidget()
{
    if(mAllocator)
        mAllocator->destroyBuffers();
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
    if(useExternalOES) {
        DEF_VAL = "SHADER_VARIANT_EXTERNAL";
    } else {
        DEF_VAL = "SHADER_VARIANT_RGB";
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

    mDisplay = eglGetCurrentDisplay();
    for(int i=0;i<MAX_BUF_CNT;i++) {
        mEGLimage[i] = createEGLImage(mBo[i].dmabuf_fd, mBo[i].fourcc,
                                      mBo[i].width, mBo[i].height,
                                      mBo[i].stride, 0);
        if(mEGLimage[i] == GL_NONE) {
            RLOGE("createEGLImage failed");
            continue;
        }

        RLOGE("createEGLImage success %d",i);

        glGenTextures(1, &mImageTextureId[i]);
        glBindTexture(GL_TEXTURE_EXTERNAL_OES, mImageTextureId[i]);
        glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glEGLImageTargetTexture2DOES(GL_TEXTURE_EXTERNAL_OES, (GLeglImageOES)mEGLimage[i]);
    }

    inFile = fopen("/userdata/render_in.bin","wb");
}

void VideoWidget::paintGL()
{
    printFps("Display");
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, mImageTextureId[mCurrentPreviewIndex]);

    if(mIsShowSnapShotEffect && mSnapShotFrameCount < 5) {
        mSnapShotFrameCount++;
        if(mSnapShotFrameCount >= 5) {
            mIsShowSnapShotEffect = false;
            mSnapShotFrameCount = 0;
        }
        glClearColor(0.5f,0.5f,0.5f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        return;

    }else {
        if(useExternalOES) {
            glUniform1i(glGetUniformLocation(mProgram, "s_TextureFmt"),8);
            glUniform1i(glGetUniformLocation(mProgram, "tex"),0);
        }
        else if(mBo[mCurrentPreviewIndex].fourcc == DRM_FORMAT_RGB888 || mBo[mCurrentPreviewIndex].fourcc == DRM_FORMAT_BGR888) {
            glUniform1i(glGetUniformLocation(mProgram, "s_TextureFmt"),2);
            glUniform1i(glGetUniformLocation(mProgram, "tex"),0);
            updateTexture(0,mBo[mCurrentPreviewIndex].width,mBo[mCurrentPreviewIndex].height,(uchar *)mBo[mCurrentPreviewIndex].vir_addr,GL_RGB);
        }
        else if(mBo[mCurrentPreviewIndex].fourcc == DRM_FORMAT_NV12 || mBo[mCurrentPreviewIndex].fourcc == DRM_FORMAT_NV21) {
            glUniform1i(glGetUniformLocation(mProgram, "tex1"),1);
            glUniform1i(glGetUniformLocation(mProgram, "s_TextureFmt"),0);

            static int count = 0;
            if(count < 10)
                fwrite(mBo[mCurrentPreviewIndex].vir_addr,1,mBo[mCurrentPreviewIndex].buf_size,inFile);
            count++;

            updateTexture(0,mBo[mCurrentPreviewIndex].width,mBo[mCurrentPreviewIndex].height,(uchar *)mBo[mCurrentPreviewIndex].vir_addr,GL_LUMINANCE);

            updateTexture(1,mBo[mCurrentPreviewIndex].width/2,mBo[mCurrentPreviewIndex].height/2,(uchar *)mBo[mCurrentPreviewIndex].vir_addr+mBo[mCurrentPreviewIndex].width*mBo[mCurrentPreviewIndex].height,GL_LUMINANCE_ALPHA);
        }
    }
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
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
    mCurrentPreviewIndex = buf_index;
    update();
}

void VideoWidget::showSnapShotEffect()
{
    if(mIsShowSnapShotEffect)
        return;
    mIsShowSnapShotEffect = true;

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
    GLint fmt = format;
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

EGLImageKHR VideoWidget::createEGLImage(int fd, int fourcc, int w, int h,int stride, int offset)
{
    RLOGD("fd:%d,width:%d,height:%d,stride:%d",fd,w,h,stride);
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
        RLOGD("fourcc is NV12");attr = attr_nv12;
        break;
    case DRM_FORMAT_YUV420:
    case DRM_FORMAT_YVU420:
        attr = attr_yuv420;
        break;
    default:
        RLOGD("fourcc is packet"); attr = attr_packed;
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
