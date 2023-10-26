#include "observer.h"

using namespace std;

Observer::Observer()
{

}

void Observer::attach(Observer *obs)
{
    mLock.lock();
    mObservers.push_back(obs);
}

void Observer::detach(Observer *obs)
{
    mLock.lock();
    mObservers.remove(obs);
}

void Observer::notify(OBSERVERDATA data)
{
    mLock.lock();

    if(mObservers.size() <= 0)
        return;

    list<Observer*>::iterator it = mObservers.begin();
    while(it != mObservers.end()){
        (*it)->update(data);
        it++;
    }
}

Observer::~Observer()
{
    mLock.lock();
    if(mObservers.size() > 0) {
        mObservers.clear();
    }
}
