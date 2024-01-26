#include "record_timer_widget.h"
#include "ui_recordtimerwidget.h"
#include "log.h"

using namespace std;

RecordTimerWidget::RecordTimerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecordTimerWidget),
    mTimer(shared_ptr<QTimer>(new QTimer)),
    mSec(0),
    mMin(0),
    mHour(0)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    connect(mTimer.get(),SIGNAL(timeout()),this,SLOT(updateView()));

    close();
}

RecordTimerWidget::~RecordTimerWidget()
{
    delete ui;
}

void RecordTimerWidget::stop()
{
    if(mTimer->isActive())
        mTimer->stop();
    mSec = mMin = mHour = 0;
    ui->record_time_str->setText("00:00:00");
    close();
}

void RecordTimerWidget::updateView()
{
    mSec++;
    if(mSec >= 60) {
        mMin++;
        mSec = 0;
        if(mMin >= 60) {
            mHour++;
            mMin = 0;
        }
    }

    char time_str[30] = {0};
    sprintf(time_str,"%.2d:%.2d:%.2d",mHour,mMin,mSec);
    ui->record_time_str->setText(time_str);
    emit timeUp();
}

void RecordTimerWidget::start()
{
    mSec = mMin = mHour = 0;
    mTimer->start(1000);
    show();
}
