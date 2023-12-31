#ifndef THUMBNAILUTILS_H
#define THUMBNAILUTILS_H

#include "common/Mutex.h"
#include "common/common.h"
#include "common/log.h"
#include "common/rthread.h"
#include <QImage>
#include <QPixmap>
#include "media/rkmpp_dec_api.h"
extern "C"{
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}
class ThumbnailUtils : public RThread
{
    Q_OBJECT
public:
    ThumbnailUtils();

    ~ThumbnailUtils();

    virtual void run();
    void setDataSource(QList<QString> files);
    void setThumbSize(int width,int height);
signals:
    void onGetOneImage(QPixmap pixmap,QString file_path);

private:
    typedef enum Type {
        Type_Unkown,
        Type_Image,
        Type_Video
    }Type_t;

    Type getType(const char *suffix);
    void getThumbnail(QString file_path,Type type);

private:
    QList<QString> mFileslist;
    QSize mThumbSize;
    Mutex mLock;
    RKHWDecApi mDecoder;
    AVFormatContext *mFormatCtx;
};

#endif // THUMBNAILUTILS_H
