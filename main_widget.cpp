#include "main_widget.h"
#include "ui_mainwidget.h"
#include <QTimer>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

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
//    showFullScreen();
//    v4l2_device.setVideoWidget(ui->openGLWidget);
//    bool success = v4l2_device.init_device(1);


//    initWidgets();

//    v4l2_device.start();
//    QTimer *timer = new QTimer;
//    timer->start(2000);
//    connect(timer,SIGNAL(timeout()),this,SLOT(close()));
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::initWidgets(QResizeEvent *event)
{
    if(!mVideoWidget) {
        mVideoWidget = sp<QWidget>(ui->video_widget);
        if(!mRenderWidget)
            mRenderWidget = sp<VideoWidget>(new VideoWidget(ui->video_widget));
    }
    if(!mMenuWidget)
        mMenuWidget = sp<QWidget>(ui->menu_widget);

    mVideoWidget->resize(event->size());
    mVideoWidget->move(0,0);

    mRenderWidget->setGeometry(mVideoWidget->geometry());

    mMenuWidget->setFixedWidth(event->size().width());
    mMenuWidget->move(0,event->size().height()-mMenuWidget->height());

    if (access(IMAGES_SAVE_DIR, F_OK)) {
        mkdir(IMAGES_SAVE_DIR);
    }

    if (access(VIDEOS_SAVE_DIR, F_OK)) {
        mkdir(VIDEOS_SAVE_DIR);
    }
}

void MainWidget::resizeEvent(QResizeEvent *event)
{
    initWidgets(event);
}

void MainWidget::captureClicked()
{
//    char time_str[50] = {0};
//    getCurentTime(time_str,NULL);
//    char file_name[50] = {0};
//    char file_save_path[50] = {0};
//    sprintf(file_name,"%s.jpg",time_str);
//    sprintf(file_save_path,"%s/%s",IMAGES_SAVE_DIR,file_name);
//    QPixmap pix_map = ui->openGLWidget->grab();
//    pix_map.save(file_save_path);
//    ui->openGLWidget->showSnapShotEffect();
}

void MainWidget::recordChecked(bool checked)
{
//    if(checked) {
//        v4l2_device.startEnc();
//        ui->record->setEnabled(false);
//        ui->record_stop->setEnabled(true);
//        ui->record_timer_widget->start();
//    }
}

void MainWidget::recordStopClicked()
{
//    v4l2_device.stopEnc();
//    ui->record->setEnabled(true);
//    ui->record->setChecked(false);
//    ui->record_stop->setEnabled(false);
//    ui->record_timer_widget->stop();
}

void MainWidget::pictureFileClicked()
{
    mImageViewer->open();
    mImageViewer->showFullScreen();
}

void MainWidget::videoFileClicked()
{
    mVideoViewer->open();
    mVideoViewer->showFullScreen();
}

void MainWidget::settingClicked()
{
    mSettingWidget->show();
    mSettingWidget->showFullScreen();
}
