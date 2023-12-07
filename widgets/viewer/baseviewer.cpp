#include "baseviewer.h"
#include <QFile>
#include <QDir>
BaseViewer::BaseViewer(QWidget *parent)
    :BaseWidget(parent)
    ,mSelectMode(false)
{
    setWindowState(Qt::WindowFullScreen);
    close();
}

void BaseViewer::findAllFiles(const char *dir)
{
    QDir file_dir(dir);
    if(!file_dir.exists())
        return;
    QStringList nameFilter;
    if(mFileType == FILE_TYPE_IMAGE)
        nameFilter<<"*.jpg"<<"*.png"<<"*.jfif";
    else if(mFileType == FILE_TYPE_VIDEO)
        nameFilter<<"*.mp4"<<"*.mkv"<<"*.avi";

    mFileList.clear();
    for(auto file:file_dir.entryList(nameFilter))
    {
        mFileList.append(QString(dir)+"/"+file);
        mFileNameList.append(file);
    }
}
