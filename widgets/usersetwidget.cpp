#include "usersetwidget.h"
#include "ui_usersetwidget.h"
#include "utils/configutils.h"

UserSetWidget::UserSetWidget(QWidget *parent) :
    BaseWidget(parent),
    mOsdView(sp<OsdView>(new OsdView())),
    mSetFpsView(sp<SelectView>(new SelectView())),
    mSetDiZhiView(sp<SelectView>(new SelectView())),
    mTimeSelectView(sp<SelectView>(new SelectView())),
    mStorageUtils(StorageUtils::get_instance()),
    ui(new Ui::UserSetWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, false);//设置窗口背景透明
    ui->lb_value_vision->setText(tr("V1.0.8"));

    connect(mSetFpsView.get(),&SelectView::checkedIndex,this,&UserSetWidget::setFps);
    connect(mTimeSelectView.get(),&SelectView::checkedIndex,this,&UserSetWidget::setTime);
    connect(mSetDiZhiView.get(),&SelectView::checkedIndex,this,&UserSetWidget::setDiZhi);

    fps = ConfigUtils::Fps;
    setFps(fps);
    mSetFpsView->initList(fpsList,fps);

    time = ConfigUtils::time;
    setTime(time);
    mTimeSelectView->initList(timeList,time);

    ui->pBtn_left->setDisabled(true);
    ui->widget_2->hide();
}

UserSetWidget::~UserSetWidget()
{
    delete ui;
}

void UserSetWidget::onHasOpened()
{
    if(!ConfigUtils::isUsbMedia){
        ui->pBtn_dizhi->setText("本地");
    }else{
        ui->pBtn_dizhi->setText(ConfigUtils::usbLabel);
    }
}

void UserSetWidget::onHasClosed()
{
    if(mOsdView)mOsdView->close();
    if(mSetFpsView)mSetFpsView->close();
    if(mSetDiZhiView)mSetDiZhiView->close();
    if(mTimeSelectView)mTimeSelectView->close();
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
    if(mSetFpsView) {
        mSetFpsView->setGeometry(this->geometry());
        mSetFpsView->showFullScreen();
    }
}

void UserSetWidget::setFps(int index)
{
    if(fps<0||fps>=fpsList.count())
        fps = 0;
    ui->pBtn_fps->setText(fpsList.at(index));
    ConfigUtils::Fps = fps;
    ConfigUtils::writeConfig();
}

void UserSetWidget::on_pBtn_time_clicked()
{
    if(mTimeSelectView) {
        mTimeSelectView->setGeometry(this->geometry());
        mTimeSelectView->showFullScreen();
    }
}
void UserSetWidget::setTime(int index)
{
    if(time<0||time>=timeList.count())
        time = 0;
    ui->pBtn_time->setText(timeList.at(index));

    ConfigUtils::time = index;
    ConfigUtils::writeConfig();
}


void UserSetWidget::setPage(int page)
{
    if(page==this->page){
        return;
    }
    switch (this->page) {
    case 1:ui->widget_1->hide();break;
    case 2:ui->widget_1->hide();break;
    }
    switch (page) {
    case 1:ui->widget_1->show(); break;
    case 2:ui->widget_2->show();break;
    }
    this->page = page;
    ui->pBtn_left->setDisabled(page==1);
    ui->pBtn_right->setDisabled(page==2);
}

void UserSetWidget::on_pBtn_left_clicked()
{
    setPage(this->page-1);
}

void UserSetWidget::on_pBtn_right_clicked()
{

    setPage(this->page+1);
}


void UserSetWidget::on_pBtn_dizhi_clicked()
{
    mExternalStorageInfo.clear();
    mExternalStorageInfo = mStorageUtils->getExternalStorageInfoList();
    QList<QString> dizhiList;
    dizhiList.append("本地");
    int index = 0;
    for(int i = 0;i < mExternalStorageInfo.count();i++) {
        dizhiList.append(QString::fromStdString(mExternalStorageInfo[i].label));
        if(ConfigUtils::isUsbMedia){
            if(QString::fromStdString(mExternalStorageInfo[i].node_path)
                    ==ConfigUtils::usbName&&
                    QString::fromStdString(mExternalStorageInfo[i].mount_path)
                    ==ConfigUtils::usbMonut)
                index = i+1;
        }
    }

    if(ConfigUtils::isUsbMedia&&index==0){
        ConfigUtils::isUsbMedia = false;
    }
    if(mSetDiZhiView) {
        mSetDiZhiView->setGeometry(this->geometry());
        mSetDiZhiView->initList(dizhiList,index);
        mSetDiZhiView->showFullScreen();
    }

}

void UserSetWidget::setDiZhi(int index)
{
    if(index==0){
        ui->pBtn_dizhi->setText("本地");
        ConfigUtils::isUsbMedia = false;
    }else{
        ui->pBtn_dizhi->setText(QString::fromStdString(mExternalStorageInfo[index-1].label));
        ConfigUtils::isUsbMedia = true;
        ConfigUtils::usbLabel = QString::fromStdString(mExternalStorageInfo[index-1].label);
        ConfigUtils::usbName = QString::fromStdString(mExternalStorageInfo[index-1].node_path);
        ConfigUtils::usbMonut = QString::fromStdString(mExternalStorageInfo[index-1].mount_path);
    }
}
