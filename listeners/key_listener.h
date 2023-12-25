#ifndef KEYLISTENER_H
#define KEYLISTENER_H

#include "common/Mutex.h"
#include "common/common.h"
#include "common/log.h"
#include "common/rthread.h"
#include "utils/serialportutils.h"

#define KEY_PORT   "ttyS9"
#define BAUD_RATE   115200
#define DATA_HEAD   0x5AA50401
#define DATA_END    0xEFEF
#define RECORD      0x01EC
#define RIGHT       0x02BF
#define DOWN        0x038E
#define UP          0x0419
#define LEFT        0x0528
#define CAPTURE     0x067B

class KeyListener : public RThread
{
    Q_OBJECT
public:
    static KeyListener *get_instance() {
        static KeyListener key_listener;
        return &key_listener;
    }

    typedef enum EventType {
        Key_EventType_UNKNOWN = -1,
        Key_EventType_RECORD = 1,
        Key_EventType_RIGHT,
        Key_EventType_DOWN,
        Key_EventType_UP,
        Key_EventType_LEFT,
        Key_EventType_CAPTURE
    }EventType_t;

    ~KeyListener();
    virtual bool startTask();
    virtual void stopTask();

public slots:
    void onReadData();

protected:
    virtual void run();
    KeyListener();

signals:
    void onPressed(KeyListener::EventType type);

private:
    sp<SerialPortUtils> mSerialPort;
};

#endif // KEYLISTENER_H
