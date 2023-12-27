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
    virtual bool startTask(){
        mThreadExit = false;
        this->start();
        return true;
    }
    virtual void stopTask() {
        mThreadExit = true;
        quit();
        wait(QUIT_TIMEOUT);
    }
    virtual void reset(){}
protected:
    bool mThreadExit;
    Mutex mLock;
    const int QUIT_TIMEOUT = 1000;
};

#endif // RTHREAD_H
