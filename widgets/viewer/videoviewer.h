#ifndef VIDEOVIEWER_H
#define VIDEOVIEWER_H

#include <QWidget>
#include "baseviewer.h"
#include "videoframereader.h"
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
    virtual void open();
private:
    Ui::VideoViewer *ui;

    VideoFrameReader *mVideoFrameReader;
    std::shared_ptr<VideoPlayer>mPlayer;
public slots:
    void itemDoubleClicked(QListWidgetItem*item);
};

#endif // VIDEOVIEWER_H
