#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>
#include "media/videoinputdevice.h"
#include <QCloseEvent>

namespace Ui {
class BaseWidget;
}

class BaseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BaseWidget(QWidget *parent = nullptr);
    explicit BaseWidget(QWidget *parent,VideoInputDevice *input_Device);
    virtual void closeEvent(QCloseEvent *event);
    virtual void showEvent(QShowEvent *event);
    ~BaseWidget();

    virtual void open();

private:
    Ui::BaseWidget *ui;

protected:
    VideoInputDevice *mVideoInputDevice;

signals:
    void onOpened();
    void onClosed();


public slots:
    virtual void onHasOpened() {}
    virtual void onHasClosed() {}
};

#endif // BASEWIDGET_H
