#ifndef HOTPLUGLISTENER_H
#define HOTPLUGLISTENER_H

#include "common/common.h"
#include "common/log.h"
#include "common/rthread.h"
#include "common/observer.h"

class HotplugListener : public RThread ,
                        public Observer
{
public:
    HotplugListener();

    virtual void run();
};

#endif // HOTPLUGLISTENER_H
