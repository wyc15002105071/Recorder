#ifndef IMAGEBROWSER_H
#define IMAGEBROWSER_H

#include <QWidget>
#include <QList>

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

private:
    void showImage(QString path);

public slots:
    void onLastClicked();
    void onNextClicked();
};

#endif // IMAGEBROWSER_H
