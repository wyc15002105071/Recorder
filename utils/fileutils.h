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
    static const char *getFileSuffix(QString file_path);
    static const char *pathToName(QString path);
    static const char *pathToDirPath(QString path);
    static void mkdirIfNotExit(QString dir);
    void startCopy(QList<QString> &filesPath,QString dst_dir);
    void startDelete(QList<QString> &filesPath);
    long long getFileSize(QString filePath);
    long long getFilesSize(QList<QString> fileslist);
	long long getDirSize(QString dirPath);
private:
#define BUFFER_SIZE 4096
    QList<QString> mSrcFilesPath;
    QString mDstDir;

    int mOperation;
private:

};

#endif // FILEUTILS_H
