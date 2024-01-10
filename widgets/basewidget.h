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
    virtual void closeEvent(QCloseEvent *event);
    virtual void showEvent(QShowEvent *event);
    ~BaseWidget();

    virtual void open();

private:
    Ui::BaseWidget *ui;

signals:
    void onOpened();
    void onClosed();


public slots:
    virtual void onHasOpened() {}
    virtual void onHasClosed() {}
};

#endif // BASEWIDGET_H
