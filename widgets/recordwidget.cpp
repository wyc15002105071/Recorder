#include "recordwidget.h"
#include "ui_recordwidget.h"
#include "common/log.h"

RecordWidget::RecordWidget(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::RecordWidget)
{
    ui->setupUi(this);
}

RecordWidget::RecordWidget(QWidget *parent, VideoInputDevice *video_input_device):
    BaseWidget(parent)
   , ui(new Ui::RecordWidget)
   , mVideoInputDevice(video_input_device)
{
    ui->setupUi(this);

    connect(this,SIGNAL(onOpened()),this,SLOT(startRec()));
}

RecordWidget::~RecordWidget()
{
    delete ui;
}

void RecordWidget::onRecordBtnToggled(bool toggled)
{
    if(toggled) {
        ui->record_timer_widget->stop();
        if(mVideoInputDevice) {
            mVideoInputDevice->stopRecord();
        }

        this->close();
    }
}

void RecordWidget::startRec()
{
    ui->record_btn->setChecked(false);

    if(mVideoInputDevice) {
        mVideoInputDevice->startRecord();
    }
    ui->record_timer_widget->start();
}

void RecordWidget::onHasClosed()
{
    ui->record_btn->setChecked(true);
}
