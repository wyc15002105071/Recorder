#include "observer.h"

using namespace std;

Observer::Observer()
{

}

void Observer::attach(Observer *obs)
{
    mMutex.lock();
    mObservers.push_back(obs);
    mMutex.unlock();
}

void Observer::detach(Observer *obs)
{
    mMutex.lock();
    mObservers.remove(obs);
    mMutex.unlock();
}

void Observer::notify(NotifyData data)
{
    mMutex.lock();

    if(mObservers.size() <= 0) {
        mMutex.unlock();
        return;
    }

    list<Observer*>::iterator it = mObservers.begin();
    while(it != mObservers.end()){
        (*it)->update(data);
        it++;
    }
    mMutex.unlock();
}

Observer::~Observer()
{
    mMutex.lock();
    if(mObservers.size() > 0) {
        mObservers.clear();
    }
    mMutex.unlock();
}
