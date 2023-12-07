#include "videoviewer.h"
#include "ui_videoviewer.h"
#include <QLabel>
#include <QMovie>
#include "listwidgetitem.h"

#define MODULE_TAG "VideoViewer"
using namespace std;

VideoViewer::VideoViewer(QWidget *parent) :
    BaseViewer(parent),
    ui(new Ui::VideoViewer)
    ,mVideoFrameReader(VideoFrameReader::Instance())
    ,mPlayer(shared_ptr<VideoPlayer>(new VideoPlayer))
    ,mIconHasUpdated(0)
{
    ui->setupUi(this);
    mFileType = FILE_TYPE_VIDEO;

    connect(mVideoFrameReader,SIGNAL(imageAvailable(QImage)),this,SLOT(onUpdateIcon(QImage)));
//    connect(mVideoFrameReader,SIGNAL(imagesAvailable(QList<QImage>)),this,SLOT(onImageIconsAvailable(QList<QImage>)));
}

VideoViewer::~VideoViewer()
{
    delete ui;
}

void VideoViewer::showEvent(QShowEvent *event)
{
    ui->video_list->clear();
    mIconHasUpdated = 0;
    static QMovie movie(":/resources/icons/loading.gif");
    int count = mFileList.count();
    for(int i = 0;i < count;i++) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(QSize(ICON_WIDTH,ICON_HEIGHT));
        ui->video_list->addItem(item);
        ListWidgetItem *custom_item = new ListWidgetItem;
        custom_item->setAlignment(Qt::AlignHCenter);
        movie.setScaledSize(QSize(item->sizeHint().width() / 3,item->sizeHint().height() / 3));
        custom_item->setMovie(&movie);

        ui->video_list->setItemWidget(item,custom_item);
        movie.start();
    }

    mVideoFrameReader = VideoFrameReader::Instance();
    mVideoFrameReader->showFirstFrame(mFileList);
}

void VideoViewer::open()
{
    findAllFiles(VIDEOS_SAVE_DIR);
    show();
}

void VideoViewer::onItemClicked(QListWidgetItem *item)
{
    if(mSelectMode) {
        ListWidgetItem *item_widget = (ListWidgetItem *)ui->video_list->itemWidget(item);
        if(!item_widget)
            return;
        bool is_selected = item_widget->isSelected();
        item_widget->setSelected(!is_selected);
    } else {
        int index = ui->video_list->currentRow();
        QString name = mFileList.at(index);
        QString video_path = "file://"+name;
        if(mPlayer)
            mPlayer->open(mFileList,index);
    }
}

void VideoViewer::onUpdateIcon(QImage image)
{
    QListWidgetItem *item = ui->video_list->item(mIconHasUpdated);

    QString file_name = mFileNameList.at(mIconHasUpdated);
    if(!item) {
        item = new QListWidgetItem;
        ui->video_list->addItem(item);
    }
    if(image.isNull())
        item->setSizeHint(QSize(ICON_WIDTH,ICON_HEIGHT));
    else
    {
        QPixmap file_icon;
        if(image.width() > ICON_WIDTH || image.height() > ICON_HEIGHT) {
            file_icon = QPixmap::fromImage(image.scaled(ICON_WIDTH,ICON_HEIGHT,Qt::KeepAspectRatio));//保持长宽比例缩放
        }
        item->setSizeHint(QSize(file_icon.width() + 20,file_icon.height() + 20));
        ListWidgetItem *custom_item = new ListWidgetItem;
        custom_item->setIcon(file_icon);
        custom_item->setFileName(file_name);
        custom_item->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->video_list->setItemWidget(item,custom_item);
    }
    mIconHasUpdated++;
}

void VideoViewer::onUpdateIcons(QList<QImage> images)
{
    ui->video_list->clear();
    for(int i = 0;i<images.size();i++)
    {
        QListWidgetItem *item = new QListWidgetItem;
        QString file_name = mFileNameList.at(mIconHasUpdated);
        if(images[i].isNull())
            item->setSizeHint(QSize(ICON_WIDTH,ICON_HEIGHT));
        else
        {
            QPixmap file_icon;
            if(images[i].width() > 150 || images[i].height() > 150) {
                file_icon = QPixmap::fromImage(images[i].scaled(150,150,Qt::KeepAspectRatio));//保持长宽比例缩放
            }
            item->setSizeHint(QSize(file_icon.width() + 20,file_icon.height() + 20));
            ListWidgetItem *custom_item = new ListWidgetItem;
            custom_item->setIcon(file_icon);
            custom_item->setFileName(file_name);
            custom_item->setAlignment(Qt::AlignHCenter);
            ui->video_list->addItem(item);
            ui->video_list->setItemWidget(item,custom_item);
        }
    }

    mIconHasUpdated = images.size() - 1;
}

void VideoViewer::onSelectModeToggled(bool toggled)
{
    int count = ui->video_list->count();
    if(count <= 0)
        return;
    mSelectMode = toggled;
    for(int i = 0;i < count;i++) {
        QListWidgetItem *item = ui->video_list->item(i);
        if(!item)
            continue;
        ListWidgetItem *item_widget = (ListWidgetItem *)ui->video_list->itemWidget(item);
        item_widget->setSelectable(toggled);
    }
}

