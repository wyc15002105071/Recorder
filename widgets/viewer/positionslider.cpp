#include "positionslider.h"

PositionSlider::PositionSlider(QWidget *parent)
    : QSlider(parent)
    , mPressed(false)
    , mMoving(false)
    , mHandleLen(0)
{

}

void PositionSlider::setSliderValue(int val, bool bNotify)
{
    setSliderPosition(val);
    if (bNotify)
        emit sliderMoved(val);
}

void PositionSlider::mousePressEvent(QMouseEvent *event)
{
    setSliderDown(true);
    int32_t pos = mousePostionToSliderVal(event->pos());
    if (pos != sliderPosition()) {
        setValue(pos);
        emit valueChanged(pos);
    }
}

void PositionSlider::mouseReleaseEvent(QMouseEvent *event)
{
    setSliderDown(false);
    if (!isEnabled() || event->button() != Qt::LeftButton) {
        mMoving = false;
        return;
    }

    if (mMoving) {
        QSlider::mouseReleaseEvent(event);
        mMoving = false;
    } else {
        int32_t pos = mousePostionToSliderVal(event->pos());
        if (pos != sliderPosition()) {
            setSliderPosition(pos);
            emit sliderMoved(pos);
        }
    }
}

void PositionSlider::mouseMoveEvent(QMouseEvent *event)
{
    mMoving = true;
    int32_t pos = mousePostionToSliderVal(event->pos());
    if (pos != sliderPosition()) {
        setSliderPosition(pos);
        emit sliderMoved(pos);
    }
}

int32_t PositionSlider::mousePostionToSliderVal(const QPoint &pos)
{
    int32_t duration = maximum() - minimum();
    int32_t sliderPos = 0;
    qreal mousePos = 0;
    uint16_t hanleLen = 0;
    if (Qt::Vertical == orientation()) {
        //设置了handle高度  则以m_handleLen为准， 如无设置 则以宽度一半为handleLen
        hanleLen = mHandleLen ? mHandleLen : width() / 2.0;
        mousePos = (qreal)(pos.y() - hanleLen / 2);
        if (0 > mousePos)
            mousePos = 0;
        else if (mousePos > height() - hanleLen)
            mousePos = (qreal)(height() - hanleLen);
        sliderPos = maximum() - duration * (mousePos / (height() - hanleLen));
    } else {
        //设置了handle宽度  则以m_handleLen为准， 如无设置 则以高度度一半为handleLen
        hanleLen = mHandleLen ? mHandleLen : height() / 2.0;
        mousePos = (qreal)(pos.x() - hanleLen / 2);
        if (0 > mousePos)
            mousePos = 0;
        else if (mousePos > width() - hanleLen)
            mousePos = (qreal)(width() - hanleLen);
        sliderPos = minimum() + duration * ((double)mousePos / (double)(width() - hanleLen));
    }
    return sliderPos;
}
