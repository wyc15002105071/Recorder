#ifndef RECORD_TIMER_WIDGET_H
#define RECORD_TIMER_WIDGET_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class RecordTimerWidget;
}

class RecordTimerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RecordTimerWidget(QWidget *parent = nullptr);
    ~RecordTimerWidget();
    void start();
    void stop();

signals:
    void timeUp();
public slots:
    void updateView();
private:
    Ui::RecordTimerWidget *ui;
    std::shared_ptr<QTimer>mTimer;

    int mSec;
    int mMin;
    int mHour;
};

#endif // RECORD_TIMER_WIDGET_H
