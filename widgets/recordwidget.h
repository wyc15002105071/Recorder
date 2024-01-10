#ifndef RECORDWIDGET_H
#define RECORDWIDGET_H

#include <QWidget>
#include "media/videoinputdevice.h"
#include "common/common.h"
#include "basewidget.h"

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
public slots:
    void onRecordBtnToggled(bool toggled);
    void startRec();
    virtual void onHasClosed();
};

#endif // RECORDWIDGET_H
