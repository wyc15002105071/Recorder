#ifndef SETFPSVIEW_H
#define SETFPSVIEW_H

#include <QWidget>

namespace Ui {
class SetFpsView;
}

class SetFpsView : public QWidget
{
    Q_OBJECT

public:
    explicit SetFpsView(QWidget *parent = nullptr);
    ~SetFpsView();

signals:
    void setFps(int fps);

private slots:
    void on_pBtn_cancel_clicked();

    void on_pBtn_high_clicked();

    void on_pBtn_low_clicked();

    void on_pBtn_medium_clicked();

private:
    Ui::SetFpsView *ui;
};

#endif // SETFPSVIEW_H
