#include "thumbnail_utils.h"
#include "utils/fileutils.h"

ThumbnailUtils::ThumbnailUtils()
{

}

ThumbnailUtils::~ThumbnailUtils()
{

}

void ThumbnailUtils::run()
{
    int count = mFileslist.count();
    if(count == 0) {
        return;
    }
    RLOGD("count is %d",count);
    while(!mThreadExit) {
        for(int i = 0;i < count; i++) {
            QString file_path = mFileslist.at(i);
            const char *suffix = FileUtils::getFileSuffix(file_path);
            RLOGD("suffix is %s",suffix);
            Type type = getType(suffix);
            if(type == Type_Unkown) {
                usleep(1*1000);
                continue;
            }

            getThumbnail(file_path,type);
            usleep(1*1000);

            if(mThreadExit)
                break;
        }

        break;
    }
}

void ThumbnailUtils::setDataSource(QList<QString> files)
{
    this->mFileslist = files;
}

ThumbnailUtils::Type ThumbnailUtils::getType(const char *suffix)
{
    if(!suffix)
        return Type_Unkown;
    if(!strcmp(suffix,"mp4")
            || !strcmp(suffix,"mkv")
            || !strcmp(suffix,"avi")
            || !strcmp(suffix,"ts")) {
        return Type_Video;
    } else if(!strcmp(suffix,"jpeg")
              || !strcmp(suffix,"jpg")
              || !strcmp(suffix,"png")) {
        return Type_Image;
    } else
        return Type_Unkown;
}

void ThumbnailUtils::getThumbnail(QString file_path, ThumbnailUtils::Type type)
{
    QImage thumbnail;

    if(type == Type_Unkown) {
        goto FINISH;
    }

    if(type == Type_Image) {
        thumbnail.load(file_path);
    }

FINISH:
    onGetOneImage(thumbnail,file_path);
    return;
}
