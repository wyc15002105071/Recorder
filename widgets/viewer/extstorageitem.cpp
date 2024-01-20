#include "extstorageitem.h"
#include "ui_extstorageitem.h"

ProgressBar::ProgressBar(QWidget *parent) : QProgressBar(parent)
  , mIcon(sp<QLabel>(new QLabel(this)))
  , mText(sp<QLabel>(new QLabel(this)))
{

}

void ProgressBar::resizeEvent(QResizeEvent *event)
{
    mIcon->setStyleSheet("image: url(:/resources/icons/udisk.png);");
    mIcon->setGeometry(10,0,50,this->height());
    mText->setGeometry(mIcon->width() + 20,0,50,this->height());
}

void ProgressBar::setText(QString text)
{
    if(mText) {
        mText->setText(text);
    }
}

ExtStorageItem::ExtStorageItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExtStorageItem)
{
    ui->setupUi(this);
}

ExtStorageItem::~ExtStorageItem()
{
    delete ui;
}

void ExtStorageItem::setText(QString text)
{
    ui->capacity->setText(text);
}

void ExtStorageItem::setValue(int value)
{
    const char *progress_high = "QProgressBar::chunk"
                                "{"
                                " background-color:rgba(255, 87, 51, 1);"
                                "   border-radius: 4px;"
                                "}";
    value = value < 0?0:value;
    value = value > 100?100:value;

    if(value > 80) {
        ui->capacity->setStyleSheet(progress_high);
    } else {
        ui->capacity->setStyleSheet("");
    }

    ui->capacity->setValue(value);
}
