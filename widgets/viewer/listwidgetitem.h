#ifndef LISTWIDGETITEM_H
#define LISTWIDGETITEM_H

#include <QWidget>
#include "selectionicon.h"
#include <QLabel>
#include <QPainter>
#include "common/Mutex.h"

namespace Ui {
class ListWidgetItem;
}

class Label : public QLabel {
public:
    Label(QWidget *parent):
        QLabel(parent)
       ,mImage(QImage()){

    }

    ~Label() {
        mLock.lock();
        mImage.detach();
        mLock.unlock();
    }

    virtual void paintEvent(QPaintEvent * paintEvent) {
        mLock.lock();
        QPainter painter;
        painter.begin(this);
        painter.drawImage(QRect(this->rect()),mImage);
        painter.end();
        mImage.detach();
        mLock.unlock();
    }

    void setIcon(QImage image) {
        mLock.lock();
        mImage = image;
        mLock.unlock();
        update();
    }

private:
    QImage mImage;
    RMutex mLock;
};

class ListWidgetItem : public QWidget
{
    Q_OBJECT

public:
    explicit ListWidgetItem(QWidget *parent = nullptr);
    virtual void resizeEvent(QResizeEvent *event);
    ~ListWidgetItem();
    void setFileName(QString file_name);
    void setDirPath(QString dir_path) { mDirPath = dir_path;}
	void setAbsolutePath(QString absolute_path) { mAbsolutePath = absolute_path;}
    void setIcon(QImage image);
    void setAlignment(Qt::Alignment alignment);
    void setMovie(QMovie *movie);
    void setSelectable(bool selectable);
    void setSelected(bool selected);
    bool isSelected();
	
	
    QString fileName();
    QString dirPath() { return mDirPath; }
	QString absolutePath() { return mAbsolutePath; }
private:
    Ui::ListWidgetItem *ui;

    sp<SelectionIcon> mSelectionIcon;
    bool mSelectable;

    QString mDirPath;

	QString mAbsolutePath;
};

#endif // LISTWIDGETITEM_H
