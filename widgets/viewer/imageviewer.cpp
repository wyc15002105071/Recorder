#include "imageviewer.h"
#include "ui_imageviewer.h"
#include <QLabel>

ImageViewer::ImageViewer(QWidget *parent) :
    AbStractViewer(parent),
    ui(new Ui::ImageViewer)
{
    ui->setupUi(this);
    mFileType = FILE_TYPE_IMAGE;

    connect(ui->back_btn,SIGNAL(clicked()),this,SLOT(close()));
}

ImageViewer::~ImageViewer()
{
    delete ui;
}

void ImageViewer::open()
{
    findAllFiles(IMAGES_SAVE_DIR);

    for(int i = 0;i<mFileList.size();i++) {
        QListWidgetItem *item = new QListWidgetItem;
        QPixmap file_icon;
        QImage image(mFileList.at(i));

        file_icon = QPixmap::fromImage(image);
        if(image.width() > 150 || image.height() > 150) {
            file_icon = QPixmap::fromImage(image.scaled(150,150,Qt::KeepAspectRatio));//保持长宽比例缩放
        }
        QLabel *image_label = new QLabel;
        image_label->setPixmap(file_icon);
        image_label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        item->setSizeHint(QSize(file_icon.width() + 20,file_icon.height() + 20));
        ui->image_list->addItem(item);
        ui->image_list->setItemWidget(item,image_label);
    }

    show();
    showFullScreen();
}
