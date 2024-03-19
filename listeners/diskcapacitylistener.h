#ifndef DISKCAPACITYLISTENER_H
#define DISKCAPACITYLISTENER_H

#include "common/common.h"
#include "common/log.h"
#include "common/rthread.h"

class DiskCapacityListener :public RThread
{

    Q_OBJECT

public:
    DiskCapacityListener();

    virtual void run();
signals:
    void sendDiskSpace(long free,long total);

private:
    QString path;
};

#endif // DISKCAPACITYLISTENER_H
