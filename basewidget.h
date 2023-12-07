#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>
#include "media/videoinputdevice.h"

namespace Ui {
class BaseWidget;
}

class BaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BaseWidget(QWidget *parent = nullptr);
    explicit BaseWidget(QWidget *parent,VideoInputDevice *input_Device);
    ~BaseWidget();

    virtual void open();

private:
    Ui::BaseWidget *ui;

protected:
    VideoInputDevice *mVideoInputDevice;

signals:
    void onOpened();
    void onClosed();
};

#endif // BASEWIDGET_H
