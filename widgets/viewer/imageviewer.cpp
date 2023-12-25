#include "imageviewer.h"
#include "ui_imageviewer.h"
#include <QLabel>
#include "listwidgetitem.h"
#include <QScroller>
#include <QScrollBar>

ImageViewer::ImageViewer(QWidget *parent) :
    BaseViewer(parent),
    ui(new Ui::ImageViewer),
    mImageBrowser(sp<ImageBrowser>(new ImageBrowser()))
{
    ui->setupUi(this);
    mFileType = FILE_TYPE_IMAGE;

    ui->image_list->setAttribute(Qt::WA_AcceptTouchEvents,true);
    QScroller::grabGesture(ui->image_list,QScroller::TouchGesture);
    connect(mDiskSelectionWidget.get(),SIGNAL(itemClicked(int)),this,SLOT(onDiskItemClicked(int)),Qt::UniqueConnection);
    ui->image_list->verticalScrollBar()->setStyleSheet(ui->image_list->styleSheet());
}

ImageViewer::~ImageViewer()
{
    ui->image_list->clear();
    delete ui;
}

void ImageViewer::open()
{
    findAllFiles(IMAGES_SAVE_DIR);
    show();
}

void ImageViewer::onItemClicked(QListWidgetItem *item)
{
    if(mSelectMode) {
        ListWidgetItem *item_widget = (ListWidgetItem *)ui->image_list->itemWidget(item);
        if(!item_widget)
            return;
        bool is_selected = item_widget->isSelected();
        item_widget->setSelected(!is_selected);
    } else {
        int current_index = ui->image_list->currentRow();
        if(mImageBrowser) {
            mImageBrowser->open(mFilePathList,current_index);
        }
    }
}

void ImageViewer::onSelectModeToggled(bool toggled)
{
    int count = ui->image_list->count();
    if(count <= 0)
        return;
    mSelectMode = toggled;
    for(int i = 0;i < count;i++) {
        QListWidgetItem *item = ui->image_list->item(i);
        if(!item)
            continue;
        ListWidgetItem *item_widget = (ListWidgetItem *)ui->image_list->itemWidget(item);
        item_widget->setSelectable(toggled);
    }
}

void ImageViewer::onHasOpened()
{
    ui->image_list->clear();
    for(int i = 0;i<mFilePathList.size();i++) {
        QListWidgetItem *item = new QListWidgetItem;
        QPixmap file_icon;
        QImage image(mFilePathList.at(i));

        file_icon = QPixmap::fromImage(image);
        if(image.isNull()) {
            QImage broken_image(":/resources/icons/broken.png");
            file_icon = QPixmap::fromImage(broken_image.scaled(ICON_WIDTH - X_OFFSET,ICON_HEIGHT - Y_OFFSET,Qt::KeepAspectRatio));
        }else if(image.width() > ICON_WIDTH || image.height() > ICON_HEIGHT) {
            file_icon = QPixmap::fromImage(image.scaled(ICON_WIDTH - X_OFFSET,ICON_HEIGHT - Y_OFFSET,Qt::KeepAspectRatio));//保持长宽比例缩放
        }
        ListWidgetItem *custom_item = new ListWidgetItem;
        custom_item->setIcon(file_icon);
        custom_item->setFileName(mFileNameList.at(i));
        custom_item->setAlignment(Qt::AlignCenter);
        item->setSizeHint(QSize(ICON_WIDTH,ICON_HEIGHT));
        ui->image_list->addItem(item);
        ui->image_list->setItemWidget(item,custom_item);
    }
    if(mProgressViewer) {
        mProgressViewer->move(this->width()/2-mProgressViewer->width()/2,this->height()-mProgressViewer->height()-20);
        mProgressViewer->close();
    }
}

void ImageViewer::onHasClosed()
{
    if(mDiskSelectionWidget)
        mDiskSelectionWidget->close();

    mSelectMode = false;
    ui->selectMode_btn->setChecked(mSelectMode);
}

void ImageViewer::onCopySelectedClicked()
{
    mSelectionlist.clear();
    mOperation = FileUtils::COPY;
    for(int i = 0;i<ui->image_list->count();i++) {
        QListWidgetItem *item = ui->image_list->item(i);
        ListWidgetItem *item_widget = (ListWidgetItem *)ui->image_list->itemWidget(item);

        if(!item_widget)
            continue;
        if(item_widget->isSelected()) {
            mSelectionlist.push_back(mFilePathList.at(i));
        }
    }

    RLOGD("mSelectionlist count is %d",mSelectionlist.count());
    openDiskSelection();
}

void ImageViewer::onCopyAllClicked()
{
    mSelectionlist.clear();
    mOperation = FileUtils::COPY;
    mSelectionlist = mFilePathList;
    openDiskSelection();
}

void ImageViewer::onDelSelectClicked()
{
    mOperation = FileUtils::DELETE;
    mSelectionlist.clear();
    QList<QListWidgetItem*> del_list;
    del_list.clear();
    if(mProgressViewer) {
        mProgressViewer->setOperation(mOperation);
    }
    for(int i = 0;i<ui->image_list->count();i++) {
        QListWidgetItem *item = ui->image_list->item(i);
        ListWidgetItem *item_widget = (ListWidgetItem *)ui->image_list->itemWidget(item);

        if(!item_widget)
            continue;
        if(item_widget->isSelected()) {
            mSelectionlist.push_back(mFilePathList.at(i));
            del_list.push_back(item);
        }
    }

    while(del_list.count() > 0) {
        QListWidgetItem *item = del_list.front();

        int row = ui->image_list->row(item);
        ui->image_list->removeItemWidget(item);
        ui->image_list->takeItem(row);
        del_list.pop_front();

        mFilePathList.removeAt(row);
        mFileNameList.removeAt(row);
    }

    mFileUtils->startDelete(mSelectionlist);
}

void ImageViewer::onDelAllClicked()
{
    mOperation = FileUtils::DELETE;

    if(mProgressViewer) {
        mProgressViewer->setOperation(mOperation);
    }
    ui->image_list->clear();
    mFileUtils->startDelete(mFilePathList);
    mFilePathList.clear();
    mFileNameList.clear();
}

void ImageViewer::onDiskItemClicked(int index)
{
    RLOGD("label:%s,nodepath:%s,mountpath:%s,filesystem:%s",mExternalStorageInfo[index].label.c_str(),
                      mExternalStorageInfo[index].node_path.c_str(),
                      mExternalStorageInfo[index].mount_path.c_str(),
                      mExternalStorageInfo[index].file_system.c_str());
    RLOGD("select count %d",mSelectionlist.count());
    QString dst_dir = QString::fromStdString(mExternalStorageInfo[index].mount_path);
    mFileUtils->startCopy(mSelectionlist,dst_dir);
    if(mDiskSelectionWidget)
        mDiskSelectionWidget->close();
    if(mProgressViewer) {
        mProgressViewer->setOperation(mOperation);
    }
}


