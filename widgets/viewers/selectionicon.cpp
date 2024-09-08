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

void SelectionIcon::setSelected(bool selected)
{
    ui->selection_btn->setChecked(selected);
}

bool SelectionIcon::isSelected()
{
    return ui->selection_btn->isChecked();
}
