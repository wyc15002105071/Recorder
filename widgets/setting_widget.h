#ifndef SETTING_WIDGET_H
#define SETTING_WIDGET_H

#include <QWidget>

namespace Ui {
class SettingWidget;
}

class SettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingWidget(QWidget *parent = nullptr);
    ~SettingWidget();

private:
    Ui::SettingWidget *ui;
};

#endif // SETTING_WIDGET_H
