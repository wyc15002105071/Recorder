#ifndef OBSERVER_H
#define OBSERVER_H

#include "Mutex.h"
#include "list"

typedef void* OBSERVERDATA;

class Observer
{
public:
    Observer();
    virtual void update(OBSERVERDATA data){}

    void attach(Observer *obs);
    void detach(Observer *obs);
    void notify(OBSERVERDATA data);
    ~Observer();
private:
    std::list<Observer *>mObservers;
    Mutex mLock;
};

#endif // OBSERVER_H
