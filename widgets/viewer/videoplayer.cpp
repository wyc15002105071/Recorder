#include "videoplayer.h"
#include "ui_videoplayer.h"
#include "log.h"
#include "media/mediautils.h"

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
    connect(mPlayer.get(),SIGNAL(positionChanged(qint64)),this,SLOT(updateSlider(qint64)),Qt::UniqueConnection);
    connect(mPlayer.get(),SIGNAL(durationChanged(qint64)),this,SLOT(onDurationChanged(qint64)));

    setWindowState(Qt::WindowFullScreen);
    close();
}

VideoPlayer::~VideoPlayer()
{
    delete ui;
}

void VideoPlayer::showEvent(QShowEvent *event)
{
    if(mUrls.count() <= 0 || mCurrentIndex < 0)
        return;
    playVideo(mUrls.at(mCurrentIndex));
}

void VideoPlayer::closeEvent(QCloseEvent *event)
{
    if(mPlayer) {
        mPlayer->stop();
    }
    mUrls.clear();
    mCurrentIndex = 0;
}

void VideoPlayer::open(QList<QString> &list, int index)
{
    this->mUrls = list;
    this->mCurrentIndex = index;

    if(index < 0)
        return;
    this->show();
}

void VideoPlayer::playVideo(QString file_url)
{
    mPlayer->stop();
    if(!mVideoWidget || mUrls.count() <= 0 || mCurrentIndex < 0)
        return;
    mPlayer->setVideoOutput(mVideoWidget.get());

    char num_str[20] = {0};
    int count = mUrls.count();
    sprintf(num_str,"%d/%d",mCurrentIndex+1,count);
    ui->label->setText(num_str);

    RLOGD("play video %s start",file_url.toLatin1().data());

    QString url_header = "file://";

    mPlayer->setMedia(QUrl(url_header+file_url));
    mPlayer->play();

    ui->pos_slider->setValue(0);
}

void VideoPlayer::onLastClicked()
{
    int count = mUrls.count();
    if(count <= 0)
        return;
    mCurrentIndex--;
    if(mCurrentIndex < 0)
        mCurrentIndex = count - 1;
    playVideo(mUrls.at(mCurrentIndex));
}

void VideoPlayer::onNextClicked()
{
    int count = mUrls.count();
    if(count <= 0)
        return;
    mCurrentIndex++;
    if(mCurrentIndex >= count)
        mCurrentIndex = 0;
    playVideo(mUrls.at(mCurrentIndex));
}

void VideoPlayer::updateSlider(qint64 pos)
{
    if(pos < 0 || !mPlayer || mPlayer->duration() <= 0)
        return;
    qint64 duration = mPlayer->duration();
    double pos_precent = (double)pos / (double)duration;
    int slider_value = pos_precent * (ui->pos_slider->maximum()-ui->pos_slider->minimum());
    ui->pos_slider->setValue(slider_value);
    RLOGD("pos is %d",pos);
    ui->position->setText(QString::fromStdString(convertMillisecondsToTime(pos)));
}

void VideoPlayer::onDurationChanged(qint64 duration)
{
    ui->duration->setText(QString::fromStdString(convertMillisecondsToTime(duration)));
}

void VideoPlayer::onPlayChecked(bool checked)
{
    if(checked) {
        if(mPlayer) {
            mPlayer->pause();
        }
    } else {
        if(mPlayer) {
            mPlayer->play();
        }
    }
}
