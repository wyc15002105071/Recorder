#include "diskselectionitem.h"
#include "ui_diskselectionitem.h"

DiskSelectionItem::DiskSelectionItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DiskSelectionItem)
{
    ui->setupUi(this);

    connect(ui->item_btn,&QPushButton::clicked,this,[=]{ emit itemClicked(this);});
}

void DiskSelectionItem::setText(QString text)
{
    ui->item_btn->setText(text);
}

void DiskSelectionItem::setText(std::string text)
{
    setText(QString::fromStdString(text));
}

DiskSelectionItem::~DiskSelectionItem()
{
    delete ui;
}

void DiskSelectionItem::setStorageInfo(StorageUtils::ExternalStorageInfo info)
{
    mInfo = std::move(info);

    setText(mInfo.label);
}
