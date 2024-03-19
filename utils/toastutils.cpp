#include "toastutils.h"
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QApplication>
#include <QDesktopWidget>

class ToastDlg: public QDialog
{
private:
    QLabel* mLabel;
    QLabel* mCloseBtn;
protected:
    bool eventFilter(QObject *obj, QEvent *ev) override
    {
        if (obj == mCloseBtn)
        {
            if (ev->type() == QEvent::MouseButtonRelease)
            {
                accept();
            }
        }
        return QObject::eventFilter(obj, ev);
    }
public:
    ToastDlg()
    {
        auto layout = new QHBoxLayout;//水平布局

        layout->addSpacing(20);

        //mCloseBtn = new QLabel;
        //mCloseBtn->setPixmap(QPixmap(""));
        //mCloseBtn->installEventFilter(this);
        //mCloseBtn->setStyleSheet("background:transparent;border-image:url(:/img/icon_dialog_sure.png)");
        //mCloseBtn->setFixedSize(25,25);
        //layout->addWidget(mCloseBtn);

        //layout->addSpacing(10);
        mLabel = new QLabel;
        mLabel->setAlignment(Qt::AlignCenter);
        mLabel->setStyleSheet("color: white; background:transparent;font-size:24px;");//red
        layout->addWidget(mLabel, 1);//stretch = 1

        layout->addSpacing(20);
        setLayout(layout);
        setWindowFlag(Qt::FramelessWindowHint);//生成一个无边界窗口。用户不能通过窗口系统移动或调整无边界窗口的大小。
        setAttribute(Qt::WA_ShowWithoutActivating,true);//Show the widget without making it active.
        //setAttribute(Qt::WA_TranslucentBackground, true);   // 背景透明
    }

    void show(ToastUtils::Level level, const QString& text)
    {
        QPalette p = palette();
        //QColor(int r, int g, int b, int a = 255)
        //Constructs a color with the RGB value r, g, b, and the alpha-channel (transparency) value of a.
        p.setColor(QPalette::Window, QColor(57,63,77,200));
        if (level == ToastUtils::INFO)
        {
            //mCloseBtn->setVisible(false);
            p.setColor(QPalette::Window, QColor(57,63,77,255));
        }
        else if (level == ToastUtils::WARN)
        {
            //mCloseBtn->setVisible(true);
            p.setColor(QPalette::Window, QColor(34,34,34,255));//黑色
        }
        else//ERROR
        {
            p.setColor(QPalette::Window, QColor(255,0,0,200));
        }
        setPalette(p);//set widget's palette

        mLabel->setText(text);
        setWindowFlag(Qt::WindowStaysOnTopHint);//通知窗口系统该窗口应保持在所有其他窗口的顶部。
        setAttribute(Qt::WA_ShowWithoutActivating, true);
        setParent(nullptr);
        QDialog::show();
        QDesktopWidget* desktop = QApplication::desktop(); // =qApp->desktop();也可以
        move((desktop->width() - this->width())/2, desktop->height() - this->height()-100);

    }
};//~class ToastDlg end


ToastUtils::ToastUtils()
{
    mDlg = new ToastDlg;
}
//返回一个实例(instance)
ToastUtils &ToastUtils::instance()
{
    static ToastUtils thiz;//这种实例化方法会自动回收内存
    return thiz;
}

void ToastUtils::show(ToastUtils::Level level, const QString &text)
{
    mDlg->show(level, text);//ToastDlg.show方法
    if (mTimerId != 0)//int mTimerId
    {
        //如果之前已经开启了一个定时器，先把他关掉
        killTimer(mTimerId);
    }
    mTimerId = startTimer(2000);//启动定时器，每2s触发定时器事件，直到调用killTimer

}

void ToastUtils::timerEvent(QTimerEvent *event)
{
    killTimer(mTimerId);
    mTimerId = 0;
    mDlg->accept();//隐藏模态对话框

}
