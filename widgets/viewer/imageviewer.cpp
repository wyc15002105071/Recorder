#include "imageviewer.h"
#include "ui_imageviewer.h"
#include <QLabel>
#include "listwidgetitem.h"
#include <QScroller>
#include <QScrollBar>

ImageViewer::ImageViewer(QWidget *parent) : BaseViewer(parent)
    , mKeyListener(KeyListener::get_instance())
    , ui(new Ui::ImageViewer)
{
    ui->setupUi(this);
    //mImageBrowser = sp<ImageBrowser>(new ImageBrowser(this));
    mFileType     = FILE_TYPE_IMAGE;
    //mListViewer   = ui->image_list;
    mCapacityBar  = ui->capacity;
    mExtStorageWidget = ui->extStorageWidget;

//    mScroll     = mListViewer->verticalScrollBar();
//    mScroll->setStyleSheet(mListViewer->styleSheet());
//    mListViewer->setAttribute(Qt::WA_AcceptTouchEvents,true);
    mFileListView = ui->image_list;
    mFileListView->setPath(IMAGES_SAVE_DIR);
    mFileListView->setIconSize(mIconWidth, mIconHeight);
	mFileListView->setFileType(QFileListView::File_TypeImage);
	
    connect(mDiskSelectionWidget.get(),SIGNAL(itemClicked(int)),this,SLOT(onDiskItemClicked(int)),Qt::UniqueConnection);
    connect(mScroll,SIGNAL(valueChanged(int)),this,SLOT(onScrollValueChanged(int)));
    connect(mThumbnail.get(),SIGNAL(onGetOneImage(QImage,QString)),this,SLOT(onLoadThumbnail(QImage,QString)));
    connect(mKeyListener,SIGNAL(onPressed(KeyListener::EventType)),this,SLOT(onKeyEventHandler(KeyListener::EventType)));
}

ImageViewer::~ImageViewer()
{
    if(mThumbnail.get()) {
        mThumbnail->stopTask();
    }
    mListViewer->clear();
    mLoadNum = 0;
    delete ui;
}

void ImageViewer::open()
{
    if(mProgressViewer){
        mProgressViewer->setOperation(2);
    }
//    findAllFiles(IMAGES_SAVE_DIR);
    show();
}

void ImageViewer::onItemClicked(QListWidgetItem *item)
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
        RLOGD("current index is %d");
        if(mImageBrowser) {
            mImageBrowser->setGeometry(this->geometry());
            mImageBrowser->openPlayer(mFilePathList,current_index);
        }
    }
}

void ImageViewer::onSelectModeToggled(bool toggled)
{
	mFileListView->setSelectionMode(toggled);
}

void ImageViewer::onHasOpened()
{
//    mListViewer->clear();
    RLOGD("---image view has opened---");
    mFileListView->updateDirView();
    if(mThumbnail.get()) {
//        mThumbnail->stopTask();
        mThumbnail->setThumbSize(mIconWidth - X_OFFSET,mIconHeight - Y_OFFSET);
        mThumbnail->setDataSource(mFilePathList);
        mThumbnail->startTask();
    }
//    loadThumbnail(mFilePathList,0,mFilePathList.count());
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

void ImageViewer::onHasClosed()
{

    if(mConfirmDialog)
        mConfirmDialog->close();

    if(mDiskSelectionWidget.get())
        mDiskSelectionWidget->close();
    if(mThumbnail.get()) {
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

void ImageViewer::onCopySelectedClicked()
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

void ImageViewer::onCopyAllClicked()
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

void ImageViewer::onCopyCurrent()
{
    mSelectionlist.clear();
    mOperation = FileUtils::COPY;
    mSelectionlist.append(mImageBrowser->getCurrentIndex());
    if (mSelectionlist.count() == 0) {
        mProgressViewer->showWarning("未选中对象...");
        if (mDiskSelectionWidget)
            mDiskSelectionWidget->close();
        return;
    }

    openDiskSelection();
}

void ImageViewer::onDelSelectClicked()
{
    isDeleteAll = false;
    mOperation = FileUtils::DELETE;
    mSelectionlist.clear();

    //QList<QListWidgetItem*> del_list;
    del_list.clear();
    if(mProgressViewer.get()) {
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
    connect(mConfirmDialog,&ConfirmDialog::accepted,this,&ImageViewer::onDelSelect);
    connect(mConfirmDialog,&ConfirmDialog::accepted,this,&ImageViewer::onDelAll);

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

void ImageViewer::onDelSelect()
{
    if(isDeleteAll)return;

	mFileListView->delSelected();

    mFileUtils->startDelete(mSelectionlist);
}
void ImageViewer::onDelAllClicked()
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
        connect(mConfirmDialog,&ConfirmDialog::accepted,this,&ImageViewer::onDelSelect);
        connect(mConfirmDialog,&ConfirmDialog::accepted,this,&ImageViewer::onDelAll);
    }

    mConfirmDialog->showFullScreen();
    //int ret = mConfirmDialog->exec();
    //if(ret == QDialog::Rejected) {
    //    return;
    //}
    //
    //mThumbnail->stopTask();
    //mOperation = FileUtils::DELETE;
    //if(mProgressViewer.get()) {
    //    mProgressViewer->setOperation(mOperation);
    //}
    //mListViewer->clear();
    //mFileUtils->startDelete(mFilePathList);
    //mFilePathList.clear();
    //mFileNameList.clear();
}

void ImageViewer::onBackClicked()
{
    int page = mFileListView->getCurrentPage();
    if(page > 0) {
        mFileListView->setCurrentPage(page - 1);
		ui->selectMode_btn->setChecked(false);
    } else {
        this->close();
    }
}

void ImageViewer::onDelAll()
{
    if(!isDeleteAll)return;
    //mThumbnail->stopTask();
    mOperation = FileUtils::DELETE;
    if(mProgressViewer.get()) {
        mProgressViewer->setOperation(mOperation);
    }

	mFileListView->delAll();
	
    //mListViewer->clear();
    mFileUtils->startDelete(mSelectionlist);
    //mFilePathList.clear();
    //mFileNameList.clear();

}

void ImageViewer::onDiskItemClicked(int index)
{
//    RLOGD("label:%s,nodepath:%s,mountpath:%s,filesystem:%s",mExternalStorageInfo[index].label.c_str(),
//                      mExternalStorageInfo[index].node_path.c_str(),
//                      mExternalStorageInfo[index].mount_path.c_str(),
//                      mExternalStorageInfo[index].file_system.c_str());
    RLOGD("select count %d",mSelectionlist.count());
    if (mSelectionlist.count() == 0) {
        mProgressViewer->showWarning("未选中对象...");

        if(mDiskSelectionWidget.get())
            mDiskSelectionWidget->close();

        return;
    }

    if(mExternalStorageInfo.count() == 0) {
        mProgressViewer->showWarning(COPY_FAILED);
        return;
    }
    RLOGD("select count %d",mSelectionlist.count());
    if(!compareDisk(mExternalStorageInfo[index].mount_path.c_str(),mSelectionlist)){
        mProgressViewer->showWarning(FREE_NO);
        return;
    }
    QString dst_dir = QString::fromStdString(mExternalStorageInfo[index].mount_path);
    mFileUtils->startCopy(mSelectionlist, dst_dir);
    if(mDiskSelectionWidget.get())
        mDiskSelectionWidget->close();
    if(mProgressViewer.get()) {
        mProgressViewer->setOperation(mOperation);
    }
}

void ImageViewer::onScrollValueChanged(int value)
{

}

void ImageViewer::onLoadThumbnail(QImage image,QString file_path)
{
    loadThumbnail(image,file_path);
    mLoadNum++;
}

void ImageViewer::onKeyEventHandler(KeyListener::EventType type)
{
    if(!this->isVisible()
            ||(mDiskSelectionWidget&&mDiskSelectionWidget->isVisible())
            ||(mProgressViewer&&mProgressViewer->isVisible())) {
        return;
    }
    switch (type)
    {
    case KeyListener::Key_EventType_UP:
        if(mImageBrowser&&mImageBrowser->isVisible()){
            onCopyCurrent();
        }else if(mConfirmDialog&&mConfirmDialog->isVisible()){

        }
        else{
            if(mSelectMode) {
                onCopySelectedClicked();
            } else {
                onCopyAllClicked();
            }
        }
        break;
    case KeyListener::Key_EventType_DOWN:
        if(mImageBrowser&&mImageBrowser->isVisible()){
        }else if(mConfirmDialog&&mConfirmDialog->isVisible())
        {
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
