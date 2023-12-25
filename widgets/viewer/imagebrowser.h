#ifndef IMAGEBROWSER_H
#define IMAGEBROWSER_H

#include <QWidget>
#include <QList>
#include "listeners/key_listener.h"

namespace Ui {
class ImageBrowser;
}

class ImageBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit ImageBrowser(QWidget *parent = nullptr);
    ~ImageBrowser();
    virtual void showEvent(QShowEvent *event);
    virtual void closeEvent(QCloseEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    void open(QList<QString> &list,int index);
private:
    Ui::ImageBrowser *ui;

    QList<QString> mUrls;
    int mCurrentIndex;
    KeyListener *mKeyListener;
private:
    void showImage(QString path);

public slots:
    void onLastClicked();
    void onNextClicked();
    void onKeyEventHandler(KeyListener::EventType type);
};

#endif // IMAGEBROWSER_H
