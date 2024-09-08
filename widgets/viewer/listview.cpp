#include "listview.h"
#include "ui_listview.h"

ListView::ListView(ListType type, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListView),
    mList(nullptr),
    mListType(type)
{
    ui->setupUi(this);

    mList = ui->list;
}

ListView::~ListView()
{
    delete ui;
}

QListWidget *ListView::list_ui()
{
    return mList;
}

void ListView::onItemClicked(QListWidgetItem *item)
{
    emit itemClicked(item);
}
