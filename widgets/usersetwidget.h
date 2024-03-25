#ifndef USERSETWIDGET_H
#define USERSETWIDGET_H

#include <QWidget>
#include "common/common.h"
#include "basewidget.h"
#include "widgets/viewer/osdview.h"
#include "widgets/viewer/setfpsview.h"
#include "widgets/viewer/selectview.h"
#include "utils/storageutils.h"

namespace Ui {
class UserSetWidget;
}

class UserSetWidget : public BaseWidget
{
    Q_OBJECT

public:
    explicit UserSetWidget(QWidget *parent = nullptr);
    ~UserSetWidget();

    virtual void onHasOpened();
public slots:
    void doEditOsd();
    void doSelectFps();
    void setFps(int index);
    void setTime(int index);
    void setDiZhi(int index);
    void setPage(int page);

private slots:
    void on_pBtn_left_clicked();
    void on_pBtn_right_clicked();
    void on_pBtn_time_clicked();
    void on_pBtn_dizhi_clicked();

private:
    Ui::UserSetWidget *ui;
    sp<OsdView>   mOsdView;
    sp<SelectView>mTimeSelectView;
    sp<SelectView>mSetFpsView;
    sp<SelectView>mSetDiZhiView;
    sp<StorageUtils>mStorageUtils;
    int fps = 0;
    int page = 1;
    int time = 0;
    int dizhi = 0;

    QVector<StorageUtils::ExternalStorageInfo> mExternalStorageInfo;
    QList<QString> timeList = {"30min","60min","90min","120min"};
    QList<QString> fpsList = {"低","中","高"};
};

#endif // USERSETWIDGET_H
