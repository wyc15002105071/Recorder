#include "baseviewer.h"
#include <QFile>
#include <QDir>
BaseViewer::BaseViewer(QWidget *parent)
    :BaseWidget(parent)
    ,mSelectMode(false)
    ,mProgressViewer(sp<ProgressViewer>(new ProgressViewer))
    ,mDiskSelectionWidget(sp<DiskSelectionWidget>(new DiskSelectionWidget))
    ,mStorageUtils(sp<StorageUtils>(StorageUtils::get_instance()))
    ,mFileUtils(sp<FileUtils>(new FileUtils))
    ,mOperation(FileUtils::COPY)
{
    setWindowState(Qt::WindowFullScreen);
    close();
}

void BaseViewer::resizeEvent(QResizeEvent *event)
{
    mProgressViewer->move(this->width()/2-mProgressViewer->width()/2,this->height()-mProgressViewer->height()-20);
}

void BaseViewer::findAllFiles()
{
    QDir file_dir;
    QStringList nameFilter;
    if(mFileType == FILE_TYPE_IMAGE) {
        nameFilter<<"*.jpg"<<"*.png"<<"*.jfif";
        file_dir = QDir(IMAGES_SAVE_DIR);
    }
    else if(mFileType == FILE_TYPE_VIDEO) {
        nameFilter<<"*.mp4"<<"*.mkv"<<"*.avi";
        file_dir = QDir(VIDEOS_SAVE_DIR);
    }

    if(!file_dir.exists()) {
        LOG("open dir %s failed",file_dir.path().toLatin1().data());
        return;
    }

    mFilePathList.clear();
    mFileNameList.clear();
    for(auto file:file_dir.entryList(nameFilter))
    {
        mFilePathList.append(QString(file_dir.path())+"/"+file);
        mFileNameList.append(file);
    }
}

void BaseViewer::open()
{
    if(this->isVisible()) {
        return;
    }

    findAllFiles();
    this->show();
    showFullScreen();
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

