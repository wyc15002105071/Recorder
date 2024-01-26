#ifndef SERIALPORTUTILS_H
#define SERIALPORTUTILS_H

#include <QList>
#include "common/Mutex.h"
#include "common/common.h"
#include "common/log.h"
#include "common/rthread.h"
#include <QSerialPort>

class SerialPortUtils : public RThread
{
    Q_OBJECT
public:
    SerialPortUtils();

    QList<QString> getAvailablePort();
    virtual bool startTask();
    virtual void stopTask();
    virtual void reset();
    bool open(QString portName,int bandrate);
    QByteArray readData();

protected:
    virtual void run();
private:
    sp<QSerialPort> mSerialPort;
    bool mInitOk;

public slots:
    void sendData(QByteArray data);
signals:
    void onDataAvailable();
};

#endif // SERIALPORTUTILS_H
