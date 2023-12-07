#include "basewidget.h"
#include "ui_basewidget.h"

BaseWidget::BaseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseWidget)
{
    ui->setupUi(this);
}

BaseWidget::BaseWidget(QWidget *parent, VideoInputDevice *input_Device):
    QWidget(parent),
    ui(new Ui::BaseWidget),
    mVideoInputDevice(input_Device)
{
    ui->setupUi(this);
}

BaseWidget::~BaseWidget()
{
    delete ui;
}

void BaseWidget::open()
{
    this->show();

    emit onOpened();
}
