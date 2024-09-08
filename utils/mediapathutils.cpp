#include "mediapathutils.h"
#include "utils/configutils.h"
#include <mntent.h>
#include <QDir>
#include "common/log.h"
#include "utils/fileutils.h"

QString MediaPathUtils::getRootPath()
{
    if(!ConfigUtils::isUsbMedia)
        return QString(ROOT_DIR);
    const char * mount= getMountPath(ConfigUtils::usbName.toStdString().c_str());
    if(!mount ||QString(mount)!= ConfigUtils::usbMonut){
        ConfigUtils::isUsbMedia = false;
        return QString(ROOT_DIR);
    }
    return QString(mount);
}

QString MediaPathUtils::getRootVideoPath()
{
    if(!ConfigUtils::isUsbMedia)
        return QString(VIDEOS_SAVE_DIR);
    const char * mount= getMountPath(ConfigUtils::usbName.toStdString().c_str());
    if(!mount ||QString(mount)!= ConfigUtils::usbMonut){
        ConfigUtils::isUsbMedia = false;
        return QString(VIDEOS_SAVE_DIR);
    }
    QString path = QString(mount).append("/videos");
    QDir dir;
    if (!dir.exists(path))
    {
        bool res = dir.mkpath(path);
        if(res)
        RLOGD("make %s success",path.toStdString().c_str());
    }
    return path;
}

QString MediaPathUtils::getRootImagePath()
{
    if(!ConfigUtils::isUsbMedia)
        return QString(IMAGES_SAVE_DIR);
    const char * mount= getMountPath(ConfigUtils::usbName.toStdString().c_str());
    if(!mount ||QString(mount)!= ConfigUtils::usbMonut){
        ConfigUtils::isUsbMedia = false;
        return QString(IMAGES_SAVE_DIR);
    }

    QString path = QString(mount).append("/images");
    QDir dir;
    if (!dir.exists(path))
    {
        bool res = dir.mkpath(path);
        if(res)
        RLOGD("make %s success",path.toStdString().c_str());
    }
    return path;
}

QString MediaPathUtils::getImagePath()
{
    char time_str[100] = {0};
    char file_name[100] = {0};
    char file_save_path[100] = {0};
    char date_str[100] = {0};

    getCurentTime(time_str,"%Y-%m-%d_%H-%M-%S",true);
    getCurrentDate(date_str, "%Y-%m-%d");
    char dir[100] = {0};

    QString path = getRootImagePath();
    RLOGD(path.toStdString().c_str());
    sprintf(dir, "%s/%s", path.toStdString().c_str() ,date_str);

    FileUtils::mkdirIfNotExit(QString(dir));

    sprintf(file_name,"%s.jpg",time_str);
    sprintf(file_save_path,"%s/%s/%s",path.toStdString().c_str(), date_str, file_name);
    RLOGD("capture file path is %s",file_save_path);
    return QString(file_save_path);
}

MediaPathUtils::MediaPathUtils()
{

}

const char *MediaPathUtils::getMountPath(const char *node_path)
{
    FILE *mounts;
    struct mntent *ent;

    // 打开 /etc/mtab 文件以获取挂载点信息
    mounts = setmntent("/etc/mtab", "r");
    if (mounts == NULL) {
        perror("Error opening /etc/mtab");
        return NULL;
    }

    // 读取并输出挂载信息
    while ((ent = getmntent(mounts)) != NULL) {
        if(!strcmp(node_path,ent->mnt_fsname)) {
            return ent->mnt_dir;
        }
    }

    // 关闭文件
    endmntent(mounts);

    return NULL;
}
