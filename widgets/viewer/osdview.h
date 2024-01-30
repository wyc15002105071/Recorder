#ifndef OSDVIEW_H
#define OSDVIEW_H

#include <QWidget>

namespace Ui {
class OsdView;
}

class OsdView : public QWidget
{
    Q_OBJECT

public:
    explicit OsdView(QWidget *parent = nullptr);
    ~OsdView();

public slots:
    void open();
    void doSure();
private:
    Ui::OsdView *ui;
};

#endif // OSDVIEW_H
