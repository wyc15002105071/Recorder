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

public slots:
    virtual void onHasOpened();
    virtual void onHasClosed();
    void onItemClicked(QListWidgetItem *item);
    void onSelectModeToggled(bool toggled);

    void onCopySelectedClicked();
    void onCopyAllClicked();
    void onDelSelectClicked();
    void onDelAllClicked();
public slots:
    void onDiskItemClicked(int index);
private:
    Ui::ImageViewer *ui;
    sp<ImageBrowser> mImageBrowser;
    QList<QString> mSelectionlist;
};

#endif // IMAGEVIEWER_H
