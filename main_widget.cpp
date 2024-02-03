#include "main_widget.h"
#include "ui_mainwidget.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "mpp_common.h"
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include "utils/configutils.h"
#include "widgets/powoffwidget.h"

#define MODULE_TAG "MainWidget"

using namespace std;

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
    , mKeyListener(KeyListener::get_instance())
    , mSignalHandler(sp<SignalHandler>(new SignalHandler(this)))
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
    if(!mMenuWidget) {
        mMenuWidget = sp<MenuWidget>(new MenuWidget(this));
        connect(mMenuWidget.get(),SIGNAL(onMenuEvent(MenuWidget::EventType)),this,SLOT(onMenuEventHandler(MenuWidget::EventType)));
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

    if (access(IMAGES_SAVE_DIR, F_OK)) {
        mkdir(IMAGES_SAVE_DIR);
    }

    if (access(VIDEOS_SAVE_DIR, F_OK)) {
        mkdir(VIDEOS_SAVE_DIR);
    }
    ui->NoSignalWidget->resize(this->size());
    ui->NoSignalWidget->move(0,0);

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
    QApplication::quit();
}

void MainWidget::onCapture()
{
    if(!signalIn)return;

    char time_str[50] = {0};
    getCurentTime(time_str,"%Y-%m-%d_%H-%M-%S");
    char file_name[50] = {0};
    char file_save_path[50] = {0};
    sprintf(file_name,"%s.jpg",time_str);
    sprintf(file_save_path,"%s/%s",IMAGES_SAVE_DIR,file_name);
    RLOGD("capture file path is %s",file_save_path);
    if(mVideoWidget) {
        QPixmap pix_map = mVideoWidget->grab();
        pix_map.save(file_save_path);
        mVideoWidget->showSnapShotEffect();
    }
}

void MainWidget::onRecord()
{
    if(!signalIn)return;
    if(mRecordWidget) {
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
}

void MainWidget::onUserSet()
{
    mUserSetWidget->setGeometry(this->geometry());
    mUserSetWidget->open();
}

void MainWidget::onCreateTask()
{
    mInputDevice.setVideoWidget(mVideoWidget.get());
    connect(&mInputDevice,&VideoInputDevice::signalChange,this,&MainWidget::signalChange);
    mInputDevice.initDevice(1);
    mInputDevice.start();

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
        if((!mVideoViewer&&mVideoViewer->isVisible())
                ||(!mImageViewer&&mImageViewer->isVisible())
                ||(!mUserSetWidget&&mUserSetWidget->isVisible())
                ||(!mPushWidget&&mPushWidget->isVisible()))
            return
        onRecord();
    }break;
    case KeyListener::Key_EventType_CAPTURE: {
        if((!mVideoViewer&&mVideoViewer->isVisible())
                ||(!mImageViewer&&mImageViewer->isVisible())
                ||(!mUserSetWidget&&mUserSetWidget->isVisible())
                ||(!mPushWidget&&mPushWidget->isVisible()))
            return
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
