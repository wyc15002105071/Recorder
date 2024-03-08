#include "storageutils.h"
#include "common/log.h"
#include <mntent.h>
#include <sys/statvfs.h>
#include <unistd.h>
//#include <QThread>

#define MODULE_TAG "StorageUtils"

using namespace std;

StorageUtils::StorageUtils()
{

}

QVector<StorageUtils::ExternalStorageInfo> StorageUtils::getExternalStorageInfoList()
{
    mInfoVec.clear();

    struct udev_monitor *monitor;
    struct udev_enumerate *enumerate;
    struct udev_list_entry *devices;
    struct udev_list_entry *entry;
    struct udev *udev = udev_new();
    if (!udev) {
        RLOGE("Failed to create udev context");
        goto RET;
    }

    // 创建udev监视器用于监视USB设备
    monitor = udev_monitor_new_from_netlink(udev, "udev");
    udev_monitor_filter_add_match_subsystem_devtype(monitor, "block", "disk");
    udev_monitor_enable_receiving(monitor);

    // 获取初始设备列表
    enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, "block");
    udev_enumerate_scan_devices(enumerate);
    devices = udev_enumerate_get_list_entry(enumerate);

    // 遍历设备列表并输出U盘的设备信息
    udev_list_entry_foreach(entry, devices) {
        struct udev_device *dev = udev_device_new_from_syspath(udev, udev_list_entry_get_name(entry));
        // 检查是否为U盘
        if (dev) {
            const char *devnode = udev_device_get_devnode(dev);
            const char *model = udev_device_get_property_value(dev, "ID_MODEL");
            const char *serial_short = udev_device_get_property_value(dev, "ID_SERIAL_SHORT");
            const char *filesystem = udev_device_get_property_value(dev, "ID_FS_TYPE");
            const char *label = udev_device_get_property_value(dev, "ID_FS_LABEL");

            //RLOGD("==============");
            //if(devnode)
            //RLOGD(devnode);
            //if(model)
            //RLOGD(model);
            //if(serial_short)
            //RLOGD(serial_short);
            //if(filesystem)
            //RLOGD(filesystem);
            //if(!label){
            //    label = ;
            //}
            //RLOGD(label);
            //RLOGD("==============");
            if (devnode && model && serial_short && filesystem) {

                const char *mnt_path = getMountPath(devnode);
                if(!mnt_path)
                    continue;
                ExternalStorageInfo info;
                info.node_path = devnode;
                if(label){
                    info.label = label;
                }else{
                    info.label ="qy";
                    info.label.append(std::to_string(mInfoVec.size()+1));
                }
                info.file_system = filesystem;
                info.mount_path = mnt_path;
                getStorageCapacity(mnt_path,info.total,info.used,info.free);
                RLOGD("node:%s,label:%s,filesystem:%s,mnt_path:%s",info.node_path.c_str()
                      ,info.label.c_str(),info.file_system.c_str(),info.mount_path.c_str());
                if(info.node_path.find("/dev/sda")<0||info.mount_path!="/mnt/storage")
                mInfoVec.push_back(info);
            }
            udev_device_unref(dev);
        }
    }

    // 清理资源
    if(enumerate)
        udev_enumerate_unref(enumerate);
    if(monitor)
        udev_monitor_unref(monitor);
    if(udev)
        udev_unref(udev);

RET:
    return mInfoVec;
}

void StorageUtils::getStorageCapacity(const char *root, long &total, long &used, long &free)
{
    if(!root||access(root, F_OK) != 0) {
        RLOGE("%s not exists",root);
        total = 0;
        used = 0;
        free = 0;
        return;
    }

    struct statvfs buf;
    if (statvfs(root, &buf) == 0) {
        total = buf.f_frsize * buf.f_blocks; // 磁盘总容量
        free = buf.f_frsize * buf.f_bfree; // 可用容量
        used = total - free; // 已使用容量
    } else {
        RLOGE("Error in getting disk space information");
        total = 0;
        used = 0;
        free = 0;
    }
}

const char *StorageUtils::getMountPath(const char *node_path)
{
    FILE *mounts;
    struct mntent *ent;

    // 打开 /etc/mtab 文件以获取挂载点信息
    mounts = setmntent("/etc/mtab", "r");
    if (mounts == NULL) {
        perror("Error opening /etc/mtab");
        return NULL;
    }

    // 读取并输出挂载信息
    while ((ent = getmntent(mounts)) != NULL) {
        if(!strcmp(node_path,ent->mnt_fsname)) {
            return ent->mnt_dir;
        }
    }

    // 关闭文件
    endmntent(mounts);

    return NULL;
}
