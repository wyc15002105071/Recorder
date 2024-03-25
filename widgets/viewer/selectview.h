#ifndef SELECTVIEW_H
#define SELECTVIEW_H

#include <QWidget>
#include <QPushButton>
namespace Ui {
class SelectView;
}

class SelectView : public QWidget
{
    Q_OBJECT

public:
    explicit SelectView(QWidget *parent = nullptr);
    ~SelectView();

    void initList(QList<QString> list,int index);

private slots:
    void on_pBtn_sure_clicked();
signals:
    void checkedIndex(int index);
private:
    void clearLayout();
private:
    Ui::SelectView *ui;
    QList<QString> list;
    QList<QPushButton *> buttonList;
};

#endif // SELECTVIEW_H
