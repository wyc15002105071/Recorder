#include "diskcapacitylistener.h"
#include "utils/storageutils.h"

DiskCapacityListener::DiskCapacityListener()
{
    path = "/mnt/storage";
}

void DiskCapacityListener::run()
{
    long total=0,used=0,free=0;
    while (!mThreadExit) {
        StorageUtils::get_instance()->getStorageCapacity("/mnt/storage",total,used,free);
        emit sendDiskSpace(free/1024/1024,total/1024/1024);

        //RLOGE("free= %d total= %d",free/1024/1024,total/1024/1024);
        sleep(6);
    }
}
