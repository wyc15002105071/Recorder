#ifndef PUSHSTREAMWIDGET_H
#define PUSHSTREAMWIDGET_H

#include <QWidget>
#include "media/videoinputdevice.h"
#include "common/common.h"
#include "basewidget.h"
#include "listeners/udplistener.h"

namespace Ui {
class PushStreamWidget;
}

class PushStreamWidget : public BaseWidget
{
    Q_OBJECT

public:
    explicit PushStreamWidget(QWidget *parent = nullptr);
    explicit PushStreamWidget(QWidget *parent = nullptr,VideoInputDevice *video_input_device = nullptr);
    ~PushStreamWidget();

private:
    Ui::PushStreamWidget *ui;
    VideoInputDevice *mVideoInputDevice;
    UdpListener *udp;

public slots:
    void onPushToggled(bool toggled);
    void onStopPush();
private slots:
    void on_back_clicked();
};

#endif // PUSHSTREAMWIDGET_H
