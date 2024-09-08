#ifndef POSITIONSLIDER_H
#define POSITIONSLIDER_H

#include <QWidget>
#include <QSlider>
#include <QMouseEvent>

class PositionSlider : public QSlider
{
public:
    PositionSlider(QWidget *parent = nullptr);
    void setSliderValue(int val, bool bNotify = false);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

private:
    bool mPressed;
    bool mMoving;
    uint16_t mHandleLen;
private:
    int32_t mousePostionToSliderVal(const QPoint & pos);
};

#endif // POSITIONSLIDER_H
