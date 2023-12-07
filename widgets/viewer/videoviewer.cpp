#include "videoviewer.h"
#include "ui_videoviewer.h"
#include <QLabel>
#include <QLabel>

using namespace std;

VideoViewer::VideoViewer(QWidget *parent) :
    BaseViewer(parent),
    ui(new Ui::VideoViewer)
    ,mVideoFrameReader(VideoFrameReader::Instance())
    ,mPlayer(shared_ptr<VideoPlayer>(new VideoPlayer))
{
    ui->setupUi(this);
    mFileType = FILE_TYPE_VIDEO;

    connect(ui->back_btn,SIGNAL(clicked()),this,SLOT(close()));

    connect(mVideoFrameReader,&VideoFrameReader::Icon_image_available,this,[=](QList<QImage>icons){
        ui->video_list->clear();
        for(int i = 0;i<icons.size();i++)
        {
            QListWidgetItem *item = new QListWidgetItem;

            if(icons[i].isNull())
                item->setSizeHint(QSize(100,150));
            else
            {
                QPixmap file_icon;
                if(icons[i].width() > 150 || icons[i].height() > 150) {
                    file_icon = QPixmap::fromImage(icons[i].scaled(150,150,Qt::KeepAspectRatio));//保持长宽比例缩放
                }
                item->setSizeHint(QSize(file_icon.width() + 20,file_icon.height() + 20));
                QLabel *image_label = new QLabel;
                image_label->setPixmap(file_icon);
                image_label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                ui->video_list->addItem(item);
                ui->video_list->setItemWidget(item,image_label);
            }
        }

    });
}

VideoViewer::~VideoViewer()
{
    delete ui;
}

void VideoViewer::open()
{
    findAllFiles(VIDEOS_SAVE_DIR);

//    for(int i = 0;i<mFileList.size();i++) {
//        QListWidgetItem *item = new QListWidgetItem;
//        QPixmap file_icon;

//        Metadata meta = ThumbnailUtils::getThumbnail(mFileList.at(i).toLatin1().data());
//        QImage image = QImage::fromData(meta.pData.get(),meta.size);;

//        QLabel *image_label = new QLabel;
//        image_label->setPixmap(QPixmap::fromImage(image));
//        item->setSizeHint(QSize(file_icon.width() + 20,file_icon.width() + 20));
//        ui->video_list->addItem(item);
//        ui->video_list->setItemWidget(item,image_label);
//    }

    show();
    showFullScreen();

    mVideoFrameReader = VideoFrameReader::Instance();
    mVideoFrameReader->showFirstFrame(mFileList);

}

void VideoViewer::itemDoubleClicked(QListWidgetItem *item)
{
    int index = ui->video_list->currentRow();
    QString name = mFileList.at(index);
    QString video_path = "file://"+name;
    mPlayer->playVideo(video_path);
}
