#include "imageviewer.h"
#include "ui_imageviewer.h"
#include <QLabel>
#include "listwidgetitem.h"

ImageViewer::ImageViewer(QWidget *parent) :
    BaseViewer(parent),
    ui(new Ui::ImageViewer),
    mImageBrowser(sp<ImageBrowser>(new ImageBrowser()))
{
    ui->setupUi(this);
    mFileType = FILE_TYPE_IMAGE;
}

void ImageViewer::showEvent(QShowEvent *event)
{
    ui->image_list->clear();
    for(int i = 0;i<mFileList.size();i++) {
        QListWidgetItem *item = new QListWidgetItem;
        QPixmap file_icon;
        QImage image(mFileList.at(i));

        file_icon = QPixmap::fromImage(image);
        if(image.width() > ICON_WIDTH || image.height() > ICON_HEIGHT) {
            file_icon = QPixmap::fromImage(image.scaled(ICON_WIDTH,ICON_HEIGHT,Qt::KeepAspectRatio));//保持长宽比例缩放
        }
        ListWidgetItem *custom_item = new ListWidgetItem;
        custom_item->setIcon(file_icon);
        custom_item->setFileName(mFileNameList.at(i));
        custom_item->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        item->setSizeHint(QSize(file_icon.width() + X_OFFSET,file_icon.height() + Y_OFFSET));
        ui->image_list->addItem(item);
        ui->image_list->setItemWidget(item,custom_item);
    }
}

ImageViewer::~ImageViewer()
{
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
            mImageBrowser->open(mFileList,current_index);
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
