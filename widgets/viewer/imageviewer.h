#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include "baseviewer.h"
#include <QListWidgetItem>
#include "imagebrowser.h"

namespace Ui {
class ImageViewer;
}

class ImageViewer : public BaseViewer
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = nullptr);
    virtual void showEvent(QShowEvent *event);
    ~ImageViewer();
    virtual void open();
private:
    Ui::ImageViewer *ui;
    sp<ImageBrowser> mImageBrowser;

public slots:
    void onItemClicked(QListWidgetItem *item);
    void onSelectModeToggled(bool toggled);
};

#endif // IMAGEVIEWER_H
