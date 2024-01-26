#ifndef UDPLISTENER_H
#define UDPLISTENER_H

#include "common/common.h"
#include "common/log.h"
#include "common/rthread.h"
#include <QUdpSocket>
#include <QMutex>

class UdpListener :public RThread
{
public:
    UdpListener();

    virtual void run();
    void setPlay(bool is){isPlay = is;}

private:
    QUdpSocket *receiver;
    QMutex m_lock;
    bool isPlay = false;
};

#endif // UDPLISTENER_H
