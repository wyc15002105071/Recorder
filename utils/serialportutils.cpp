#include "serialportutils.h"
#include <QSerialPortInfo>

#define MODULE_TAG "SerialPortUtils"

SerialPortUtils::SerialPortUtils()
    : mSerialPort(sp<QSerialPort>(new QSerialPort))
    , mInitOk(false)
{
    connect(mSerialPort.get(),&QSerialPort::readyRead,this,[=]{
        onDataAvailable();
    },Qt::UniqueConnection);
}

QList<QString> SerialPortUtils::getAvailablePort()
{
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();

    QList<QString> portNames;

    for(int i = 0;i<portList.count();i++) {
        portNames.push_back(portList.at(i).portName());
    }

    return portNames;
}

bool SerialPortUtils::startTask()
{

}

void SerialPortUtils::stopTask()
{
    mThreadExit = true;
    quit();
    wait();
    if(mSerialPort) {
        mSerialPort->close();
    }
}

void SerialPortUtils::reset()
{

}

bool SerialPortUtils::open(QString portName,int bandrate)
{
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    for(int i = 0;i<portList.count();i++) {
        QSerialPortInfo info = portList.at(i);
        if(portName == info.portName()) {
            if(mSerialPort->isOpen())
                mSerialPort->close();
            mSerialPort->setPort(info);  //设置串口
            if (!mSerialPort->open(QIODevice::ReadWrite)) {
                RLOGD("open port %s failed",portName.toLatin1().data());
                return false;
            }

            mSerialPort->setBaudRate(bandrate);          //波特率
            mSerialPort->setParity(QSerialPort::NoParity);   //设置奇偶校验
            mSerialPort->setDataBits(QSerialPort::Data8);    //设置数据位数
            mSerialPort->setStopBits(QSerialPort::OneStop);  //设置停止位
            mSerialPort->setFlowControl(QSerialPort::NoFlowControl);  //设置流控制
            mInitOk = true;
            RLOGD("open port %s success",portName.toLatin1().data());
            return true;
        }
    }

    RLOGE("not find port %s",portName.toLatin1().data());
    return false;
}

QByteArray SerialPortUtils::readData()
{
    QByteArray data;
    if(mSerialPort && mInitOk) {
        data = mSerialPort->readAll();
    }

    return data;
}

void SerialPortUtils::sendData(QByteArray data)
{
    if(mSerialPort && mInitOk) {
        RLOGE("serial send %s",data.toHex());
        mSerialPort->write(data);
    }
}

void SerialPortUtils::run()
{

}
