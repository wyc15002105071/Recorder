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
        Event_Type_Capture,
        Event_Type_Record,
        Event_Type_PushStream,
        Event_Type_ImageBrowse,
        Event_Type_VideoBrowse
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
