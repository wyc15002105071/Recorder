#include "imagebrowser.h"
#include "ui_imagebrowser.h"

ImageBrowser::ImageBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageBrowser)
{
    ui->setupUi(this);
}

ImageBrowser::~ImageBrowser()
{
    delete ui;
}
