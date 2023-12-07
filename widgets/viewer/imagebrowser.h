#ifndef IMAGEBROWSER_H
#define IMAGEBROWSER_H

#include <QWidget>

namespace Ui {
class ImageBrowser;
}

class ImageBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit ImageBrowser(QWidget *parent = nullptr);
    ~ImageBrowser();

private:
    Ui::ImageBrowser *ui;
};

#endif // IMAGEBROWSER_H
