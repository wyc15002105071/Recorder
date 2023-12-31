#ifndef THUMBNAILUTILS_H
#define THUMBNAILUTILS_H

#include "common/Mutex.h"
#include "common/common.h"
#include "common/log.h"
#include "common/rthread.h"
#include <QImage>

class ThumbnailUtils : public RThread
{
    Q_OBJECT
public:
    ThumbnailUtils();

    ~ThumbnailUtils();

    virtual void run();
    void setDataSource(QList<QString> files);
signals:
    void onGetOneImage(QImage image,QString file_path);

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
    Mutex mLock;
};

#endif // THUMBNAILUTILS_H
