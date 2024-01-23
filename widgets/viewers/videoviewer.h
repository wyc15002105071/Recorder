#ifndef VIDEOVIEWER_H
#define VIDEOVIEWER_H

#include <QWidget>
#include "baseviewer.h"
#include "media/videoframereader.h"
#include "videoplayer.h"

namespace Ui {
class VideoViewer;
}

class VideoViewer : public BaseViewer
{
    Q_OBJECT

public:
    explicit VideoViewer(QWidget *parent = nullptr);
    ~VideoViewer();

public slots:
    virtual void onHasOpened();
    virtual void onHasClosed();
    void onItemClicked(QListWidgetItem *item);
    void onSelectModeToggled(bool toggled);
    void onUpdateIcons(QList<QImage> images);
    void onUpdateIcon(QImage image);

    void onCopySelectedClicked();
    void onCopyAllClicked();
    void onDelSelectClicked();
    void onDelAllClicked();
public slots:
    void onDiskItemClicked(int index);

private:
    Ui::VideoViewer  *ui;
    VideoFrameReader *mVideoFrameReader;

    sp<VideoPlayer> mPlayer;

    int mIconIndex;
};

#endif // VIDEOVIEWER_H
