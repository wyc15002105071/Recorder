#ifndef RTHREAD_H
#define RTHREAD_H

#include <QThread>
#include "Mutex.h"

class RThread : public QThread
{
public:
    RThread();
    virtual ~RThread() {}
    virtual void run(){};
    virtual bool startTask(){ return true; }
    virtual void stopTask(){}
    virtual void reset(){}
protected:
    bool mThreadExit;
    Mutex mLock;
    const int QUIT_TIMEOUT = 500;
};

#endif // RTHREAD_H
