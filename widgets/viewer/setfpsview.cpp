#include "setfpsview.h"
#include "ui_setfpsview.h"

SetFpsView::SetFpsView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetFpsView)
{
    ui->setupUi(this);
}

SetFpsView::~SetFpsView()
{
    delete ui;
}

void SetFpsView::on_pBtn_cancel_clicked()
{
    this->close();
}

void SetFpsView::on_pBtn_high_clicked()
{

    emit setFps(3);this->close();
}

void SetFpsView::on_pBtn_low_clicked()
{

    emit setFps(1);this->close();
}

void SetFpsView::on_pBtn_medium_clicked()
{

    emit setFps(2);this->close();
}
