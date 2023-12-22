#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QObject>
#include <QSocketNotifier>
#include <signal.h>
#include <QWidget>
#include <QPushButton>
#include <QResizeEvent>
#include "log.h"
#include "mediautils.h"
#include "videoinputdevice.h"
#include "widgets/record_timer_widget.h"
#include "widgets/viewer/imageviewer.h"
#include "widgets/viewer/videoviewer.h"
#include "widgets/setting_widget.h"
#include "common/common.h"
#include "widgets/recordwidget.h"
#include "widgets/viewer/progressviewer.h"

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
    virtual void resizeEvent(QResizeEvent *event);

    void quit();

private:
    Ui::MainWidget *ui;

    VideoInputDevice    mInputDevice;
    sp<ImageViewer>     mImageViewer;
    sp<VideoViewer>     mVideoViewer;
    sp<SettingWidget>   mSettingWidget;

    sp<QWidget>       mVideoWidget_Container;
    sp<QWidget>       mMenuWidget;
    sp<VideoWidget>   mVideoWidget;
    sp<RecordWidget>  mRecordWidget;
    sp<SignalHandler> mSignalHandler;
public slots:
    void onCaptureClicked();
    void onRecordClicked();

    void recordChecked(bool checked);
    void recordStopClicked();
    void onPictureFileClicked();
    void onVideoFileClicked();
    void settingClicked();

    void onCreateTask();
private:
    void initWidgets();

};
#endif // MAINWIDGET_H
