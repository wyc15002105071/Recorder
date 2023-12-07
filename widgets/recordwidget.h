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
    RecordWidget(QWidget *parent,VideoInputDevice *video_input_device);
    ~RecordWidget();

private:
    Ui::RecordWidget *ui;

public slots:
    void onRecordBtnToggled(bool toggled);

    void startRec();
};

#endif // RECORDWIDGET_H
