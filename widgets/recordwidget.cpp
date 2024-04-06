#include "recordwidget.h"
#include "ui_recordwidget.h"
#include "common/log.h"
#include "utils/configutils.h"

RecordWidget::RecordWidget(QWidget *parent) :
    BaseWidget(parent),
    mSureDialog(sp<SureDialog>(new SureDialog)),
    //mHotplugListener(sp<HotplugListener>(new HotplugListener)),
    ui(new Ui::RecordWidget)
{
    ui->setupUi(this);
}

RecordWidget::RecordWidget(QWidget *parent, VideoInputDevice *video_input_device):
    BaseWidget(parent)
   , ui(new Ui::RecordWidget)
   , mSureDialog(sp<SureDialog>(new SureDialog))
   //, mHotplugListener(sp<HotplugListener>(new HotplugListener))
   , mVideoInputDevice(video_input_device)
{
    ui->setupUi(this);

    connect(this,SIGNAL(onOpened()),this,SLOT(startRec()));
    connect(ui->record_timer_widget,&RecordTimerWidget::timeUp,this,&RecordWidget::timeUp);

    //mHotplugListener->attach(this);
    connect(this,SIGNAL(onHotplugEvent()),this,SLOT(checkUsb()));
}

RecordWidget::~RecordWidget()
{
    //if(mHotplugListener) {
    //    mHotplugListener->stopTask();
    //}
    delete ui;
}

void RecordWidget::onRecordBtnToggled(bool toggled)
{
    if(toggled) {
        if(getSureDialogShow())
            return;
        ui->record_timer_widget->stop();
        if(mVideoInputDevice) {
            mVideoInputDevice->stopRecord();
            if(ConfigUtils::isUsbMedia)
                system("sync");
            if(!ConfigUtils::isPowOff){
                //mSureDialog->exec();
                mSureDialog->setText("视屏录制完成");
                mSureDialog->showFullScreen();
            }
        }
        this->close();
    }
}

void RecordWidget::startRec()
{
    ui->record_btn->setChecked(false);

    if(mVideoInputDevice) {
        MediaRecorder::VideoProfile profile = MediaRecorder::VideoProfile_Standard;
        switch (ConfigUtils::Fps) {
        case 0:profile = MediaRecorder::VideoProfile_Low;
            break;
        case 1:profile = MediaRecorder::VideoProfile_Standard;
            break;
        case 2:profile = MediaRecorder::VideoProfile_High;
            break;
        }
        mVideoInputDevice->startRecord(profile);
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
    //if(mHotplugListener) {
    //    mHotplugListener->stopTask();
    //}
}

//void RecordWidget::onHasOpened()
//{
//    if(mHotplugListener) {
//        mHotplugListener->startTask();
//    }
//}

bool RecordWidget::getSureDialogShow()
{
    return mSureDialog->isVisible();
}

void RecordWidget::checkUsb()
{
    if(!ConfigUtils::isUsbMedia){
        ui->record_timer_widget->stop();
        if(mVideoInputDevice) {
            mVideoInputDevice->stopRecord();
            system("sync");
            mSureDialog->showFullScreen();
            mSureDialog->setText("录制错误：储存设备移除");
        }
        this->close();
    }
}
