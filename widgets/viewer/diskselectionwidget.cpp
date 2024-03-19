#include "diskselectionwidget.h"
#include "ui_diskselectionwidget.h"

using namespace std;

DiskSelectionWidget::DiskSelectionWidget(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::DiskSelectionWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground,true);
    connect(this,SIGNAL(onClosed()),this,SLOT(onRemoveItems()));
}

DiskSelectionWidget::~DiskSelectionWidget()
{
    removeAll();
    delete ui;
}

void DiskSelectionWidget::additem(QString text)
{
    sp<DiskSelectionItem> item = sp<DiskSelectionItem>(new DiskSelectionItem);
    item->setText(text);
    ui->items->layout()->addWidget(item.get());
    mItemlist.push_back(item);

    connect(item.get(),SIGNAL(itemClicked(DiskSelectionItem *)),this,SLOT(onItemClicked(DiskSelectionItem *)),Qt::UniqueConnection);
}

void DiskSelectionWidget::remove(int index)
{
    if(mItemlist.count() == 0 || index > mItemlist.count() - 1 || index < 0)
        return;
    sp<DiskSelectionItem> item = mItemlist.at(index);
    disconnect(item.get(),SIGNAL(itemClicked(DiskSelectionItem *)),this,SLOT(onItemClicked(DiskSelectionItem *)));
    ui->items->layout()->layout()->removeWidget(item.get());
    mItemlist.removeAt(index);
    item->deleteLater();
    ui->items->setMinimumHeight(0);
    ui->items->resize(ui->items->width(),0);
    this->setMinimumHeight(0);
    this->resize(this->width(),0);
}

void DiskSelectionWidget::removeAll()
{
    int count = mItemlist.count();
    if(count <= 0)
        return;
    for(int i = 0;i < count;i++) {
        remove(i);
    }
}

void DiskSelectionWidget::onRemoveItems()
{
    removeAll();
}

void DiskSelectionWidget::onItemClicked(DiskSelectionItem *item)
{
    int count = mItemlist.count();
    if(!item || count <= 0)
        return;

    for(int i = 0;i < count;i++) {
        if(mItemlist.at(i).get() == item) {
            emit itemClicked(i);
            break;
        }
    }
}

void DiskSelectionWidget::onHasOpened()
{
}

