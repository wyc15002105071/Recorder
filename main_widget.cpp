#include "main_widget.h"
#include "ui_mainwidget.h"
#include <QTimer>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "mpp_common.h"

#define MODULE_TAG "MainWidget"

using namespace std;

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
    , mImageViewer(sp<ImageViewer>(new ImageViewer()))
    , mVideoViewer(sp<VideoViewer>(new VideoViewer()))
    , mSettingWidget(sp<SettingWidget>(new SettingWidget()))
{
    ui->setupUi(this);
    setWindowState(Qt::WindowFullScreen);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::initWidgets()
{
    if(!mVideoWidget_Container) {
        mVideoWidget_Container = sp<QWidget>(ui->videowidget_container);
        if(!mVideoWidget) {
            mVideoWidget = sp<VideoWidget>(new VideoWidget(ui->videowidget_container));
            connect(mVideoWidget.get(),SIGNAL(onVideoWidgetCreated()),this,SLOT(onCreateTask()),Qt::UniqueConnection);
        }
    }
    if(!mMenuWidget)
        mMenuWidget = sp<QWidget>(ui->menu_widget);

    mVideoWidget->resize(this->size());
    mVideoWidget->move(0,0);

    mVideoWidget_Container->setGeometry(mVideoWidget->geometry());

    mMenuWidget->setFixedWidth(this->size().width());
    mMenuWidget->move(0,this->size().height()-mMenuWidget->height());

    if(!mRecordWidget) {
        mRecordWidget = sp<RecordWidget>(new RecordWidget(this,&mInputDevice));
        connect(mRecordWidget.get(),&RecordWidget::onClosed,this,[=]{ if(mMenuWidget) mMenuWidget->show(); },Qt::UniqueConnection);
    }

    mRecordWidget->setGeometry(this->rect());
    mRecordWidget->close();

    if (access(IMAGES_SAVE_DIR, F_OK)) {
        mkdir(IMAGES_SAVE_DIR);
    }

    if (access(VIDEOS_SAVE_DIR, F_OK)) {
        mkdir(VIDEOS_SAVE_DIR);
    }
}

void MainWidget::resizeEvent(QResizeEvent *event)
{
    initWidgets();
}

void MainWidget::destroy()
{

}

void MainWidget::onCaptureClicked()
{
    char time_str[50] = {0};
    getCurentTime(time_str,"%Y-%m-%d_%H-%M-%S");
    char file_name[50] = {0};
    char file_save_path[50] = {0};
    sprintf(file_name,"%s.jpg",time_str);
    sprintf(file_save_path,"%s/%s",IMAGES_SAVE_DIR,file_name);
    RLOGD("capture file path is %s",file_save_path);
    if(mVideoWidget) {
        QPixmap pix_map = mVideoWidget->grab();
        mVideoWidget->showSnapShotEffect();
        pix_map.save(file_save_path);
    }
}

void MainWidget::onRecordClicked()
{
    RLOGD("ready to open record widget");
    if(mRecordWidget) {
        mMenuWidget->close();
        mRecordWidget->open();
    }

//    static bool isenc = false;
//    isenc = !isenc;
//    if(isenc)
//        mInputDevice.startRecord();
//    else
//        mInputDevice.stopRecord();
}

void MainWidget::recordChecked(bool checked)
{

}

void MainWidget::recordStopClicked()
{
//    v4l2_device.stopEnc();
//    ui->record->setEnabled(true);
//    ui->record->setChecked(false);
//    ui->record_stop->setEnabled(false);
//    ui->record_timer_widget->stop();
}

void MainWidget::onPictureFileClicked()
{
    mImageViewer->open();
}

void MainWidget::onVideoFileClicked()
{
    mVideoViewer->open();
}

void MainWidget::settingClicked()
{
    mSettingWidget->show();
}

void MainWidget::onCreateTask()
{
    mInputDevice.setVideoWidget(mVideoWidget.get());
    mInputDevice.initDevice(1);
    mInputDevice.start();
}
