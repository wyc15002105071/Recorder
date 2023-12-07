#ifndef MAINWIDGET_H
#define MAINWIDGET_H

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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    virtual void resizeEvent(QResizeEvent *event);

    void destroy();
private:
    Ui::MainWidget *ui;

    VideoInputDevice    mInputDevice;
    sp<ImageViewer>     mImageViewer;
    sp<VideoViewer>     mVideoViewer;
    sp<SettingWidget>   mSettingWidget;

    sp<QWidget>      mVideoWidget_Container;
    sp<QWidget>      mMenuWidget;
    sp<VideoWidget>  mVideoWidget;
    sp<RecordWidget> mRecordWidget;

//    QWidget* mVideoWidget;
//    QWidget* mMenuWidget;

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
