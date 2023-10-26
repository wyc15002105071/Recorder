#include "videoplayer.h"
#include "ui_videoplayer.h"
#include "log.h"

#define MODULE_TAG "VideoPlayer"

using namespace std;

VideoPlayer::VideoPlayer(QWidget *parent) :
    QWidget(parent)
    ,ui(new Ui::VideoPlayer)
    ,mPlayer(shared_ptr<QMediaPlayer>(new QMediaPlayer))
{
    ui->setupUi(this);

    mVideoWidget = shared_ptr<QVideoWidget>(new QVideoWidget(ui->video_widget));
    mVideoWidget->move(0,0);
    auto layout = ui->video_widget->layout();
    layout->addWidget(mVideoWidget.get());
    connect(ui->back_btn,SIGNAL(clicked()),this,SLOT(closeViewAndPlayer()));
    connect(mPlayer.get(),SIGNAL(positionChanged(qint64)),this,SLOT(updateSlider(qint64)));
}

VideoPlayer::~VideoPlayer()
{
    delete ui;
}

void VideoPlayer::playVideo(QString file_url)
{
    mPlayer->stop();
    if(!mVideoWidget)
        return;
    mPlayer->setVideoOutput(mVideoWidget.get());

    RLOGD("play video %s start",file_url.toLatin1().data());
    mPlayer->setMedia(QUrl(file_url));
    mPlayer->play();
    this->show();
    showFullScreen();
    ui->pos_slider->setValue(0);
}

void VideoPlayer::closeViewAndPlayer()
{
    if(mPlayer) {
        mPlayer->stop();
    }
    close();
}

void VideoPlayer::updateSlider(qint64 pos)
{
    if(pos < 0 || !mPlayer || mPlayer->duration() <= 0)
        return;
    qint64 duration = mPlayer->duration();
    double pos_precent = (double)pos / (double)duration;
    int slider_value = pos_precent * (ui->pos_slider->maximum()-ui->pos_slider->minimum());
    ui->pos_slider->setValue(slider_value);
}
