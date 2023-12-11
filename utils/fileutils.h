#ifndef FILEUTILS_H
#define FILEUTILS_H

#include "common/common.h"
#include "common/rthread.h"
#include "common/observer.h"
#include <QList>

class FileUtils : public RThread,
                  public Observer
{
public:
    enum Operation {
        COPY    = 0,
        DELETE  = 1
    }Operation_t;

    FileUtils();
    ~FileUtils();

    virtual void run();
    virtual bool startTask();
    virtual void stopTask();
    void startCopy(QList<QString> &filesPath,QString dst_dir);
    void startDelete(QList<QString> &filesPath);
    int64_t  getFileSize(QString filePath);
    int64_t  getFilesSize(QList<QString> fileslist);
private:
#define BUFFER_SIZE 4096
    QList<QString> mSrcFilesPath;
    QString mDstDir;

    int mOperation;
private:

};

#endif // FILEUTILS_H
