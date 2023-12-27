#include "main_widget.h"
#include "ui_mainwidget.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "mpp_common.h"
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

#define MODULE_TAG "MainWidget"

using namespace std;

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
    , mKeyListener(KeyListener::get_instance())
    , mImageViewer(sp<ImageViewer>(new ImageViewer()))
    , mVideoViewer(sp<VideoViewer>(new VideoViewer()))
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

    mVideoWidget->resize(this->size());
    mVideoWidget->move(0,0);

    mVideoWidget_Container->setGeometry(mVideoWidget->geometry());

    mMenuWidget->resize(this->size().width(),mMenuWidget->height());
    mMenuWidget->move(0,this->size().height()-mMenuWidget->height());
    mMenuWidget->open();

    mRecordWidget->setGeometry(this->rect());
    mRecordWidget->close();

    connect(mKeyListener,SIGNAL(onPressed(KeyListener::EventType)),this,SLOT(onKeyEventHandler(KeyListener::EventType)),Qt::UniqueConnection);

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

void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(mRecordWidget->isVisible()) {
        return;
    }
    mMenuWidget->open();
}

void MainWidget::mousePressEvent(QMouseEvent *event)
{
    if(mRecordWidget->isVisible()) {
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
    RLOGD("start push stream...");
}

void MainWidget::onOpenImageBrowser()
{
    mImageViewer->open();
}

void MainWidget::onOpenVideoBrowser()
{
    mVideoViewer->open();
}

void MainWidget::onCreateTask()
{
    mInputDevice.setVideoWidget(mVideoWidget.get());
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
    default:
        break;
    }
}

void MainWidget::onKeyEventHandler(KeyListener::EventType type)
{
    switch (type)
    {
    case KeyListener::Key_EventType_RECORD: {
        onRecord();
    }break;
    case KeyListener::Key_EventType_CAPTURE: {
        onCapture();
    }break;
    default:
        break;
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
