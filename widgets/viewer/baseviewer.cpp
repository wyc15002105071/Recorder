#include "baseviewer.h"
#include <QFile>
#include <QDir>
BaseViewer::BaseViewer(QWidget *parent) : BaseWidget(parent)
    ,mDiskSelectionWidget(sp<DiskSelectionWidget>(new DiskSelectionWidget))
    ,mProgressViewer(sp<ProgressViewer>(new ProgressViewer))
    ,mFileUtils(sp<FileUtils>(new FileUtils))
    ,mThumbnail(sp<ThumbnailUtils>(new ThumbnailUtils))
    ,mStorageUtils(StorageUtils::get_instance())
    ,mSelectMode(false)
    ,mOperation(FileUtils::COPY)
    ,mLoadNum(0)
    ,mIconWidth(0)
    ,mIconHeight(0)
{
    setWindowState(Qt::WindowFullScreen);
    mFileUtils->attach(mProgressViewer.get());
    connect(mDiskSelectionWidget.get(),SIGNAL(itemClicked(int)),this,SLOT(onDiskItemClicked(int)));
    close();
}

BaseViewer::~BaseViewer()
{
	if(mStorageUtils) {
		mStorageUtils = nullptr;
	}
}

void BaseViewer::resizeEvent(QResizeEvent *event)
{
    mProgressViewer->move(this->width()/2-mProgressViewer->width()/2,this->height()-mProgressViewer->height()-20);
    mIconWidth = (mListViewer->width() / COLUMN_COUNT_ONE_LINE) - mListViewer->spacing() - X_OFFSET;
    mIconHeight = mListViewer->height() / ROW_COUNT_ONE_PAGE - mListViewer->spacing() - Y_OFFSET;
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

    mFilePathList.sort();
    mFileNameList.sort();
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
