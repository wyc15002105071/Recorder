#ifndef CONFIRMDIALOG_H
#define CONFIRMDIALOG_H

#include <QDialog>
#include "listeners/key_listener.h"

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

public slots:
    void onKeyEventHandler(KeyListener::EventType type);
private:
    Ui::ConfirmDialog *ui;
    KeyListener *mKeyListener;
};

#endif // CONFIRMDIALOG_H
