#ifndef POWOFFWIDGET_H
#define POWOFFWIDGET_H

#include <QWidget>

namespace Ui {
class PowOffWidget;
}

class PowOffWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PowOffWidget(QWidget *parent = nullptr);
    ~PowOffWidget();

private:
    Ui::PowOffWidget *ui;
};

#endif // POWOFFWIDGET_H
