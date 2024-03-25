#include "suredialog.h"
#include "ui_suredialog.h"

SureDialog::SureDialog(QWidget *parent) :
    QDialog(parent),
    mKeyListener(KeyListener::get_instance()),
    ui(new Ui::SureDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    connect(mKeyListener,SIGNAL(onPressed(KeyListener::EventType)),this,SLOT(onKeyEventHandler(KeyListener::EventType)));

}

SureDialog::~SureDialog()
{
    delete ui;
}

void SureDialog::setText(QString t)
{
    ui->title_lb->setText(t);
}

void SureDialog::onKeyEventHandler(KeyListener::EventType type)
{
    if(!this->isVisible()) {
        return;
    }
    switch (type)
    {
    case KeyListener::Key_EventType_OK:
     {
    this->accept();
    }
        break;
    default:
        break;
    }
}
