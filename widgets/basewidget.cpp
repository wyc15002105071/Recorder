#include "basewidget.h"
#include "ui_basewidget.h"

BaseWidget::BaseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseWidget)
{
    ui->setupUi(this);
    connect(this,SIGNAL(onOpened()),this,SLOT(onHasOpened()),Qt::UniqueConnection);
    connect(this,SIGNAL(onClosed()),this,SLOT(onHasClosed()),Qt::UniqueConnection);
}

BaseWidget::BaseWidget(QWidget *parent, VideoInputDevice *input_Device):
    QWidget(parent),
    ui(new Ui::BaseWidget),
    mVideoInputDevice(input_Device)
{
    ui->setupUi(this);
    connect(this,SIGNAL(onOpened()),this,SLOT(onHasOpened()),Qt::UniqueConnection);
    connect(this,SIGNAL(onClosed()),this,SLOT(onHasClosed()),Qt::UniqueConnection);
}

void BaseWidget::closeEvent(QCloseEvent *event)
{
    emit onClosed();
}

void BaseWidget::showEvent(QShowEvent *event)
{
    emit onOpened();
}

BaseWidget::~BaseWidget()
{
    delete ui;
}

void BaseWidget::open()
{
    this->show();
}
