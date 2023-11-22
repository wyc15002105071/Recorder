#ifndef THUMBNAILUTILS_H
#define THUMBNAILUTILS_H

#include "rthread.h"
#include <list>
#include <memory>

class ThumbnailUtils:public RThread
{
public:
    ThumbnailUtils();
    virtual void run();
    virtual void stopTask();
    virtual bool startTask();

protected:
    std::list<std::string>mFilePathlist;
};

#endif // THUMBNAILUTILS_H
