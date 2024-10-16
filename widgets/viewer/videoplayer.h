#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <memory>
#include <QVideoWidget>
#include "listeners/key_listener.h"

namespace Ui {
class VideoPlayer;
}

class VideoPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit VideoPlayer(QWidget *parent = nullptr);
    ~VideoPlayer();
    virtual void showEvent(QShowEvent *event);
    virtual void closeEvent(QCloseEvent *event);
    void open(QList<QString> &list,int index);
    void playVideo(QString file_url);
private:
    Ui::VideoPlayer *ui;

    std::shared_ptr<QMediaPlayer> mPlayer;
    std::shared_ptr<QVideoWidget> mVideoWidget;
    QList<QString> mUrls;
    int mCurrentIndex;
    KeyListener *mKeyListener;
public slots:
    void onBackClicked();
    void onLastClicked();
    void onNextClicked();
    void updateSlider(qint64 pos);
    void onDurationChanged(qint64 duration);
    void onPlayChecked(bool checked);
    void onStateChanged(QMediaPlayer::State state);
    void onKeyEventHandler(KeyListener::EventType type);
};

#endif // VIDEOPLAYER_H
