#include "powoffwidget.h"
#include "ui_powoffwidget.h"

PowOffWidget::PowOffWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PowOffWidget)
{
    ui->setupUi(this);
}

PowOffWidget::~PowOffWidget()
{
    delete ui;
}
