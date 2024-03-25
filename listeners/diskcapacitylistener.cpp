#include "diskcapacitylistener.h"
#include "utils/storageutils.h"
#include "utils/mediapathutils.h"

DiskCapacityListener::DiskCapacityListener()
{
}

void DiskCapacityListener::run()
{
    long total=0,used=0,free=0;
    while (!mThreadExit) {
        //RLOGE(path.toLatin1().constData());
        StorageUtils::get_instance()->getStorageCapacity(
                    MediaPathUtils::get_instance()->getRootPath().toStdString().c_str()
                    //"/mnt/storage"
                    ,total,used,free);
        emit sendDiskSpace(free/1024/1024,total/1024/1024);

        //RLOGE("free= %d total= %d",free/1024/1024,total/1024/1024);
        sleep(4);
    }
}
