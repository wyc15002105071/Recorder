#ifndef TOASTUTILS_H
#define TOASTUTILS_H

#include <QObject>
#include <QRect>
class ToastDlg;

class ToastUtils: public QObject
{
    Q_OBJECT

public:
    enum Level{INFO, WARN, ERROR};
private:
    ToastUtils();
public:
    static ToastUtils& instance();
public:
    void show(Level level, const QString& text);

private:
    void timerEvent(QTimerEvent *event) override;

private:
    ToastDlg* mDlg;
    int mTimerId{0};
    QRect mGeometry;


};

#endif // TOASTUTILS_H
