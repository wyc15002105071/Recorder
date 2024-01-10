#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QObject>
#include <QSocketNotifier>
#include <signal.h>
#include <QWidget>
#include <QPushButton>
#include <QResizeEvent>
#include "media/mediautils.h"
#include "media/videoinputdevice.h"
#include "widgets/record_timer_widget.h"
#include "widgets/viewer/imageviewer.h"
#include "widgets/viewer/videoviewer.h"
#include "widgets/menuwidget.h"
#include "common/common.h"
#include "common/log.h"
#include "widgets/recordwidget.h"
#include "widgets/viewer/progressviewer.h"
#include "widgets/pushstreamwidget.h"
#include "listeners/key_listener.h"

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

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

    void quit();

private:
    Ui::MainWidget *ui;

    VideoInputDevice  mInputDevice;
    KeyListener      *mKeyListener;

    sp<ImageViewer>         mImageViewer;
    sp<VideoViewer>         mVideoViewer;
    sp<QWidget>             mVideoWidget_Container;
    sp<MenuWidget>          mMenuWidget;
    sp<VideoWidget>         mVideoWidget;
    sp<RecordWidget>        mRecordWidget;
    sp<SignalHandler>       mSignalHandler;
    sp<PushStreamWidget>    mPushWidget;

    const uint32_t    mMenuAutoHideMs = 5000;
public slots:
    void onCreateTask();
    void onMenuEventHandler(MenuWidget::EventType type);
    void onKeyEventHandler(KeyListener::EventType type);

private:
    void initWidgets();
    void onCapture();
    void onRecord();
    void onPushSteam();
    void onOpenImageBrowser();
    void onOpenVideoBrowser();
};
#endif // MAINWIDGET_H
