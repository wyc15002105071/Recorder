#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>
#include "widgets/basewidget.h"
#include <QTimer>

namespace Ui {
class MenuWidget;
}

class MenuWidget : public BaseWidget
{
    Q_OBJECT

public:
    explicit MenuWidget(QWidget *parent = nullptr);
    ~MenuWidget();
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void open();

    typedef enum EventType {
        Menu_EventType_Capture,
        Menu_EventType_Record,
        Menu_EventType_PushStream,
        Menu_EventType_ImageBrowse,
        Menu_EventType_VideoBrowse
    } EventType_t;
signals:
    void onMenuEvent(MenuWidget::EventType type);
private:
    Ui::MenuWidget *ui;

    sp<QTimer> mTimer;
    const int32_t mAutoHideMs = 5000;
public slots:
    void onCaptureClicked();
    void onRecordClicked();
    void onPushClicked();
    void onPictureFileClicked();
    void onVideoFileClicked();

    virtual void onHasOpened();
};

#endif // MENUWIDGET_H
