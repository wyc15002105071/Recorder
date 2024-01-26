#include "suredialog.h"
#include "ui_suredialog.h"

SureDialog::SureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SureDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
}

SureDialog::~SureDialog()
{
    delete ui;
}
