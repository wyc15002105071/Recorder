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
    connect(this,&MenuWidget::onMenuEvent,this,[=]{
        mTimer->stop();
        mTimer->start();
    },Qt::UniqueConnection);
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

void MenuWidget::setButtonDisabled(bool isDisabled)
{
    ui->capture_btn->setDisabled(isDisabled);
    ui->push_btn->setDisabled(isDisabled);
    ui->record_btn->setDisabled(isDisabled);
}

void MenuWidget::onCaptureClicked()
{
    onMenuEvent(Menu_EventType_Capture);
}

void MenuWidget::onRecordClicked()
{
    onMenuEvent(Menu_EventType_Record);
}

void MenuWidget::onPushClicked()
{
    onMenuEvent(Menu_EventType_PushStream);
}

void MenuWidget::onPictureFileClicked()
{
    onMenuEvent(Menu_EventType_ImageBrowse);
}

void MenuWidget::onVideoFileClicked()
{
    onMenuEvent(Menu_EventType_VideoBrowse);
}

void MenuWidget::onUserSetClicked()
{
    onMenuEvent(Menu_EventType_UserSet);
}

void MenuWidget::onHasOpened()
{
    if(mTimer) {
        mTimer->start();
    }
    onShowChange(true);
}

void MenuWidget::onHasClosed()
{
    onShowChange(false);
}

