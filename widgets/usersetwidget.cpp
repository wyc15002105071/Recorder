#include "usersetwidget.h"
#include "ui_usersetwidget.h"

UserSetWidget::UserSetWidget(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::UserSetWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, false);//设置窗口背景透明

    mOsdView = sp<OsdView>(new OsdView());
    mOsdView->close();
}

UserSetWidget::~UserSetWidget()
{
    delete ui;
}

void UserSetWidget::doEditOsd()
{
    if(mOsdView) {
        mOsdView->setGeometry(this->geometry());
        mOsdView->open();
    }
}
