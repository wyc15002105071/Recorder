#ifndef LISTWIDGETITEM_H
#define LISTWIDGETITEM_H

#include <QWidget>
#include "selectionicon.h"

namespace Ui {
class ListWidgetItem;
}

class ListWidgetItem : public QWidget
{
    Q_OBJECT

public:
    explicit ListWidgetItem(QWidget *parent = nullptr);
    virtual void resizeEvent(QResizeEvent *event);
    ~ListWidgetItem();
    void setFileName(QString file_name);
    void setIcon(QImage image);
    void setIcon(QPixmap pixmap);
    void setAlignment(Qt::Alignment alignment);
    void setMovie(QMovie *movie);
    void setSelectable(bool selectable);
    void setSelected(bool selected);
    bool isSelected();
private:
    Ui::ListWidgetItem *ui;

    sp<SelectionIcon> mSelectionIcon;
    bool mSelectable;
};

#endif // LISTWIDGETITEM_H
