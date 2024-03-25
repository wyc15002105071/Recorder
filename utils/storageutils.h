#ifndef STORAGEUTILS_H
#define STORAGEUTILS_H
#include <stdio.h>
#include <libudev.h>
#include <string.h>
#include "common/common.h"
#include <string>
#include <QVector>

class StorageUtils
{
public:
    static StorageUtils *get_instance() {
        static StorageUtils utils;
        return &utils;
    }

    typedef struct ExternalStorageInfo {
        std::string node_path;
        std::string label;
        std::string mount_path;
        std::string file_system;
        long    total;
        long    free;
        long    used;
    }ExternalStorageInfo_t;

    QVector<ExternalStorageInfo> getExternalStorageInfoList();

    void getStorageCapacity(const char *root,long &total,long &used ,long &free);

    void getUsePath();

protected:
    StorageUtils();

private:
    const char *getMountPath(const char *node_path);
private:
    QVector<ExternalStorageInfo> mInfoVec;
};

#endif // STORAGEUTILS_H
