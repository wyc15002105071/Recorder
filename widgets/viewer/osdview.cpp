#include "osdview.h"
#include "ui_osdview.h"
#include "utils/configutils.h"

OsdView::OsdView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OsdView)
{
    ui->setupUi(this);
    ui->lineEdit->setText(ConfigUtils::ost_txt);
}

OsdView::~OsdView()
{
    delete ui;
}

void OsdView::open()
{
    this->showFullScreen();
}

void OsdView::doSure()
{
    QString s = ui->lineEdit->text();
    //if(s.simplified().isEmpty()){
    if(s.isEmpty()){
        return;
    }
    ConfigUtils::ost_txt = s;
    ConfigUtils::writeConfig();
    this->close();
}
