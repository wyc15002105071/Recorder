#ifndef RECORDWIDGET_H
#define RECORDWIDGET_H

#include <QWidget>
#include "media/videoinputdevice.h"
#include "common/common.h"
#include "basewidget.h"
#include "widgets/suredialog.h"

namespace Ui {
class RecordWidget;
}


class RecordWidget : public BaseWidget
{
    Q_OBJECT

public:
    explicit RecordWidget(QWidget *parent = nullptr);
    RecordWidget(QWidget *parent = nullptr,VideoInputDevice *video_input_device = nullptr);
    ~RecordWidget();

private:
    Ui::RecordWidget *ui;
    VideoInputDevice *mVideoInputDevice;
    sp<SureDialog>   mSureDialog;
public slots:
    void onRecordBtnToggled(bool toggled);
    void startRec();
    void timeUp();
    virtual void onHasClosed();
    bool getSureDialogShow();
};

#endif // RECORDWIDGET_H
