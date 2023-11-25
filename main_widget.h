#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QResizeEvent>
#include "log.h"
#include "media_utils.h"
#include "v4l2_device.h"
#include "widgets/record_timer_widget.h"
#include "widgets/viewer/imageviewer.h"
#include "widgets/viewer/videoviewer.h"
#include "widgets/setting_widget.h"
#include "common/common.h"

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
private:
    Ui::MainWidget *ui;

    V4L2Device v4l2_device;
//    std::shared_ptr<RecordTimerWidget>mRecordTimerWidget;
    sp<ImageViewer> mImageViewer;
    sp<VideoViewer> mVideoViewer;
    sp<SettingWidget> mSettingWidget;

    sp<QWidget> mVideoWidget;
    sp<QWidget> mMenuWidget;
    sp<VideoWidget>mRenderWidget;

//    QWidget* mVideoWidget;
//    QWidget* mMenuWidget;

public slots:
    void captureClicked();
    void recordChecked(bool checked);
    void recordStopClicked();
    void pictureFileClicked();
    void videoFileClicked();
    void settingClicked();

private:
    void initWidgets(QResizeEvent *event);

};
#endif // MAINWIDGET_H
