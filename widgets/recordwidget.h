#ifndef RECORDWIDGET_H
#define RECORDWIDGET_H

#include <QWidget>
#include "media/videoinputdevice.h"
#include "common/common.h"
#include "basewidget.h"
#include "widgets/suredialog.h"
#include "listeners/hotpluglistener.h"

namespace Ui {
class RecordWidget;
}


class RecordWidget : public BaseWidget ,public Observer
{
    Q_OBJECT

public:
    explicit RecordWidget(QWidget *parent = nullptr);
    RecordWidget(QWidget *parent = nullptr,VideoInputDevice *video_input_device = nullptr);
    ~RecordWidget();

    virtual void update(NotifyData data) {if(data.isUsbOut)onHotplugEvent();}
private:
    Ui::RecordWidget *ui;
    VideoInputDevice *mVideoInputDevice;
    sp<SureDialog>   mSureDialog;
    sp<HotplugListener> mHotplugListener;
public slots:
    void onRecordBtnToggled(bool toggled);
    void startRec();
    void timeUp();
    virtual void onHasClosed();
    virtual void onHasOpened();
    bool getSureDialogShow();
    void checkUsb();

signals:
    void onHotplugEvent();
};

#endif // RECORDWIDGET_H
