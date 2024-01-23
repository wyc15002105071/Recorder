#ifndef EXTSTORAGEITEM_H
#define EXTSTORAGEITEM_H

#include <QWidget>
#include <QProgressBar>
#include "common/common.h"
#include <QLabel>

namespace Ui {
class ExtStorageItem;
}

class ProgressBar : public QProgressBar {
    Q_OBJECT
public:
    explicit ProgressBar(QWidget *parent = nullptr);
    virtual void resizeEvent(QResizeEvent *event);
    void setText(QString text);
private:
    sp<QLabel> mIcon;
    sp<QLabel> mText;
};


class ExtStorageItem : public QWidget
{
    Q_OBJECT

public:
    explicit ExtStorageItem(QWidget *parent = nullptr);
    ~ExtStorageItem();
    void setText(QString text);
    void setValue(int value);
private:
    Ui::ExtStorageItem *ui;
};

#endif // EXTSTORAGEITEM_H
