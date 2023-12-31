#ifndef DISKSELECTIONITEM_H
#define DISKSELECTIONITEM_H

#include <QWidget>
#include <string>
#include "utils/storageutils.h"

namespace Ui {
class DiskSelectionItem;
}

class DiskSelectionItem : public QWidget
{
    Q_OBJECT

public:
    explicit DiskSelectionItem(QWidget *parent = nullptr);
    ~DiskSelectionItem();
    void setStorageInfo(StorageUtils::ExternalStorageInfo info);
    void setText(QString text);
    void setText(std::string text);
private:
    Ui::DiskSelectionItem *ui;

    StorageUtils::ExternalStorageInfo mInfo;

signals:
    void itemClicked(DiskSelectionItem *item);
};

#endif // DISKSELECTIONITEM_H
