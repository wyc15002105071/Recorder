#include "key_listener.h"

#define MODULE_TAG "KeyListener"

KeyListener::KeyListener()
    : mSerialPort(sp<SerialPortUtils>(new SerialPortUtils))
{
    connect(mSerialPort.get(),SIGNAL(onDataAvailable()),this,SLOT(onReadData()),Qt::UniqueConnection);
}

KeyListener::~KeyListener()
{
    stopTask();
}

bool KeyListener::startTask()
{
    mThreadExit = false;

    bool find_port = false;
    QList<QString>ports = mSerialPort->getAvailablePort();
    for(int i = 0;i<ports.count();i++) {
        if(!strcmp(KEY_PORT,ports.at(i).toLatin1().data())) {
            RLOGD("find key port:%s",KEY_PORT);
            find_port = true;
            break;
        }
    }

    if(!find_port)
        return false;
    if(!mSerialPort->open(KEY_PORT,BAUD_RATE)) {
        RLOGD("open serial port failed");
        return false;
    }
//    mSerialPort->start();
    RLOGD("open serial port success");
    start();
    return true;
}

void KeyListener::stopTask()
{
    mThreadExit = true;
    quit();
    wait();

    if(mSerialPort) {
        mSerialPort->stopTask();
    }
}

void KeyListener::onReadData()
{
    if(mSerialPort) {
        QByteArray data_all = mSerialPort->readData();
        QByteArray data_head;
        QByteArray data_body;
        QByteArray data_end;

        if(data_all.length() > 0) {
            if(data_all.length() == 8) {
                data_end.push_back(data_all[6]);
                data_end.push_back(data_all[7]);

                data_body.push_back(data_all[4]);
                data_body.push_back(data_all[5]);

                data_head.push_back(data_all[0]);
                data_head.push_back(data_all[1]);
                data_head.push_back(data_all[2]);
                data_head.push_back(data_all[3]);

                bool ok;
                long long head = data_head.toHex().toLongLong(&ok,16);
                long long end = data_end.toHex().toLongLong(&ok,16);
                long long body = data_body.toHex().toLongLong(&ok,16);
                if((head == DATA_HEAD) && (end == DATA_END)) {
                    KeyType type = KEY_TYPE_UNKNOWN;
                    switch (body) {
                    case RECORD:
                        type = KEY_TYPE_RECORD;
                        break;
                    case RIGHT:
                        type = KEY_TYPE_RIGHT;
                        break;
                    case DOWN:
                        type = KEY_TYPE_DOWN;
                        break;
                    case UP:
                        type = KEY_TYPE_UP;
                        break;
                    case LEFT:
                        type = KEY_TYPE_LEFT;
                        break;
                    case CAPTURE:
                        type = KEY_TYPE_CAPTURE;
                        break;
                    default:
                        break;
                    }

                    RLOGD("key pressed id is %d",type);
                    onPressed(type);
                }
            }
        } else {
            RLOGE("get serial null data");
        }
    }
}

void KeyListener::run()
{

//    while(!mThreadExit) {

//    }
}
