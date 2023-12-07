#include "selectionicon.h"
#include "ui_selectionicon.h"

SelectionIcon::SelectionIcon(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::SelectionIcon)
{
    ui->setupUi(this);
}

SelectionIcon::~SelectionIcon()
{
    delete ui;
}

void SelectionIcon::setSelectable(bool selectable)
{
    if(selectable) {
        this->show();
    } else {
        ui->selection_btn->setChecked(false);
        this->close();
    }
}

void SelectionIcon::setSelected(bool selected)
{
    ui->selection_btn->setChecked(selected);
}

bool SelectionIcon::isSelected()
{
    return ui->selection_btn->isChecked();
}
