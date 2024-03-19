#ifndef USERSETWIDGET_H
#define USERSETWIDGET_H

#include <QWidget>
#include "common/common.h"
#include "basewidget.h"
#include "widgets/viewer/osdview.h"
#include "widgets/viewer/setfpsview.h"

namespace Ui {
class UserSetWidget;
}

class UserSetWidget : public BaseWidget
{
    Q_OBJECT

public:
    explicit UserSetWidget(QWidget *parent = nullptr);
    ~UserSetWidget();
public slots:
    void doEditOsd();
    void doSelectFps();
    void setFps(int fps);

private:
    Ui::UserSetWidget *ui;
    sp<OsdView>   mOsdView;
    sp<SetFpsView>mSetFpsView;
    int fps = 0;
};

#endif // USERSETWIDGET_H
