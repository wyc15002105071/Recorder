#ifndef MUTEX_H
#define MUTEX_H

#include <mutex>
#include <pthread.h>

class Mutex {
public:
    Mutex(){}
    void lock(){
//        std::unique_lock<std::mutex> mux(mLock);
        pthread_mutex_lock(&mMtx);
    }
    void unlock() {
        pthread_mutex_unlock(&mMtx);
    }
    void condWait() {
        pthread_cond_wait(&mCond, &mMtx);
    }
    void condSignal() {
        pthread_cond_signal(&mCond);
    }
    void condBroadCast() {
        pthread_cond_broadcast(&mCond);
    }
private:
//    std::mutex mLock;
    pthread_mutex_t mMtx = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t mCond = PTHREAD_COND_INITIALIZER;
};

#endif // MUTEX_H
