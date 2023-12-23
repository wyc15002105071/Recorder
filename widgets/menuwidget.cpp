#include "menuwidget.h"
#include "ui_menuwidget.h"

MenuWidget::MenuWidget(QWidget *parent)
    : BaseWidget(parent)
    , ui(new Ui::MenuWidget)
    , mTimer(sp<QTimer>(new QTimer))
{
    ui->setupUi(this);
    mTimer->setSingleShot(true);
    mTimer->setInterval(mAutoHideMs);

    connect(mTimer.get(),SIGNAL(timeout()),this,SLOT(close()));
}

MenuWidget::~MenuWidget()
{
    delete ui;
}

void MenuWidget::mousePressEvent(QMouseEvent *event)
{
    if(mTimer) {
        mTimer->stop();
        mTimer->start();
    }
}

void MenuWidget::open()
{
    if(this->isVisible()) {
        if(mTimer) {
            mTimer->stop();
        }
    } else {
        this->show();
    }

    mTimer->start();
}

void MenuWidget::onCaptureClicked()
{
    onMenuEvent(Event_Type_Capture);
}

void MenuWidget::onRecordClicked()
{
    onMenuEvent(Event_Type_Record);
}

void MenuWidget::onPushClicked()
{
    onMenuEvent(Event_Type_PushStream);
}

void MenuWidget::onPictureFileClicked()
{
    onMenuEvent(Event_Type_ImageBrowse);
}

void MenuWidget::onVideoFileClicked()
{
    onMenuEvent(Event_Type_VideoBrowse);
}

void MenuWidget::onHasOpened()
{
    if(mTimer) {
        mTimer->start();
    }
}

