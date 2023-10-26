#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <memory>
#include <QVideoWidget>

namespace Ui {
class VideoPlayer;
}

class VideoPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit VideoPlayer(QWidget *parent = nullptr);
    ~VideoPlayer();
    void playVideo(QString file_url);
private:
    Ui::VideoPlayer *ui;

    std::shared_ptr<QMediaPlayer> mPlayer;
    std::shared_ptr<QVideoWidget>mVideoWidget;

public slots:
    void closeViewAndPlayer();
    void updateSlider(qint64 pos);
};

#endif // VIDEOPLAYER_H
