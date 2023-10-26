#ifndef THUMBNAILUTILS_H
#define THUMBNAILUTILS_H

#include "rthread.h"
#include <list>
#include "FFmpegMediaMetadataRetriever/ffmpeg_mediametadataretriever.h"
#include "FFmpegMediaMetadataRetriever/ffmpeg_utils.h"
#include <memory>

class ThumbnailUtils:public RThread
{
public:
    ThumbnailUtils();
    virtual void run();
    virtual void stopTask();
    virtual bool startTask();

    static Metadata getThumbnail(const char *file_path);
protected:
    std::list<std::string>mFilePathlist;
};

#endif // THUMBNAILUTILS_H
