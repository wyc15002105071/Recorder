#include "confirmdialog.h"
#include "ui_confirmdialog.h"

ConfirmDialog::ConfirmDialog(QWidget *parent) :
    QDialog(parent),
    mKeyListener(KeyListener::get_instance()),
    ui(new Ui::ConfirmDialog)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground,true);
    setAttribute(Qt::WA_DeleteOnClose,true);
    connect(mKeyListener,SIGNAL(onPressed(KeyListener::EventType)),this,SLOT(onKeyEventHandler(KeyListener::EventType)));
}

ConfirmDialog::~ConfirmDialog()
{
    delete ui;

}

void ConfirmDialog::setTitle(QString title)
{
    ui->title->setText(title);
}

void ConfirmDialog::onKeyEventHandler(KeyListener::EventType type)
{
    if(!this->isVisible()) {
        return;
    }
    switch (type)
    {
    case KeyListener::Key_EventType_OK:
        ui->accept_btn->click();
        break;
    //case KeyListener::Key_EventType_DOWN:
      //  break;
    default:
        break;
    }
}



