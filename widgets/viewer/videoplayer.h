#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QMediaPlayer>
#include <memory>
#include <QVideoWidget>
#include <QVideoFrame>
#include "listeners/key_listener.h"
#include "widgets/basewidget.h"

namespace Ui {
class VideoPlayer;
}

class VideoPlayer : public BaseWidget
{
    Q_OBJECT

public:
    explicit VideoPlayer(QWidget *parent = nullptr);
    ~VideoPlayer();
    void openPlayer(QList<QString> &list,int index);
    void playVideo(QString file_url);
private:
    Ui::VideoPlayer *ui;

    std::shared_ptr<QMediaPlayer> mPlayer;
    std::shared_ptr<QVideoWidget> mVideoWidget;
    QList<QString> mUrls;
    int mCurrentIndex;
    KeyListener *mKeyListener;
    QVideoFrame frame;
    bool isFirst = true;
public slots:
    void onBackClicked();
    void onLastClicked();
    void onNextClicked();
    void updateSlider(qint64 pos);
    void onDurationChanged(qint64 duration);
    void onPlayChecked(bool checked);
    void onStateChanged(QMediaPlayer::State state);
    void onKeyEventHandler(KeyListener::EventType type);
    void onSetPosition(int value);

    void videoFrameProbed(const QVideoFrame &frame);
    void Screenshot();

    virtual void onHasOpened();
    virtual void onHasClosed();
};

#endif // VIDEOPLAYER_H
