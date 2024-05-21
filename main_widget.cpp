#include "main_widget.h"
#include "ui_mainwidget.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "mpp_common.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "widgets/powoffwidget.h"
#include "utils/configutils.h"
#include "utils/toastutils.h"
#include "utils/mediapathutils.h"

#define MODULE_TAG "MainWidget"
#define LAST_SPACE 1024
using namespace std;

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
    , mKeyListener(KeyListener::get_instance())
    , mDiskListener(new DiskCapacityListener())
    , mStorageUtils(StorageUtils::get_instance())
    , mHotplugListener(sp<HotplugListener>(new HotplugListener))
    , mSignalHandler(sp<SignalHandler>(new SignalHandler(this)))
    , mMediaRecorder(sp<MediaRecorder>(new MediaRecorder()))
{
    ui->setupUi(this);
    setWindowState(Qt::WindowFullScreen);

    mHotplugListener->attach(this);
    connect(this,SIGNAL(onHotplugEvent(bool)),this,SLOT(checkUsb(bool)));

    mHotplugListener->startTask();

    connect(mDiskListener,&DiskCapacityListener::sendDiskSpace,this,&MainWidget::sendDiskSpace);
    mDiskListener->startTask();
}

MainWidget::~MainWidget()
{
    if(mDiskListener)mDiskListener->stopTask();
    delete ui;
}

void MainWidget::onShowChange(bool isShow)
{
    ui->widget_time->setVisible(isShow);
}

void MainWidget::initWidgets()
{
    if(!mMenuWidget) {
        mMenuWidget = sp<MenuWidget>(new MenuWidget(this));
        connect(mMenuWidget.get(),SIGNAL(onMenuEvent(MenuWidget::EventType)),this,SLOT(onMenuEventHandler(MenuWidget::EventType)));
        connect(mMenuWidget.get(),SIGNAL(onShowChange(bool)),this,SLOT(onShowChange(bool)));
    }
    if(!mVideoWidget_Container) {
        mVideoWidget_Container = sp<QWidget>(ui->videowidget_container);
        if(!mVideoWidget) {
            mVideoWidget = sp<VideoWidget>(new VideoWidget(ui->videowidget_container));
            connect(mVideoWidget.get(),SIGNAL(onVideoWidgetCreated()),this,SLOT(onCreateTask()),Qt::UniqueConnection);
        }
    }

    if(!mRecordWidget) {
        mRecordWidget = sp<RecordWidget>(new RecordWidget(this,&mInputDevice));
        connect(mRecordWidget.get(),&RecordWidget::onClosed,this,[=]{ if(mMenuWidget) mMenuWidget->show(); },Qt::UniqueConnection);
    }
    if(!mPushWidget) {
        mPushWidget = sp<PushStreamWidget>(new PushStreamWidget(this,&mInputDevice));
        connect(mPushWidget.get(),&PushStreamWidget::onClosed,this,[=]{ if(mMenuWidget) mMenuWidget->show(); },Qt::UniqueConnection);
    }


    mVideoWidget->resize(this->size());
    mVideoWidget->move(0,0);

    mVideoWidget_Container->setGeometry(mVideoWidget->geometry());

    mMenuWidget->resize(this->size().width(),mMenuWidget->height());
    mMenuWidget->move(0,this->size().height()-mMenuWidget->height());
    mMenuWidget->open();

    mRecordWidget->setGeometry(this->rect());
    mRecordWidget->close();

    mPushWidget->resize(this->size().width(),mPushWidget->height());
    mPushWidget->move(0,this->size().height()-mPushWidget->height());
    mPushWidget->close();

    if(!mImageViewer) {
        mImageViewer = sp<ImageViewer>(new ImageViewer(this));

        mImageViewer->setGeometry(this->geometry());
        mImageViewer->close();
    }

    if(!mVideoViewer) {
        mVideoViewer = sp<VideoViewer>(new VideoViewer(this));
        mVideoViewer->setGeometry(this->geometry());
        mVideoViewer->showFullScreen();
        mVideoViewer->close();
    }
    if(!mUserSetWidget){
        mUserSetWidget = sp<UserSetWidget>(new UserSetWidget(this));
        mUserSetWidget->setGeometry(this->geometry());
        mUserSetWidget->close();
    }
    connect(mKeyListener,SIGNAL(onPressed(KeyListener::EventType)),this,SLOT(onKeyEventHandler(KeyListener::EventType)),Qt::UniqueConnection);

    mHotplugListener->attach(mRecordWidget.get());
    mHotplugListener->attach(mVideoViewer.get());
    mHotplugListener->attach(mImageViewer.get());


    if (access(IMAGES_SAVE_DIR, F_OK)) {
        mkdir(IMAGES_SAVE_DIR);
    }

    if (access(VIDEOS_SAVE_DIR, F_OK)) {
        mkdir(VIDEOS_SAVE_DIR);
    }
    ui->NoSignalWidget->resize(this->size());
    ui->NoSignalWidget->move(0,0);

    //ui->widget_time->move((this->width()-ui->widget_time->width())/2,20);
    ui->widget_time->move(0,20);
}

void MainWidget::resizeEvent(QResizeEvent *event)
{
    initWidgets();
}

void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(mRecordWidget->isVisible()) {
        return;
    }
    mMenuWidget->open();
}

void MainWidget::mousePressEvent(QMouseEvent *event)
{
    if(mRecordWidget->isVisible() || mPushWidget->isVisible()) {
        return;
    }
    mMenuWidget->open();
}

void MainWidget::quit()
{
    mInputDevice.stopTask();
    mAudioRender.stopTask();

    if(mVideoWidget) {
        mVideoWidget->deleteLater();
        mVideoWidget = nullptr;
    }

    if(mImageViewer) {
        mImageViewer->close();
        mImageViewer->deleteLater();
        mImageViewer = nullptr;
    }

    if(mVideoViewer) {
        mVideoViewer->close();
        mVideoViewer->deleteLater();
        mVideoViewer = nullptr;
    }

    RLOGD("exit...");
    //    QApplication::quit();
    exit(0);
}

void MainWidget::onCaptureFinished()
{
    if(mVideoWidget) {
        mVideoWidget->showSnapShotEffect();
    }
}

void MainWidget::onCapture()
{
    if(!signalIn)return;
    if(space<LAST_SPACE){
        ToastUtils::instance().show(ToastUtils::INFO,"储存空间不足");
        return;
    }

//    if(mVideoWidget) {
//        QPixmap pix_map = mVideoWidget->grab();
//        //pix_map.save(file_save_path);
//        pix_map.save(MediaPathUtils::get_instance()->getImagePath());
//        mVideoWidget->showSnapShotEffect();
//    }

      mInputDevice.capture();

}

void MainWidget::onRecord()
{
    if(!signalIn)return;
    if(space<LAST_SPACE){
        ToastUtils::instance().show(ToastUtils::INFO,tr("储存空间不足"));
        return;
    }
    if(mRecordWidget) {
        if(mRecordWidget->getSureDialogShow())
            return;

        if(!mRecordWidget->isVisible()) {
            RLOGD("ready to open record widget");
            mMenuWidget->close();
            mRecordWidget->open();
        } else {
            RLOGD("ready to close record widget");
            mRecordWidget->close();
            mMenuWidget->open();
        }
    }
}

void MainWidget::onPushSteam()
{
    if(!signalIn)return;
    RLOGD("start push stream...");
    if(mPushWidget) {
        //mMenuWidget->close();
        mPushWidget->open();
    }
}

void MainWidget::onOpenImageBrowser()
{
    mImageViewer->setGeometry(this->geometry());
    mImageViewer->open();
}

void MainWidget::onOpenVideoBrowser()
{
    mVideoViewer->setGeometry(this->geometry());
    mVideoViewer->open();
}

void MainWidget::onPower()
{
    if(ConfigUtils::isPowOff)return;
    ConfigUtils::isPowOff = true;

    PowOffWidget *p = new PowOffWidget;
    p->showFullScreen();

    if(mRecordWidget) {
        if(mRecordWidget->isVisible()) {
            RLOGD("ready to close record widget");
            mRecordWidget->close();
            mMenuWidget->open();
        }
    }
    if(mPushWidget){
        mPushWidget->onStopPush();
        mPushWidget->close();
    }
    if(mKeyListener) {
        mKeyListener->doSendWork(QByteArray().append(0x5A).append(0xA5)
                                 .append(0x04).append(0x01)
                                 .append(0x02).append(0xBF)
                                 .append(0xEF).append(0xEF));
    }
    system("poweroff");
}

void MainWidget::onUserSet()
{
    mUserSetWidget->setGeometry(this->geometry());
    mUserSetWidget->open();
}

void MainWidget::onCreateTask()
{
    {
        mInputDevice.setVideoWidget(mVideoWidget.get());
        connect(&mInputDevice,&VideoInputDevice::signalChange,this,&MainWidget::signalChange);
        connect(&mInputDevice,&VideoInputDevice::onCaptureFinished,this,&MainWidget::onCaptureFinished);
        mInputDevice.setRecorder(mMediaRecorder);
        mInputDevice.initDevice(1);
        mInputDevice.start();
    }

    {
        mAudioRender.setOutVolume(0.5f);
        mAudioRender.startTask();
    }

    if(mKeyListener) {
        mKeyListener->startTask();
    }
}

void MainWidget::onMenuEventHandler(MenuWidget::EventType type)
{
    switch(type) {
    case MenuWidget::Menu_EventType_Capture:
        onCapture();
        break;
    case MenuWidget::Menu_EventType_Record:
        onRecord();
        break;
    case MenuWidget::Menu_EventType_PushStream:
        onPushSteam();
        break;
    case MenuWidget::Menu_EventType_ImageBrowse:
        onOpenImageBrowser();
        break;
    case MenuWidget::Menu_EventType_VideoBrowse:
        onOpenVideoBrowser();
        break;
    case MenuWidget::Menu_EventType_UserSet:
        onUserSet();
        break;
    default:
        break;
    }
}

void MainWidget::onKeyEventHandler(KeyListener::EventType type)
{
    switch (type)
    {
    case KeyListener::Key_EventType_RECORD: {
        //if((mVideoViewer&&mVideoViewer->isVisible())
        //        ||(mImageViewer&&mImageViewer->isVisible())
        //        ||(mUserSetWidget&&mUserSetWidget->isVisible())
        //        ||(mPushWidget&&mPushWidget->isVisible()))
        //    return;
        if(!signalIn)return;
        if(mVideoViewer&&mVideoViewer->isVisible()){
            if(mVideoViewer->isFileUtilsRun())
                return;
            else
                mVideoViewer->close();
        }
        if(mImageViewer&&mImageViewer->isVisible()){
            if(mImageViewer->isFileUtilsRun())
                return;
            else
                mImageViewer->close();
        }
        if(mUserSetWidget&&mUserSetWidget->isVisible())mUserSetWidget->close();
        if(mPushWidget&&mPushWidget->isVisible())mPushWidget->close();
        if(mSureDialog&&mSureDialog->isVisible())mSureDialog->reject();
        if(mConfirmDialog&&mConfirmDialog->isVisible())return;
        onRecord();
    }break;
    case KeyListener::Key_EventType_CAPTURE: {
        if((mVideoViewer&&mVideoViewer->isVisible()&&mVideoViewer->isPlay())
                //        ||(mImageViewer&&mImageViewer->isVisible())
                //        ||(mUserSetWidget&&mUserSetWidget->isVisible())
                //        ||(mPushWidget&&mPushWidget->isVisible())
                )
            return;
        onCapture();
    }break;
    case KeyListener::Key_EventType_POWER: {
        onPower();
    }break;
    default:
        break;
    }
}

void MainWidget::signalChange(bool has)
{
    //if(has==signalIn)return;
    if(has){
        RLOGE("signal");
        signalIn = true;
        ui->NoSignalWidget->setVisible(false);
        if(mMenuWidget){
            mMenuWidget->setButtonDisabled(false);
        }
    }else{

        RLOGE("no signal");
        ui->NoSignalWidget->setVisible(true);
        mMenuWidget->setButtonDisabled(true);
        signalIn = false;
        if(mRecordWidget) {
            if(mRecordWidget->isVisible()) {
                RLOGD("ready to close record widget");
                mRecordWidget->close();
                mMenuWidget->open();
            }
        }
        if(mPushWidget){
            mPushWidget->onStopPush();
            mPushWidget->close();
        }
    }
}

void MainWidget::sendDiskSpace(long free, long total)
{
    //qDebug()<<free<<total;
    space = free;

    if(space<LAST_SPACE){
        ui->lb_time->setText("0m");
        if(mRecordWidget) {
            if(mRecordWidget->isVisible()) {
                RLOGD("ready to close record widget");
                mRecordWidget->close();
                mMenuWidget->open();
            }
        }
    }else{
        int min = (space-LAST_SPACE)/450;
        ui->lb_time->setText(QString("%1h%2m").arg(min/60).arg(min%60));
    }
}

void MainWidget::checkUsb(bool isNoLagel)
{
    mux.lock();
    if(isNoLagel){
        if(!mSureDialog){
            mSureDialog = new SureDialog;
            mSureDialog->setText("系统不支持该u盘格式");
        }
        if(!mSureDialog->isVisible()&&(!mConfirmDialog||!mConfirmDialog->isVisible()))
            mSureDialog->showFullScreen();
    }
    if(!ConfigUtils::isUsbMedia){
        if(!mConfirmDialog){
            mConfirmDialog = new ConfirmDialog;
            mConfirmDialog->setTitle("是否录制到u盘?");
            connect(mConfirmDialog,&ConfirmDialog::accepted,this,[=]{
                QVector<StorageUtils::ExternalStorageInfo> list = StorageUtils::get_instance()->getExternalStorageInfoList();
                for(int i=0;i<list.count();i++){
                    if(!ConfigUtils::isUsbMedia){
                        ConfigUtils::isUsbMedia = true;
                        ConfigUtils::usbName = QString::fromStdString(list.at(i).node_path);
                        ConfigUtils::usbLabel = QString::fromStdString(list.at(i).label);
                        ConfigUtils::usbMonut = QString::fromStdString(list.at(i).mount_path);
                    }
                }
            });
        }
        if((!mSureDialog||!mSureDialog->isVisible())&&!mConfirmDialog->isVisible())
            mConfirmDialog->showFullScreen();
    }
    mux.unlock();
}


int SignalHandler::sigintFd[2];
SignalHandler::SignalHandler(QObject *parent)
{
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sigintFd))
        RLOGE("Couldn't create INT socketpair");
    mSnInt = sp<QSocketNotifier>(new QSocketNotifier(sigintFd[1], QSocketNotifier::Read, this));
    mParent = (MainWidget *)parent;
    connect(mSnInt.get(), SIGNAL(activated(int)), this, SLOT(handleSignalInt()),Qt::UniqueConnection);
}

SignalHandler::~SignalHandler()
{
    disconnect(mSnInt.get(), SIGNAL(activated(int)), this, SLOT(handleSignalInt()));
}

void SignalHandler::intSignalHandler(int unused)
{
    char a = 1;
    ::write(sigintFd[0], &a, sizeof(a));
}

void SignalHandler::handleSignalInt()
{
    mSnInt->setEnabled(false);
    char tmp;
    ::read(sigintFd[1], &tmp, sizeof(tmp));
    if(mParent) {
        MainWidget *main = (MainWidget *)mParent;
        main->quit();
    }

    mSnInt->setEnabled(true);
}
