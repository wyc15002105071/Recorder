#ifndef DISKSELECTIONWIDGET_H
#define DISKSELECTIONWIDGET_H

#include <QWidget>
#include "widgets/basewidget.h"
#include "diskselectionitem.h"
#include <QVector>
#include <QListWidgetItem>

namespace Ui {
class DiskSelectionWidget;
}

class DiskSelectionWidget : public BaseWidget
{
    Q_OBJECT

public:
    explicit DiskSelectionWidget(QWidget *parent = nullptr);
    ~DiskSelectionWidget();

    void additem(QString text);
    void remove(int index);
    void removeAll();
public slots:
    void onRemoveItems();
    void onItemClicked(DiskSelectionItem *item);
private:
    Ui::DiskSelectionWidget *ui;
    QList<sp<DiskSelectionItem>> mItemlist;

signals:
    void itemClicked(int index);
};

#endif // DISKSELECTWIDGET_H
