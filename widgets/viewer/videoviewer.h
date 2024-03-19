#ifndef VIDEOVIEWER_H
#define VIDEOVIEWER_H

#include <QWidget>
#include "baseviewer.h"
#include "videoframereader.h"
#include "videoplayer.h"
#include "listeners/key_listener.h"

namespace Ui {
class VideoViewer;
}

class VideoViewer : public BaseViewer
{
    Q_OBJECT

public:
    explicit VideoViewer(QWidget *parent = nullptr);
    ~VideoViewer();
    virtual void open();

public slots:
    virtual void onHasOpened();
    virtual void onHasClosed();
    void onItemClicked(QListWidgetItem *item);
    void onSelectModeToggled(bool toggled);
    void onLoadThumbnail(QImage image,QString file_path);
    void onCopySelectedClicked();
    void onCopyAllClicked();
    void onDelSelectClicked();
    void onDelAllClicked();
public slots:
    void onDelSelect();
    void onDelAll();
    void onDiskItemClicked(int index);
    void onKeyEventHandler(KeyListener::EventType type);
private:
    Ui::VideoViewer  *ui;
    VideoFrameReader *mVideoFrameReader;

    sp<VideoPlayer> mPlayer;

    int mIconHasUpdated;
    KeyListener *mKeyListener;
    QList<QListWidgetItem*> del_list;
    ConfirmDialog *mConfirmDialog = nullptr;
    bool isDeleteAll;
};

#endif // VIDEOVIEWER_H
