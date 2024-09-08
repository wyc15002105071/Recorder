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
   , mKeyListener(KeyListener::get_instance())
   , mIconHasUpdated(0)
{
    ui->setupUi(this);
    //mPlayer = sp<VideoPlayer>(new VideoPlayer());
    //mPlayer->close();

    mFileType    = FILE_TYPE_VIDEO;
	
    mFileListView = ui->video_list;
    mFileListView->setPath(VIDEOS_SAVE_DIR);
    mFileListView->setIconSize(mIconWidth, mIconHeight);
	mFileListView->setFileType(QFileListView::File_TypeVideo);
	
    mCapacityBar = ui->capacity;
    mExtStorageWidget = ui->extStorageWidget;

    //mListViewer->setAttribute(Qt::WA_AcceptTouchEvents,true);
    //mListViewer->verticalScrollBar()->setStyleSheet(mListViewer->styleSheet());
    //QScroller::grabGesture(mListViewer,QScroller::TouchGesture);
    //connect(mThumbnail.get(),SIGNAL(onGetOneImage(QImage,QString)),this,SLOT(onLoadThumbnail(QImage,QString)));
    connect(mKeyListener,SIGNAL(onPressed(KeyListener::EventType)),this,SLOT(onKeyEventHandler(KeyListener::EventType)));
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
    //mListViewer->clear();
    RLOGD("---video view has opened---");
    mFileListView->updateDirView();
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

	if(mThumbnail.get()) {
		mThumbnail->setThumbSize(mIconWidth - X_OFFSET,mIconHeight - Y_OFFSET);
		mThumbnail->setDataSource(mFilePathList);
		mThumbnail->startTask();
	}


    if(mProgressViewer.get()) {
        mProgressViewer->move(this->width()/2-mProgressViewer->width()/2,this->height()-mProgressViewer->height()-20);
        mProgressViewer->close();
    }


    if(mCapacityListenerTimer) {
        mCapacityListenerTimer->start();
    }

    //if(mHotplugListener) {
    //    mHotplugListener->startTask();
    //}

    onUpdateExtStorageView();

    onUpdateCapacity();
}

void VideoViewer::onHasClosed()
{
    if(mPlayer&&mPlayer->isVisible())
        mPlayer->close();
    if(mConfirmDialog)
        mConfirmDialog->close();
    if(mDiskSelectionWidget)
        mDiskSelectionWidget->close();
    if(mThumbnail) {
        mThumbnail->stopTask();
    }
    mSelectMode = false;
    ui->selectMode_btn->setChecked(mSelectMode);

    if(mCapacityListenerTimer) {
        mCapacityListenerTimer->stop();
    }

    //if(mHotplugListener) {
    //    mHotplugListener->stopTask();
    //}
}

void VideoViewer::onBackClicked()
{
    int page = mFileListView->getCurrentPage();
    if(page > 0) {
        mFileListView->setCurrentPage(page - 1);
		ui->selectMode_btn->setChecked(false);
    } else {
        this->close();
    }
}


void VideoViewer::open()
{
    if(mProgressViewer){
        mProgressViewer->setOperation(2);
    }
    //findAllFiles(VIDEOS_SAVE_DIR);
    show();
}

bool VideoViewer::isPlay()
{
    return mPlayer&&mPlayer->isVisible();
}

void VideoViewer::onItemClicked(QListWidgetItem *item)
{
    QScroller *scroller = QScroller::scroller(mListViewer);

    if(scroller && scroller->state() != QScroller::Inactive)
        return;
    if(mSelectMode) {
        ListWidgetItem *item_widget = (ListWidgetItem *)mListViewer->itemWidget(item);
        if(!item_widget)
            return;
        bool is_selected = item_widget->isSelected();
        item_widget->setSelected(!is_selected);
    } else {
        int current_index = mListViewer->currentRow();
        if(mPlayer) {
            mPlayer->setGeometry(this->geometry());
            mPlayer->openPlayer(mFilePathList,current_index);
        }
    }

}

void VideoViewer::onSelectModeToggled(bool toggled)
{
    mFileListView->setSelectionMode(toggled);
}

void VideoViewer::onCopySelectedClicked()
{
    mSelectionlist.clear();
    mOperation = FileUtils::COPY;
    mSelectionlist = mFileListView->getSelectedPathOfItems();

    if (mSelectionlist.count() == 0) {
        mProgressViewer->showWarning("未选中对象...");
        if(mDiskSelectionWidget)
            mDiskSelectionWidget->close();
        return;
    }

    RLOGD("mSelectionlist count is %d, %s",mSelectionlist.count(), mSelectionlist.at(0).toLatin1().data());
	
    openDiskSelection();

}

void VideoViewer::onCopyAllClicked()
{
    mSelectionlist.clear();
    mOperation = FileUtils::COPY;
    mSelectionlist = mFileListView->getAllPathOfItems();;
    if (mSelectionlist.count() == 0) {
        mProgressViewer->showWarning("未选中对象...");
        if(mDiskSelectionWidget)
            mDiskSelectionWidget->close();
        return;
    }
	
    openDiskSelection();
}

void VideoViewer::onDelSelectClicked()
{
    isDeleteAll = false;
    mOperation = FileUtils::DELETE;
    mSelectionlist.clear();


    del_list.clear();
    if(mProgressViewer) {
        mProgressViewer->setOperation(mOperation);
    }
	
    mSelectionlist = mFileListView->getSelectedPathOfItems();

    if (mSelectionlist.count() == 0) {
        mProgressViewer->showWarning("未选中对象...");
        if(mDiskSelectionWidget)
            mDiskSelectionWidget->close();
        return;
    }
    if(!mConfirmDialog){
        mConfirmDialog = new ConfirmDialog();
        mConfirmDialog->setTitle("确认删除吗");
        connect(mConfirmDialog,&ConfirmDialog::rejected,this,[=]{
            //mConfirmDialog->deleteLater();
            //mConfirmDialog = nullptr;
        });
        connect(mConfirmDialog,&ConfirmDialog::accepted,this,&VideoViewer::onDelSelect);
        connect(mConfirmDialog,&ConfirmDialog::accepted,this,&VideoViewer::onDelAll);
    }
    mConfirmDialog->showFullScreen();

    //int ret = mConfirmDialog->exec();
    //if(ret == QDialog::Rejected) {
    //    return;
    //}

    //while(del_list.count() > 0) {
    //    QListWidgetItem *item = del_list.front();
    //
    //    int row = mListViewer->row(item);
    //    mListViewer->removeItemWidget(item);
    //    mListViewer->takeItem(row);
    //    del_list.pop_front();
    //
    //    mFilePathList.removeAt(row);
    //    mFileNameList.removeAt(row);
    //}
    //
    //mFileUtils->startDelete(mSelectionlist);
}

void VideoViewer::onDelAllClicked()
{
    isDeleteAll = true;
	mSelectionlist = mFileListView->getAllPathOfItems();
    if (mSelectionlist.count() == 0) {
        mProgressViewer->showWarning("未选中对象...");
        if(mDiskSelectionWidget)
            mDiskSelectionWidget->close();
        return;
    }

    if(!mConfirmDialog){
        mConfirmDialog = new ConfirmDialog();
        mConfirmDialog->setTitle("确认删除吗");
        connect(mConfirmDialog,&ConfirmDialog::rejected,this,[=]{
            //mConfirmDialog->deleteLater();
            //mConfirmDialog = nullptr;
        });
        connect(mConfirmDialog,&ConfirmDialog::accepted,this,&VideoViewer::onDelSelect);
        connect(mConfirmDialog,&ConfirmDialog::accepted,this,&VideoViewer::onDelAll);
    }
    mConfirmDialog->showFullScreen();
    //int ret = mConfirmDialog->exec();
    //if(ret == QDialog::Rejected) {
    //    return;
    //}
    //
    //mThumbnail->stopTask();
    //mOperation = FileUtils::DELETE;
    //if(mProgressViewer) {
    //    mProgressViewer->setOperation(mOperation);
    //}
    //mListViewer->clear();
    //mFileUtils->startDelete(mFilePathList);
    //mFilePathList.clear();
    //mFileNameList.clear();
}

void VideoViewer::onDelSelect()
{
    if(isDeleteAll)return;

	mFileListView->delSelected();

    mFileUtils->startDelete(mSelectionlist);

}

void VideoViewer::onDelAll()
{
    if(!isDeleteAll)return;
    mThumbnail->stopTask();
    mOperation = FileUtils::DELETE;
    if(mProgressViewer) {
        mProgressViewer->setOperation(mOperation);
    }
	
	mFileListView->delAll();
    //mListViewer->clear();
    mFileUtils->startDelete(mFilePathList);
    //mFilePathList.clear();
    //mFileNameList.clear();
}

void VideoViewer::onDiskItemClicked(int index)
{
    RLOGD("select count %d",mSelectionlist.count());
    if (mSelectionlist.count() == 0) {
        mProgressViewer->showWarning("未选中对象...");

        if(mDiskSelectionWidget)
            mDiskSelectionWidget->close();

        return;
    }

    if(mExternalStorageInfo.count() == 0) {
        mProgressViewer->showWarning(COPY_FAILED);
        return;
    }
    if(!compareDisk(mExternalStorageInfo[index].mount_path.c_str(),mSelectionlist)){
        mProgressViewer->showWarning(FREE_NO);
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

void VideoViewer::onKeyEventHandler(KeyListener::EventType type)
{
    if(!this->isVisible()||(mPlayer&&mPlayer->isVisible())
            ||(mDiskSelectionWidget&&mDiskSelectionWidget->isVisible())
            ||(mProgressViewer&&mProgressViewer->isVisible())) {
        return;
    }

    switch (type)
    {
    case KeyListener::Key_EventType_UP:
        if(mConfirmDialog&&mConfirmDialog->isVisible()){

        }else{
            if(mSelectMode) {
                onCopySelectedClicked();
            } else {
                onCopyAllClicked();
            }
        }

        break;
    case KeyListener::Key_EventType_DOWN:
        if(mConfirmDialog&&mConfirmDialog->isVisible()){
            mConfirmDialog->reject();
        }else{
            if(mSelectMode){
                onDelSelectClicked();
            }else{
                onDelAllClicked();
            }
        }
        break;
    default:
        break;
    }
}

void VideoViewer::onLoadThumbnail(QImage image, QString file_path)
{
    loadThumbnail(image,file_path);
}

