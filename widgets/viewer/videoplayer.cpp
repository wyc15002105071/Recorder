#include "videoplayer.h"
#include "ui_videoplayer.h"
#include "log.h"
#include "media/mediautils.h"

#define MODULE_TAG "VideoPlayer"

using namespace std;

VideoPlayer::VideoPlayer(QWidget *parent)
   : QWidget(parent)
   , ui(new Ui::VideoPlayer)
   , mPlayer(shared_ptr<QMediaPlayer>(new QMediaPlayer))
   , mKeyListener(KeyListener::get_instance())
{
    ui->setupUi(this);

    mVideoWidget = shared_ptr<QVideoWidget>(new QVideoWidget(ui->video_widget));
    mVideoWidget->move(0,0);
    auto layout = ui->video_widget->layout();
    layout->addWidget(mVideoWidget.get());
    connect(mPlayer.get(),SIGNAL(positionChanged(qint64)),this,SLOT(updateSlider(qint64)),Qt::UniqueConnection);
    connect(mPlayer.get(),SIGNAL(durationChanged(qint64)),this,SLOT(onDurationChanged(qint64)));
    connect(mPlayer.get(),SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(onStateChanged(QMediaPlayer::State)));
    connect(mKeyListener,SIGNAL(onPressed(KeyListener::EventType)),this,SLOT(onKeyEventHandler(KeyListener::EventType)));
    setWindowState(Qt::WindowFullScreen);
    close();
}

VideoPlayer::~VideoPlayer()
{
    RLOGD("destructor enter");
    if(mPlayer.get()) {
        mPlayer->stop();
    }
    mKeyListener = nullptr;
    delete ui;
    RLOGD("destructor enter");
}

void VideoPlayer::showEvent(QShowEvent *event)
{
    if(mUrls.count() <= 0 || mCurrentIndex < 0)
        return;
    playVideo(mUrls.at(mCurrentIndex));
}

void VideoPlayer::closeEvent(QCloseEvent *event)
{
    if(mPlayer.get()) {
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
    if(!mVideoWidget.get() || mUrls.count() <= 0 || mCurrentIndex < 0)
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

void VideoPlayer::onBackClicked()
{
    if(mPlayer.get()) {
        mPlayer->stop();
    }

    this->close();
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
    if(pos < 0 || !mPlayer.get() || mPlayer->duration() <= 0)
        return;
    qint64 duration = mPlayer->duration();
    double pos_precent = (double)pos / (double)duration;
    int slider_value = pos_precent * (ui->pos_slider->maximum()-ui->pos_slider->minimum());
    ui->pos_slider->setValue(slider_value);
    ui->position->setText(QString::fromStdString(convertMillisecondsToTime(pos)));
}

void VideoPlayer::onDurationChanged(qint64 duration)
{
    ui->duration->setText(QString::fromStdString(convertMillisecondsToTime(duration)));
}

void VideoPlayer::onPlayChecked(bool checked)
{
    if(!checked) {
        if(mPlayer.get() && mPlayer->state() == QMediaPlayer::PlayingState) {
            mPlayer->pause();
        }
    } else {
        if(mPlayer.get() && (mPlayer->state() == QMediaPlayer::PausedState || mPlayer->state() == QMediaPlayer::StoppedState)) {
            mPlayer->play();
        }
    }
}

void VideoPlayer::onStateChanged(QMediaPlayer::State state)
{
    if(state == QMediaPlayer::PausedState || state == QMediaPlayer::StoppedState) {
        ui->play_btn->setChecked(false);
    }else if(state == QMediaPlayer::PlayingState){
        ui->play_btn->setChecked(true);
    }
}

void VideoPlayer::onKeyEventHandler(KeyListener::EventType type)
{
    if(!this->isVisible() || !mPlayer.get()) {
        return;
    }

    switch (type)
    {
    case KeyListener::Key_EventType_LEFT:{
        if(mPlayer->position()>20000)
        mPlayer->setPosition(mPlayer->position() - 20000);
    }
        //onLastClicked();
        break;
    case KeyListener::Key_EventType_RIGHT:
    {
        if((mPlayer->duration() -mPlayer->position())>21000)
        mPlayer->setPosition(mPlayer->position() + 20000);
    }
        //onNextClicked();
        break;
    case KeyListener::Key_EventType_OK:
        onPlayChecked(!ui->play_btn->isChecked());
        break;
    default:
        break;
    }
}
