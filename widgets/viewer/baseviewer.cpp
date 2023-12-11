#include "baseviewer.h"
#include <QFile>
#include <QDir>
BaseViewer::BaseViewer(QWidget *parent)
    :BaseWidget(parent)
    ,mSelectMode(false)
    ,mDiskSelectionWidget(sp<DiskSelectionWidget>(new DiskSelectionWidget))
    ,mStorageUtils(sp<StorageUtils>(StorageUtils::get_instance()))
    ,mFileUtils(sp<FileUtils>(new FileUtils))
    ,mProgressViewer(sp<ProgressViewer>(new ProgressViewer))
    ,mOperation(FileUtils::COPY)
{
    setWindowState(Qt::WindowFullScreen);
    mFileUtils->attach(mProgressViewer.get());
//    connect(mDiskSelectionWidget.get(),SIGNAL(itemClicked(int)),this,SLOT(onDiskItemClicked(int)));
    close();
}

void BaseViewer::resizeEvent(QResizeEvent *event)
{
    mProgressViewer->move(this->width()/2-mProgressViewer->width()/2,this->height()-mProgressViewer->height()-20);
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

    mFilePathList.clear();
    mFileNameList.clear();
    for(auto file:file_dir.entryList(nameFilter))
    {
        mFilePathList.append(QString(dir)+"/"+file);
        mFileNameList.append(file);
    }
}

void BaseViewer::openDiskSelection()
{
    mExternalStorageInfo.clear();
    mExternalStorageInfo = mStorageUtils->getExternalStorageInfoList();
    for(int i = 0;i < mExternalStorageInfo.count();i++) {
        if(mDiskSelectionWidget) {
            mDiskSelectionWidget->additem(QString::fromStdString(mExternalStorageInfo[i].label));
        }
    }

    if(mDiskSelectionWidget) {
        if(mDiskSelectionWidget->isVisible())
            mDiskSelectionWidget->close();
        mDiskSelectionWidget->open();
        mDiskSelectionWidget->move((this->width() - mDiskSelectionWidget->width())/2,(this->height() - mDiskSelectionWidget->height())/2);
    }
}


