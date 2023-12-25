#include "imagebrowser.h"
#include "ui_imagebrowser.h"
#include "common/log.h"

#define MODULE_TAG "ImageBrowser"

ImageBrowser::ImageBrowser(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ImageBrowser)
    , mCurrentIndex(0)
    , mKeyListener(KeyListener::get_instance())
{
    ui->setupUi(this);

    setWindowState(Qt::WindowFullScreen);

    connect(mKeyListener,SIGNAL(onPressed(KeyListener::EventType)),this,SLOT(onKeyEventHandler(KeyListener::EventType)));
    close();
}

ImageBrowser::~ImageBrowser()
{
    delete ui;
}

void ImageBrowser::showEvent(QShowEvent *event)
{
    if(mUrls.count() <= 0 || mCurrentIndex < 0)
        return;
    showImage(mUrls.at(mCurrentIndex));
}

void ImageBrowser::closeEvent(QCloseEvent *event)
{
    if(mUrls.count() > 0)
        mUrls.clear();
    mCurrentIndex = 0;
    ui->image_container->clear();
}

void ImageBrowser::resizeEvent(QResizeEvent *event)
{
    if(mUrls.count() <= 0 || mCurrentIndex < 0)
        return;
    showImage(mUrls.at(mCurrentIndex));
}

void ImageBrowser::open(QList<QString> &list, int index)
{
    this->mUrls = list;
    this->mCurrentIndex = index;

    if(index < 0)
        return;
    this->show();
}

void ImageBrowser::showImage(QString path)
{
    char num_str[20] = {0};
    int count = mUrls.count();
    sprintf(num_str,"%d/%d",mCurrentIndex+1,count);
    ui->label->setText(num_str);
    QPixmap pixmap(path);
    pixmap = pixmap.scaled(ui->image_container->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->image_container->setScaledContents(true);
    ui->image_container->setPixmap(pixmap);
}

void ImageBrowser::onLastClicked()
{
    int count = mUrls.count();
    if(count <= 0)
        return;
    mCurrentIndex--;
    if(mCurrentIndex < 0)
        mCurrentIndex = count - 1;

    showImage(mUrls.at(mCurrentIndex));
}

void ImageBrowser::onNextClicked()
{
    int count = mUrls.count();
    if(count <= 0)
        return;
    mCurrentIndex++;
    if(mCurrentIndex >= count)
        mCurrentIndex = 0;

    showImage(mUrls.at(mCurrentIndex));
}

void ImageBrowser::onKeyEventHandler(KeyListener::EventType type)
{
    if(!this->isVisible()) {
        return;
    }

    switch (type)
    {
    case KeyListener::Key_EventType_LEFT:
        onLastClicked();
        break;
    case KeyListener::Key_EventType_RIGHT:
        onNextClicked();
        break;
    default:
        break;
    }
}

