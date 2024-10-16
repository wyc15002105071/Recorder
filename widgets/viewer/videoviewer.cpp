#include "videoviewer.h"
#include "ui_videoviewer.h"
#include <QLabel>
#include <QMovie>
#include "listwidgetitem.h"
#include "utils/storageutils.h"
#include <QScrollBar>
#include <QScroller>

#define MODULE_TAG "VideoViewer"
using namespace std;

VideoViewer::VideoViewer(QWidget *parent) :
    BaseViewer(parent)
   , ui(new Ui::VideoViewer)
//   , mPlayer(sp<VideoPlayer>(new VideoPlayer()))
   , mIconHasUpdated(0)
{
    ui->setupUi(this);
    mPlayer = sp<VideoPlayer>(new VideoPlayer());
    mPlayer->close();

    mFileType = FILE_TYPE_VIDEO;
    mListViewer = ui->video_list;
    mListViewer->setAttribute(Qt::WA_AcceptTouchEvents,true);
    mListViewer->verticalScrollBar()->setStyleSheet(mListViewer->styleSheet());
    QScroller::grabGesture(mListViewer,QScroller::TouchGesture);
    connect(mThumbnail.get(),SIGNAL(onGetOneImage(QImage,QString)),this,SLOT(onLoadThumbnail(QImage,QString)));
    connect(mDiskSelectionWidget.get(),SIGNAL(itemClicked(int)),this,SLOT(onDiskItemClicked(int)));
}

VideoViewer::~VideoViewer()
{
    RLOGD("destructor enter");
    if(mThumbnail.get()) {
        mThumbnail->stopTask();
    }
    mListViewer->clear();
    delete ui;
    RLOGD("destructor leave");
}

void VideoViewer::onHasOpened()
{
    mListViewer->clear();
    mIconHasUpdated = 0;
//    static QMovie movie(":/resources/icons/loading.gif");

    //    int count = mFilePathList.count();
    //    for(int i = 0;i < count;i++) {
    //        QListWidgetItem *item = new QListWidgetItem;
    //        item->setSizeHint(QSize(mIconWidth,mIconHeight));
    //        mListViewer->addItem(item);
    //        ListWidgetItem *custom_item = new ListWidgetItem;
    //        custom_item->setAlignment(Qt::AlignCenter);
    //        movie.setScaledSize(QSize(item->sizeHint().width() / 3,item->sizeHint().height() / 3));
    //        custom_item->setMovie(&movie);

    //        mListViewer->setItemWidget(item,custom_item);
    //        movie.start();
    //    }

    if(mThumbnail) {
        mThumbnail->setThumbSize(mIconWidth - X_OFFSET,mIconHeight - Y_OFFSET);
        mThumbnail->setDataSource(mFilePathList);
        mThumbnail->startTask();
    }

    if(mProgressViewer) {
        mProgressViewer->close();
    }
}

void VideoViewer::onHasClosed()
{
    if(mDiskSelectionWidget)
        mDiskSelectionWidget->close();
    if(mThumbnail) {
        mThumbnail->stopTask();
    }
    mSelectMode = false;
    ui->selectMode_btn->setChecked(mSelectMode);
}

void VideoViewer::open()
{
    findAllFiles(VIDEOS_SAVE_DIR);
    show();
}

void VideoViewer::onItemClicked(QListWidgetItem *item)
{
    if(mSelectMode) {
        ListWidgetItem *item_widget = (ListWidgetItem *)mListViewer->itemWidget(item);
        if(!item_widget)
            return;
        bool is_selected = item_widget->isSelected();
        item_widget->setSelected(!is_selected);
    } else {
        int index = mListViewer->currentRow();
        QString name = mFilePathList.at(index);
        QString video_path = "file://"+name;
        if(mPlayer) {
            mPlayer->setGeometry(this->geometry());
            mPlayer->open(mFilePathList,index);
        }
    }
}

void VideoViewer::onSelectModeToggled(bool toggled)
{
    mSelectionlist.clear();
    int count = mListViewer->count();
    if(count <= 0)
        return;
    mSelectMode = toggled;
    for(int i = 0;i < count;i++) {
        QListWidgetItem *item = mListViewer->item(i);
        if(!item)
            continue;
        ListWidgetItem *item_widget = (ListWidgetItem *)mListViewer->itemWidget(item);
        item_widget->setSelectable(toggled);
    }
}

void VideoViewer::onCopySelectedClicked()
{
    mSelectionlist.clear();

    for(int i = 0;i<mListViewer->count();i++) {
        QListWidgetItem *item = mListViewer->item(i);
        ListWidgetItem *item_widget = (ListWidgetItem *)mListViewer->itemWidget(item);

        if(!item_widget)
            continue;
        if(item_widget->isSelected()) {
            mSelectionlist.push_back(mFilePathList.at(i));
        }
    }
    mOperation = FileUtils::COPY;
    openDiskSelection();
}

void VideoViewer::onCopyAllClicked()
{
    mSelectionlist.clear();
    mOperation = FileUtils::COPY;
    mSelectionlist = mFilePathList;

    openDiskSelection();
}

void VideoViewer::onDelSelectClicked()
{
    mOperation = FileUtils::DELETE;
    mSelectionlist.clear();

    QList<QListWidgetItem*> del_list;
    del_list.clear();
    if(mProgressViewer) {
        mProgressViewer->setOperation(mOperation);
    }
    for(int i = 0;i<mListViewer->count();i++) {
        QListWidgetItem *item = mListViewer->item(i);
        ListWidgetItem *item_widget = (ListWidgetItem *)mListViewer->itemWidget(item);

        if(!item_widget)
            continue;
        if(item_widget->isSelected()) {
            mSelectionlist.push_back(mFilePathList.at(i));
            del_list.push_back(item);
        }
    }

    if (mSelectionlist.count() == 0) {
        mProgressViewer->showWarning("未选中对象...");
        if(mDiskSelectionWidget)
            mDiskSelectionWidget->close();
        return;
    }

    while(del_list.count() > 0) {
        QListWidgetItem *item = del_list.front();

        int row = mListViewer->row(item);
        mListViewer->removeItemWidget(item);
        mListViewer->takeItem(row);
        del_list.pop_front();

        mFilePathList.removeAt(row);
        mFileNameList.removeAt(row);
    }

    mFileUtils->startDelete(mSelectionlist);
}

void VideoViewer::onDelAllClicked()
{
    mOperation = FileUtils::DELETE;
    if(mProgressViewer) {
        mProgressViewer->setOperation(mOperation);
    }
    mListViewer->clear();
    mFileUtils->startDelete(mFilePathList);
    mFilePathList.clear();
    mFileNameList.clear();
}

void VideoViewer::onDiskItemClicked(int index)
{
    RLOGD("label:%s,nodepath:%s,mountpath:%s,filesystem:%s",mExternalStorageInfo[index].label.c_str(),
          mExternalStorageInfo[index].node_path.c_str(),
          mExternalStorageInfo[index].mount_path.c_str(),
          mExternalStorageInfo[index].file_system.c_str());
    RLOGD("select count %d",mSelectionlist.count());
    if (mSelectionlist.count() == 0) {
        mProgressViewer->showWarning("未选中对象...");

        if(mDiskSelectionWidget)
            mDiskSelectionWidget->close();

        return;
    }
    QString dst_dir = QString::fromStdString(mExternalStorageInfo[index].mount_path);
    mFileUtils->startCopy(mSelectionlist,dst_dir);
    if(mDiskSelectionWidget)
        mDiskSelectionWidget->close();
    if(mProgressViewer) {
        mProgressViewer->setOperation(mOperation);
    }
}

void VideoViewer::onLoadThumbnail(QImage image, QString file_path)
{
    loadThumbnail(image,file_path);
}

