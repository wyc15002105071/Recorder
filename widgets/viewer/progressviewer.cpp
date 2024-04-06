#include "progressviewer.h"
#include "ui_progressviewer.h"
#include "common/log.h"
#include <QTimer>

ProgressViewer::ProgressViewer(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::ProgressViewer)
{
    ui->setupUi(this);
    ui->progress->setValue(0);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground,true);

    connect(this,SIGNAL(onUpdateValue(int)),this,SLOT(setValue(int)));
    connect(this,SIGNAL(onUpdateText(QString)),this,SLOT(setText(QString)));
    connect(this,SIGNAL(onFinish()),this,SLOT(finish()));
    close();
}

ProgressViewer::~ProgressViewer()
{
    delete ui;
}

void ProgressViewer::setText(QString text)
{
    ui->label->setText(text);
}

void ProgressViewer::setValue(int value)
{
    if(!isVisible())
        this->open();
    ui->progress->setValue(value);
}

void ProgressViewer::setOperation(int operation)
{
     mOperation = operation;
     if(mOperation == 0) {
         setText(COPYING);
     } else if(mOperation==1){
         setText(DELETEING);
     }else{
         setText(LOAD);
     }
}

void ProgressViewer::update(NotifyData data)
{
    float value = *((float *)data.data);
    onUpdateValue(value);
    if(data.result == RET_FAILURE) {
        onUpdateText(mOperation?DELETE_FAILED:COPY_FAILED);
        onFinish();
    }
    else if(data.finish && data.result == RET_SUCCESS) {
        onUpdateText(mOperation?DELETE_SUCCESS:COPY_SUCCESS);
        onFinish();
    } else if(data.finish) {
        onFinish();
    }
}

void ProgressViewer::showWarning(QString info)
{
    setText(info);
    ui->progress->setValue(ui->progress->value());
    open();
    onFinish();
}

void ProgressViewer::finish()
{
     QTimer::singleShot(1000, this, SLOT(close()));
//    this->close();
}
