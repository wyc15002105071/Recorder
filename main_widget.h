#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QObject>
#include <QSocketNotifier>
#include <signal.h>
#include <QWidget>
#include <QPushButton>
#include <QResizeEvent>

#include "common/common.h"
#include "common/log.h"

#include "media/mediautils.h"
#include "media/videoinputdevice.h"
#include "media/mediarecorder.h"

#include "widgets/record_timer_widget.h"
#include "widgets/viewer/imageviewer.h"
#include "widgets/viewer/videoviewer.h"
#include "widgets/menuwidget.h"
#include "widgets/recordwidget.h"
#include "widgets/viewer/progressviewer.h"
#include "widgets/pushstreamwidget.h"
#include "widgets/usersetwidget.h"
#include "widgets/confirmdialog.h"
#include "widgets/suredialog.h"

#include "listeners/key_listener.h"
#include "listeners/diskcapacitylistener.h"
#include "listeners/hotpluglistener.h"

#include "utils/serialportutils.h"
#include <QMutex>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class SignalHandler : public QObject
{
    Q_OBJECT
public:
    SignalHandler(QObject *parent = nullptr);
    ~SignalHandler();
    static void intSignalHandler(int unused);

public slots:
    void handleSignalInt();

private:
    static int sigintFd[2];
    sp<QSocketNotifier> mSnInt;
    QObject *mParent;
};

class MainWidget : public QWidget,public Observer
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

    void quit();

    virtual void update(NotifyData data) {if(data.isAdd)onHotplugEvent(data.isNoLegal);}
signals:
    void onHotplugEvent(bool isNoLagel);
private:
    Ui::MainWidget *ui;

    VideoInputDevice        mInputDevice;
    AudioRender             mAudioRender;
    KeyListener            *mKeyListener;
    DiskCapacityListener   *mDiskListener;
    ConfirmDialog          *mConfirmDialog = nullptr;
    SureDialog             *mSureDialog = nullptr;

    sp<HotplugListener>     mHotplugListener;
    sp<StorageUtils>        mStorageUtils;

    sp<ImageViewer>         mImageViewer;
    sp<VideoViewer>         mVideoViewer;
    sp<QWidget>             mVideoWidget_Container;
    sp<MenuWidget>          mMenuWidget;
    sp<VideoWidget>         mVideoWidget;
    sp<RecordWidget>        mRecordWidget;
    sp<SignalHandler>       mSignalHandler;
    sp<PushStreamWidget>    mPushWidget;
    sp<UserSetWidget>       mUserSetWidget;
    sp<MediaRecorder>       mMediaRecorder;
    const uint32_t    mMenuAutoHideMs = 5000;

    bool signalIn = false;
    int space = 0;
    QMutex mux;
public slots:
    void onCreateTask();
    void onMenuEventHandler(MenuWidget::EventType type);
    void onKeyEventHandler(KeyListener::EventType type);
    void signalChange(bool has);
    void sendDiskSpace(long free,long total);

    void checkUsb(bool isNoLagel);
    void onShowChange(bool isShow);
    void onCaptureFinished();

private:
    void initWidgets();
    void onCapture();
    void onRecord();
    void onPushSteam();
    void onOpenImageBrowser();
    void onOpenVideoBrowser();
    void onPower();
    void onUserSet();
};
#endif // MAINWIDGET_H
