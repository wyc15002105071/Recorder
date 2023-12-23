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
    virtual bool startTask();
    virtual void stopTask();
    void getImages(QList<QString> file_path_list);
signals:
    void onGetOneImage(QImage image);
};

#endif // THUMBNAILUTILS_H
