#include "pushstreamwidget.h"
#include "ui_pushstreamwidget.h"
#include <QTimer>

PushStreamWidget::PushStreamWidget(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::PushStreamWidget)
{
    ui->setupUi(this);

    connect(this,SIGNAL(onClosed()),this,SLOT(onStopPush()));
}

PushStreamWidget::PushStreamWidget(QWidget *parent,VideoInputDevice *video_input_device) :
     BaseWidget(parent)
   , ui(new Ui::PushStreamWidget)
   , mVideoInputDevice(video_input_device)
{
    ui->setupUi(this);
    connect(this,SIGNAL(onClosed()),this,SLOT(onStopPush()));
}

PushStreamWidget::~PushStreamWidget()
{
    onStopPush();
    delete ui;
}

void PushStreamWidget::onPushToggled(bool toggled)
{
    if(toggled) {
        if(mVideoInputDevice) {
            mVideoInputDevice->startPush();

            QTimer::singleShot(2000,this,[=]{
                ui->url->setText(QString::fromStdString(mVideoInputDevice->getPushUrl()));
            });
        }
    } else {
        if(mVideoInputDevice) {
            mVideoInputDevice->stopPush();
            ui->url->clear();
        }
    }
}

void PushStreamWidget::onStopPush()
{
    ui->push->setChecked(false);
}
