#ifndef IMAGEBROWSER_H
#define IMAGEBROWSER_H

#include <QWidget>
#include <QList>
#include "listeners/key_listener.h"
#include "widgets/basewidget.h"

namespace Ui {
class ImageBrowser;
}

class ImageBrowser : public BaseWidget
{
    Q_OBJECT

public:
    explicit ImageBrowser(QWidget *parent = nullptr);
    ~ImageBrowser();

    virtual void resizeEvent(QResizeEvent *event);
    void openPlayer(QList<QString> &list,int index);
    QString getCurrentIndex();

    virtual void onHasOpened();
    virtual void onHasClosed();
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
