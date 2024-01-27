#include "confirmdialog.h"
#include "ui_confirmdialog.h"

ConfirmDialog::ConfirmDialog(QWidget *parent) :
    QDialog(parent)
    , ui(new Ui::ConfirmDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

}

ConfirmDialog::~ConfirmDialog()
{
    delete ui;
}

void ConfirmDialog::setTitle(QString title)
{
    ui->title->setText(title);
}


