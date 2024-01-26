#include "recordwidget.h"
#include "ui_recordwidget.h"
#include "common/log.h"

RecordWidget::RecordWidget(QWidget *parent) :
    BaseWidget(parent),
    mSureDialog(sp<SureDialog>(new SureDialog)),
    ui(new Ui::RecordWidget)
{
    ui->setupUi(this);
}

RecordWidget::RecordWidget(QWidget *parent, VideoInputDevice *video_input_device):
    BaseWidget(parent)
   , ui(new Ui::RecordWidget)
   , mSureDialog(sp<SureDialog>(new SureDialog))
   , mVideoInputDevice(video_input_device)
{
    ui->setupUi(this);

    connect(this,SIGNAL(onOpened()),this,SLOT(startRec()));
    connect(ui->record_timer_widget,&RecordTimerWidget::timeUp,this,&RecordWidget::timeUp);
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
            mSureDialog->exec();
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

void RecordWidget::timeUp()
{
    ui->top_right->setDisabled(ui->top_right->isEnabled());
}

void RecordWidget::onHasClosed()
{
    ui->record_btn->setChecked(true);
}
