#ifndef SELECTIONICON_H
#define SELECTIONICON_H

#include <QWidget>
#include "widgets/basewidget.h"

namespace Ui {
class SelectionIcon;
}

class SelectionIcon : public BaseWidget
{
    Q_OBJECT

public:
    explicit SelectionIcon(QWidget *parent = nullptr);
    ~SelectionIcon();

    void setSelected(bool selected);
    bool isSelected();
private:
    Ui::SelectionIcon *ui;
};

#endif // SELECTIONICON_H
