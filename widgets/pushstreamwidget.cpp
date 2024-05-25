#include "pushstreamwidget.h"
#include "ui_pushstreamwidget.h"
#include <QTimer>
#include <QDebug>
#include "utils/toastutils.h"

PushStreamWidget::PushStreamWidget(QWidget *parent) :
    BaseWidget(parent),
    udp(new UdpListener()),
    ui(new Ui::PushStreamWidget)
{
    ui->setupUi(this);
    udp->startTask();
    //connect(this,SIGNAL(onClosed()),this,SLOT(onStopPush()));
}

PushStreamWidget::PushStreamWidget(QWidget *parent,VideoInputDevice *video_input_device) :
     BaseWidget(parent),
   udp(new UdpListener()),
    ui(new Ui::PushStreamWidget)
   , mVideoInputDevice(video_input_device)
{
    ui->setupUi(this);
    udp->startTask();
    //connect(this,SIGNAL(onClosed()),this,SLOT(onStopPush()));
    connect(this,SIGNAL(doStopPush()),this,SLOT(onStopPush()));
}

PushStreamWidget::~PushStreamWidget()
{
    onStopPush();
    if(udp)udp->stopTask();
    delete ui;
}

void PushStreamWidget::onPushToggled(bool toggled)
{
    if(toggled) {
        if(mVideoInputDevice) {
            mVideoInputDevice->startPush();

            QTimer::singleShot(2000,this,[=]{
                if(udp)udp->setPlay(true);
                //qDebug()<<QString::fromStdString(mVideoInputDevice->getPushUrl());
                std::string url = mVideoInputDevice->getPushUrl();
                if(url.find("rtsp://192.")!=0){
                    doStopPush();
                    ToastUtils::instance().show(ToastUtils::INFO,"局域网IP获取异常!");
                }else{
                    ui->url->setText(QString::fromStdString(url));
                }
            });
        }
    } else {
        if(mVideoInputDevice) {
            if(udp)udp->setPlay(false);
            mVideoInputDevice->stopPush();
            ui->url->clear();
        }
    }
}

void PushStreamWidget::onStopPush()
{
    ui->push->setChecked(false);
}

void PushStreamWidget::on_back_clicked()
{
    this->close();
}
