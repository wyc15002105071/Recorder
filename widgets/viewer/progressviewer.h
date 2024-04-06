#ifndef PROGRESSVIEWER_H
#define PROGRESSVIEWER_H

#include <QWidget>
#include "widgets/basewidget.h"
#include "common/observer.h"

namespace Ui {
class ProgressViewer;
}

class ProgressViewer : public BaseWidget,
                       public Observer
{
    Q_OBJECT

public:
#define COPYING 		"拷贝中······"
#define DELETEING 		"删除中······"
#define COPY_SUCCESS 	"拷贝完成······"
#define DELETE_SUCCESS	"删除完成······"
#define COPY_FAILED 	"拷贝失败······"
#define DELETE_FAILED 	"删除失败······"
#define FREE_NO 	    "移动设备内存不足"
#define LOAD 	        "加载中........"

    explicit ProgressViewer(QWidget *parent = nullptr);
    ~ProgressViewer();

    void setOperation(int operation);

    virtual void update(NotifyData data);
    void showWarning(QString info);
signals:
    void onUpdateValue(int value);
    void onUpdateText(QString text);
    void onFinish();
public slots:
    void setText(QString text);
    void setValue(int value);
    void finish();
private:
    Ui::ProgressViewer *ui;
    int mOperation;

};

#endif // PROGRESSVIEWER_H
