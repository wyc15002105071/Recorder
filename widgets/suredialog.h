#ifndef SUREDIALOG_H
#define SUREDIALOG_H

#include <QDialog>
#include "listeners/key_listener.h"

namespace Ui {
class SureDialog;
}

class SureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SureDialog(QWidget *parent = nullptr);
    ~SureDialog();


public slots:
    void onKeyEventHandler(KeyListener::EventType type);
private:
    Ui::SureDialog *ui;    
    KeyListener *mKeyListener;
};

#endif // SUREDIALOG_H
