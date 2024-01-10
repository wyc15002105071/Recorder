#include "baseviewer.h"
#include <QFile>
#include <QDir>
#include "widgets/viewer/listwidgetitem.h"

BaseViewer::BaseViewer(QWidget *parent) : BaseWidget(parent)
    ,mDiskSelectionWidget(sp<DiskSelectionWidget>(new DiskSelectionWidget))
    ,mProgressViewer(sp<ProgressViewer>(new ProgressViewer))
    ,mFileUtils(sp<FileUtils>(new FileUtils))
    ,mThumbnail(sp<ThumbnailUtils>(new ThumbnailUtils))
    ,mStorageUtils(StorageUtils::get_instance())
    ,mSelectMode(false)
    ,mOperation(FileUtils::COPY)
    ,mLoadNum(0)
    ,mIconWidth(ICON_WIDTH-X_OFFSET)
    ,mIconHeight(ICON_HEIGHT-Y_OFFSET)
{
    setWindowState(Qt::WindowFullScreen);
    mFileUtils->attach(mProgressViewer.get());

//    connect(mDiskSelectionWidget.get(),SIGNAL(itemClicked(int)),this,SLOT(onDiskItemClicked(int)));
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
//    mIconWidth  = (mListViewer->width() / COLUMN_COUNT_ONE_LINE) - mListViewer->spacing() - X_OFFSET;
//    mIconHeight = (mListViewer->height() / ROW_COUNT_ONE_PAGE) - mListViewer->spacing();
//    if(mListViewer) {
//        mListViewer->clear();
//    }
//    if(mThumbnail.get()) {
//        mThumbnail->stopTask();
//        mThumbnail->setThumbSize(mIconWidth - X_OFFSET,mIconHeight - Y_OFFSET);
//        mThumbnail->setDataSource(mFilePathList);
//        mThumbnail->startTask();
//    }
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

    auto comp = [&](const QString &s1,const QString &s2)->bool {
        return (strcmp(s1.toLatin1().data(),s2.toLatin1().data()) >= 0)?0:1;
    };

    std::sort(mFilePathList.begin(),mFilePathList.end(),comp);
    std::sort(mFileNameList.begin(),mFileNameList.end(),comp);
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

void BaseViewer::loadThumbnail(QImage image, QString file_path)
{
    QListWidgetItem *item = new QListWidgetItem;
    QImage file_icon;
    file_icon = image;
    if(image.isNull()) {
        QImage broken_image(":/resources/icons/broken.png");
        file_icon = broken_image.scaled(mIconWidth - X_OFFSET,mIconHeight - Y_OFFSET,Qt::KeepAspectRatio);
    } else {
//        file_icon = file_icon.scaled(mIconWidth - X_OFFSET,mIconHeight - Y_OFFSET,Qt::KeepAspectRatio);
    }

    ListWidgetItem *custom_item = new ListWidgetItem;
    custom_item->setIcon(file_icon);
    custom_item->setFileName(FileUtils::pathToName(file_path));
    custom_item->setAlignment(Qt::AlignCenter);
    item->setSizeHint(QSize(mIconWidth - X_OFFSET,mIconHeight- Y_OFFSET));
    if(mListViewer) {
        mListViewer->addItem(item);
        mListViewer->setItemWidget(item,custom_item);
    }

    file_icon.detach();
}
