#include "videoplayer.h"
#include "ui_videoplayer.h"
#include "log.h"
#include "media/mediautils.h"
#include "utils/mediapathutils.h"
#include <QVideoProbe>
#include "utils/videoframetoimageutils.h"
#include "utils/toastutils.h"

#define MODULE_TAG "VideoPlayer"

using namespace std;

VideoPlayer::VideoPlayer(QWidget *parent)
   : BaseWidget(parent)
   , ui(new Ui::VideoPlayer)
   , mPlayer(sp<QMediaPlayer>(new QMediaPlayer))
   , mKeyListener(KeyListener::get_instance())
{
    ui->setupUi(this);

    QVideoProbe *videoProbe = new QVideoProbe;
    QObject::connect(videoProbe, &QVideoProbe::videoFrameProbed, this,&VideoPlayer::videoFrameProbed);
    videoProbe->setSource(mPlayer.get());

    mVideoWidget = sp<QVideoWidget>(new QVideoWidget(ui->video_widget));
    mVideoWidget->move(0,0);
    auto layout = ui->video_widget->layout();
    layout->addWidget(mVideoWidget.get());
    connect(mPlayer.get(),SIGNAL(positionChanged(qint64)),this,SLOT(updateSlider(qint64)),Qt::UniqueConnection);
    connect(mPlayer.get(),SIGNAL(durationChanged(qint64)),this,SLOT(onDurationChanged(qint64)));
    connect(mPlayer.get(),SIGNAL(stateChanged(QMediaPlayer::State)),this,SLOT(onStateChanged(QMediaPlayer::State)));
    connect(mKeyListener,SIGNAL(onPressed(KeyListener::EventType)),this,SLOT(onKeyEventHandler(KeyListener::EventType)));
    connect(ui->pos_slider,SIGNAL(valueChanged(int)),this,SLOT(onSetPosition(int)));
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
    RLOGD("destructor level");
}

void VideoPlayer::openPlayer(QList<QString> &list, int index)
{
    this->mUrls = list;
    this->mCurrentIndex = index;

    if(index < 0)
        return;
	
    this->open();
}

void VideoPlayer::playVideo(QString file_url)
{
    mPlayer->stop();
    if(!mVideoWidget.get() || mUrls.count() <= 0 || mCurrentIndex < 0) {
		RLOGE("failed to playe video...");
        return;
    }
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
    case KeyListener::Key_EventType_CAPTURE:
        Screenshot();
        break;
    default:
        break;
    }
}

void VideoPlayer::onSetPosition(int value)
{
    if(!mPlayer || !ui->pos_slider->isSliderDown() || mPlayer->state() != QMediaPlayer::PlayingState)
        return;

    float precent = (float)value / (float)(ui->pos_slider->maximum() - ui->pos_slider->minimum());

    //player_->pause();
    mPlayer->setPosition(precent * mPlayer->duration());
}

void VideoPlayer::videoFrameProbed(const QVideoFrame &frame)
{
    if (!frame.isValid())
        return;

    this->frame = frame;
    if(isFirst)
        isFirst = false;
}

void VideoPlayer::Screenshot()
{
    if(!isFirst)
        if(VideoFrameToImageUtils::videoFrameToImage(frame).save(MediaPathUtils::get_instance()->getImagePath()))
            ToastUtils::instance().show(ToastUtils::INFO,"截图成功!");
}

void VideoPlayer::onHasOpened()
{
    if(mUrls.count() <= 0 || mCurrentIndex < 0) {
		RLOGE("url list is empty or current index is < 0");
        return;
    }
    playVideo(mUrls.at(mCurrentIndex));
}

void VideoPlayer::onHasClosed()
{
    if(mPlayer.get()) {
        mPlayer->stop();
    }
    mUrls.clear();
    mCurrentIndex = 0;
}
