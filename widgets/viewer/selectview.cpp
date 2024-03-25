#include "selectview.h"
#include "ui_selectview.h"

SelectView::SelectView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectView)
{
    ui->setupUi(this);
}

SelectView::~SelectView()
{
    delete ui;
}

void SelectView::initList(QList<QString> list, int index)
{
    this->list = list;
    clearLayout();
    for(int i=0;i<list.count();i++){
        QPushButton *pBtn1 = new QPushButton(list.at(i),this);
        pBtn1->setCheckable(true);
        pBtn1->setMaximumHeight(120);
        pBtn1->setChecked(i==index);
        pBtn1->setAutoExclusive(true);
        ui->widget_container->layout()->addWidget(pBtn1);
        buttonList.append(pBtn1);
    }
}

void SelectView::clearLayout()
{
    for(int i =0;i<buttonList.count();i++){
     ui->widget_container->layout()->removeWidget(buttonList.at(i));
     delete buttonList.at(i);
    }
    buttonList.clear();
}

void SelectView::on_pBtn_sure_clicked()
{
    for(int i =0;i<buttonList.count();i++){
        if(buttonList.at(i)->isChecked()){
            emit checkedIndex(i);
            this->close();
        }
    }
}
