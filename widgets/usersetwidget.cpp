#include "usersetwidget.h"
#include "ui_usersetwidget.h"
#include "utils/configutils.h"

UserSetWidget::UserSetWidget(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::UserSetWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, false);//设置窗口背景透明
    ui->lb_value_vision->setText(tr("V1.0.3"));

    mOsdView = sp<OsdView>(new OsdView());
    mOsdView->close();
    mSetFpsView = sp<SetFpsView>(new SetFpsView());
    connect(mSetFpsView.get(),&SetFpsView::setFps,this,&UserSetWidget::setFps);
    mSetFpsView->close();

    fps = ConfigUtils::Fps;
    setFps(fps);
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

void UserSetWidget::doSelectFps()
{
    if(mOsdView) {
        mSetFpsView->setGeometry(this->geometry());
        mSetFpsView->showFullScreen();
    }
}

void UserSetWidget::setFps(int fps)
{
    switch (fps) {
    case 1:ui->pBtn_fps->setText("低"); break;
    case 2:ui->pBtn_fps->setText("中"); break;
    case 3:ui->pBtn_fps->setText("高"); break;
    }
    ConfigUtils::Fps = fps;
    ConfigUtils::writeConfig();
}
