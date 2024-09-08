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
    ,mIconIndex(0)
{
    ui->setupUi(this);
    mFileType = FILE_TYPE_VIDEO;

    connect(mVideoFrameReader,SIGNAL(imageAvailable(QImage)),this,SLOT(onUpdateIcon(QImage)));
    connect(mDiskSelectionWidget.get(),SIGNAL(itemClicked(int)),this,SLOT(onDiskItemClicked(int)));
//    connect(mVideoFrameReader,SIGNAL(imagesAvailable(QList<QImage>)),this,SLOT(onImageIconsAvailable(QList<QImage>)));
}

VideoViewer::~VideoViewer()
{
    ui->video_list->clear();
    delete ui;
}

void VideoViewer::onHasOpened()
{
    ui->video_list->clear();
    mIconIndex = 0;
    static QMovie movie(":/resources/images/loading.gif");
    int count = mFilePathList.count();
    for(int i = 0;i < count;i++) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(QSize(ICON_WIDTH,ICON_HEIGHT));
        ui->video_list->addItem(item);
        ListWidgetItem *custom_item = new ListWidgetItem;
        custom_item->setAlignment(Qt::AlignCenter);
        movie.setScaledSize(QSize(item->sizeHint().width() / 3,item->sizeHint().height() / 3));
        custom_item->setMovie(&movie);

        ui->video_list->setItemWidget(item,custom_item);
        movie.start();
    }

    if(!mVideoFrameReader)
        mVideoFrameReader = VideoFrameReader::Instance();
    mVideoFrameReader->showFirstFrame(mFilePathList);
    if(mProgressViewer) {
        mProgressViewer->close();
    }
}

void VideoViewer::onHasClosed()
{
    if(!mVideoFrameReader)
        mVideoFrameReader = VideoFrameReader::Instance();
    mVideoFrameReader->stop();
    if(mDiskSelectionWidget)
        mDiskSelectionWidget->close();
    mSelectMode = false;
    ui->selectMode_btn->setChecked(mSelectMode);

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
        QString name = mFilePathList.at(index);
        QString video_path = "file://"+name;
        if(mPlayer)
            mPlayer->open(mFilePathList,index);
    }
}

void VideoViewer::onUpdateIcon(QImage image)
{
    QListWidgetItem *item = ui->video_list->item(mIconIndex);

    QString file_name = mFileNameList.at(mIconIndex);
    if(!item) {
        item = new QListWidgetItem;
        ui->video_list->addItem(item);
    }

    QPixmap file_icon;
    if(image.isNull()) {
        LOG("thumbnail read error,use default image");
        QImage broken_image(":/resources/icons/broken.png");
        file_icon = QPixmap::fromImage(broken_image.scaled(ICON_WIDTH - X_OFFSET,ICON_HEIGHT - Y_OFFSET,Qt::KeepAspectRatio));
    }else if(image.width() > ICON_WIDTH || image.height() > ICON_HEIGHT) {
        file_icon = QPixmap::fromImage(image.scaled(ICON_WIDTH - X_OFFSET,ICON_HEIGHT - Y_OFFSET,Qt::KeepAspectRatio));//保持长宽比例缩放
    }
    ListWidgetItem *custom_item = (ListWidgetItem *)ui->video_list->itemWidget(item);
    custom_item->setIcon(file_icon);
    custom_item->setFileName(file_name);
    custom_item->setAlignment(Qt::AlignCenter);
    item->setSizeHint(QSize(ICON_WIDTH,ICON_HEIGHT));
    ui->video_list->setItemWidget(item,custom_item);
    mIconIndex++;
}

void VideoViewer::onUpdateIcons(QList<QImage> images)
{
    ui->video_list->clear();
    for(int i = 0;i<images.size();i++)
    {
        QListWidgetItem *item = new QListWidgetItem;
        QString file_name = mFileNameList.at(mIconIndex);
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

    mIconIndex = images.size() - 1;
}

void VideoViewer::onSelectModeToggled(bool toggled)
{
    mSelectionlist.clear();
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

void VideoViewer::onCopySelectedClicked()
{
    mSelectionlist.clear();

    for(int i = 0;i<ui->video_list->count();i++) {
        QListWidgetItem *item = ui->video_list->item(i);
        ListWidgetItem *item_widget = (ListWidgetItem *)ui->video_list->itemWidget(item);

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
    for(int i = 0;i<ui->video_list->count();i++) {
        QListWidgetItem *item = ui->video_list->item(i);
        ListWidgetItem *item_widget = (ListWidgetItem *)ui->video_list->itemWidget(item);

        if(!item_widget)
            continue;
        if(item_widget->isSelected()) {
            mSelectionlist.push_back(mFilePathList.at(i));
            del_list.push_back(item);
        }
    }

    while(del_list.count() > 0) {
        QListWidgetItem *item = del_list.front();

        int row = ui->video_list->row(item);
        ui->video_list->removeItemWidget(item);
        ui->video_list->takeItem(row);
        del_list.pop_front();

        mFilePathList.removeAt(row);
        mFileNameList.removeAt(row);
    }

    mFileUtils->startDelete(mSelectionlist);
}

void VideoViewer::onDelAllClicked()
{
    mOperation = FileUtils::DELETE;
    if(!mVideoFrameReader)
        mVideoFrameReader = VideoFrameReader::Instance();
    mVideoFrameReader->stop();
    if(mProgressViewer) {
        mProgressViewer->setOperation(mOperation);
    }
    ui->video_list->clear();
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
    QString dst_dir = QString::fromStdString(mExternalStorageInfo[index].mount_path);
    mFileUtils->startCopy(mSelectionlist,dst_dir);
    if(mDiskSelectionWidget)
        mDiskSelectionWidget->close();
    if(mProgressViewer) {
        mProgressViewer->setOperation(mOperation);
    }
}
