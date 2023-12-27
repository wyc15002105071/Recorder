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
    BaseViewer(parent),
    ui(new Ui::VideoViewer)
    ,mVideoFrameReader(VideoFrameReader::Instance())
    ,mPlayer(shared_ptr<VideoPlayer>(new VideoPlayer))
    ,mIconHasUpdated(0)
{
    ui->setupUi(this);
    mFileType = FILE_TYPE_VIDEO;
    mListViewer = ui->video_list;
    mListViewer->setAttribute(Qt::WA_AcceptTouchEvents,true);
    mListViewer->verticalScrollBar()->setStyleSheet(mListViewer->styleSheet());
    QScroller::grabGesture(mListViewer,QScroller::TouchGesture);
    connect(mVideoFrameReader,SIGNAL(imageAvailable(QImage)),this,SLOT(onUpdateIcon(QImage)));
    connect(mDiskSelectionWidget.get(),SIGNAL(itemClicked(int)),this,SLOT(onDiskItemClicked(int)));
//    connect(mVideoFrameReader,SIGNAL(imagesAvailable(QList<QImage>)),this,SLOT(onImageIconsAvailable(QList<QImage>)));
}

VideoViewer::~VideoViewer()
{
    mListViewer->clear();
    delete ui;
}

void VideoViewer::onHasOpened()
{
    mListViewer->clear();
    mIconHasUpdated = 0;
    static QMovie movie(":/resources/icons/loading.gif");

    int count = mFilePathList.count();
    for(int i = 0;i < count;i++) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(QSize(ICON_WIDTH,ICON_HEIGHT));
        mListViewer->addItem(item);
        ListWidgetItem *custom_item = new ListWidgetItem;
        custom_item->setAlignment(Qt::AlignCenter);
        movie.setScaledSize(QSize(item->sizeHint().width() / 3,item->sizeHint().height() / 3));
        custom_item->setMovie(&movie);

        mListViewer->setItemWidget(item,custom_item);
        movie.start();
    }

    loadThumbnail(mFilePathList,0,mFilePathList.count());

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

void VideoViewer::open()
{
    findAllFiles(VIDEOS_SAVE_DIR);
    show();
}

void VideoViewer::loadThumbnail(QList<QString> files_path,int start,int end)
{
    if(files_path.count() < 0 || start > files_path.size() - 1)
        return;
    if(start < 0) {
        start = 0;
    }

    if((end > files_path.size()) && files_path.size() > 0)
        end = files_path.size();
    QList<QString> file_list;
    for(int i = start;i<end;i++) {
        file_list.push_back(files_path.at(i));
    }
    if(!mVideoFrameReader)
        mVideoFrameReader = VideoFrameReader::Instance();

    mVideoFrameReader->showFirstFrame(file_list);
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
        if(mPlayer)
            mPlayer->open(mFilePathList,index);
    }
}

void VideoViewer::onUpdateIcon(QImage image)
{
    QListWidgetItem *item = mListViewer->item(mIconHasUpdated);

    QString file_name = mFileNameList.at(mIconHasUpdated);
    if(!item) {
        item = new QListWidgetItem;
        mListViewer->addItem(item);
    }

    QPixmap file_icon;
    if(image.isNull()) {
        RLOGD("thumbnail read error,use default image");
        QImage broken_image(":/resources/icons/broken.png");
        file_icon = QPixmap::fromImage(broken_image.scaled(ICON_WIDTH - X_OFFSET,ICON_HEIGHT - Y_OFFSET,Qt::KeepAspectRatio));
    }else if(image.width() > ICON_WIDTH || image.height() > ICON_HEIGHT) {
        file_icon = QPixmap::fromImage(image.scaled(ICON_WIDTH - X_OFFSET,ICON_HEIGHT - Y_OFFSET,Qt::KeepAspectRatio));//保持长宽比例缩放
    }
    ListWidgetItem *custom_item = (ListWidgetItem *)mListViewer->itemWidget(item);
    custom_item->setIcon(file_icon);
    custom_item->setFileName(file_name);
    custom_item->setAlignment(Qt::AlignCenter);
    item->setSizeHint(QSize(ICON_WIDTH,ICON_HEIGHT));
    mListViewer->setItemWidget(item,custom_item);
    mIconHasUpdated++;
}

void VideoViewer::onUpdateIcons(QList<QImage> images)
{
    mListViewer->clear();
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
            mListViewer->addItem(item);
            mListViewer->setItemWidget(item,custom_item);
        }
    }

    mIconHasUpdated = images.size() - 1;
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
    if(!mVideoFrameReader)
        mVideoFrameReader = VideoFrameReader::Instance();
    mVideoFrameReader->stop();
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
