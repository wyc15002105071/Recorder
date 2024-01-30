#ifndef USERSETWIDGET_H
#define USERSETWIDGET_H

#include <QWidget>
#include "common/common.h"
#include "basewidget.h"
#include "widgets/viewer/osdview.h"

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

private:
    Ui::UserSetWidget *ui;
    sp<OsdView>   mOsdView;
};

#endif // USERSETWIDGET_H
