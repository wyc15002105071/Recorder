#include "abstractviewer.h"
#include <QFile>
#include <QDir>
AbStractViewer::AbStractViewer(QWidget *parent)
    :QWidget(parent)
{
    showFullScreen();
    close();
}

void AbStractViewer::findAllFiles(const char *dir)
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
    for(auto image_file:file_dir.entryList(nameFilter))
    {
        mFileList.append(QString(dir)+"/"+image_file);
    }
}
