#ifndef OBSERVER_H
#define OBSERVER_H

#include "Mutex.h"
#include "list"

typedef enum Result{
    RET_SUCCESS = 0,
    RET_FAILURE = -1,
    RET_NONE = -2
} Result_t;

typedef struct NotifyData {
    void    *data;
    Result  result;
    bool    finish;
    int     flags;
    bool    isUsbOut;
    bool    isAdd;
    bool    isNoLegal;
}NotifyData_t;

class Observer
{
public:
    Observer();
    virtual void update(NotifyData data){}

    void attach(Observer *obs);
    void detach(Observer *obs);
    void notify(NotifyData data);

    ~Observer();

private:
    std::list<Observer *>mObservers;
    RMutex mMutex;
};

#endif // OBSERVER_H
