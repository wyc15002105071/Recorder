#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "log.h"
#include "media_utils.h"
#include "v4l2_device.h"
#include "widgets/record_timer_widget.h"
#include "widgets/viewer/imageviewer.h"
#include "widgets/viewer/videoviewer.h"
#include "widgets/setting_widget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    Ui::MainWidget *ui;

    V4L2Device v4l2_device;
//    std::shared_ptr<RecordTimerWidget>mRecordTimerWidget;
    std::shared_ptr<ImageViewer> mImageViewer;
    std::shared_ptr<VideoViewer> mVideoViewer;
    std::shared_ptr<SettingWidget> mSettingWidget;
public slots:
    void captureClicked();
    void recordChecked(bool checked);
    void recordStopClicked();
    void pushChecked(bool checked);
    void pictureFileClicked();
    void videoFileClicked();
    void settingClicked();

private:
    void initWidgets();

};
#endif // MAINWIDGET_H
