#ifndef CONFIRMDIALOG_H
#define CONFIRMDIALOG_H

#include <QDialog>

namespace Ui {
class ConfirmDialog;
}

class ConfirmDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmDialog(QWidget *parent = nullptr);
    ~ConfirmDialog();

    void setTitle(QString title);
private:
    Ui::ConfirmDialog *ui;
};

#endif // CONFIRMDIALOG_H
