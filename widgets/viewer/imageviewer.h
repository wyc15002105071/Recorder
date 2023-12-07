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
    ~ImageViewer();
    virtual void open();
private:
    Ui::ImageViewer *ui;
    sp<ImageBrowser> mImageBrowser;

public slots:
    void onItemClicked(QListWidgetItem *item);
};

#endif // IMAGEVIEWER_H
