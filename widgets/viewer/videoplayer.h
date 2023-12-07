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
    virtual void showEvent(QShowEvent *event);
    virtual void closeEvent(QCloseEvent *event);
    void open(QList<QString> &list,int index);
    void playVideo(QString file_url);
private:
    Ui::VideoPlayer *ui;

    std::shared_ptr<QMediaPlayer> mPlayer;
    std::shared_ptr<QVideoWidget>mVideoWidget;
    QList<QString> mUrls;
    int mCurrentIndex;
public slots:
    void onLastClicked();
    void onNextClicked();
    void updateSlider(qint64 pos);
    void onDurationChanged(qint64 duration);
    void onPlayChecked(bool checked);
};

#endif // VIDEOPLAYER_H
