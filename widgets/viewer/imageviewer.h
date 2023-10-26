#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include "abstractviewer.h"

namespace Ui {
class ImageViewer;
}

class ImageViewer : public AbStractViewer
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = nullptr);
    ~ImageViewer();
    virtual void open();
private:
    Ui::ImageViewer *ui;



};

#endif // IMAGEVIEWER_H
