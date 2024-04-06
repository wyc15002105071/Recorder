#include "hotpluglistener.h"
#include "utils/configutils.h"
#include <libudev.h>
#include <mntent.h>


HotplugListener::HotplugListener()
{

}

void HotplugListener::run()
{
    struct udev *udev;
    struct udev_monitor *mon;
    int fd;

    // 初始化 udev
    udev = udev_new();
    if (!udev) {
        RLOGE("Failed to initialize udev");
        return;
    }

    // 创建监视器
    mon = udev_monitor_new_from_netlink(udev, "udev");
    udev_monitor_filter_add_match_subsystem_devtype(mon, "block", NULL);
    udev_monitor_enable_receiving(mon);

    // 获取监视器的文件描述符
    fd = udev_monitor_get_fd(mon);
    while(!mThreadExit) {
        fd_set fds;
        struct timeval tv;
        int ret;

        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        // 设置超时时间
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        // 等待文件描述符上的事件
        ret = select(fd + 1, &fds, NULL, NULL, &tv);

        if (ret > 0 && FD_ISSET(fd, &fds)) {
            // 从监视器接收事件
            struct udev_device *dev = udev_monitor_receive_device(mon);
            if (dev) {

                NotifyData data;
                memset(&data,0,sizeof(NotifyData));
                QString devnode = udev_device_get_devnode(dev);
                if(!devnode.isEmpty()){
                    const char * action = udev_device_get_action(dev);

                    if(strcmp(action, "remove") == 0){
                        data.isAdd = false;
                        if(devnode==ConfigUtils::usbName){
                            ConfigUtils::isUsbMedia = false;
                            ConfigUtils::usbName = "";
                            ConfigUtils::usbMonut = "";
                            ConfigUtils::usbLabel = "";
                            data.isUsbOut = true;
                        }
                    }else if(strcmp(action, "add") == 0){
                        const char *mnt_path = getMountPath(devnode.toStdString().c_str());
                        if(mnt_path){
                            RLOGD(action);
                            data.isAdd = true;
                        }
                        QString filesystem = udev_device_get_property_value(dev, "ID_FS_TYPE");
                        if(filesystem == "ntfs")
                            data.isNoLegal = true;
                    }
                }

                notify(data);
                udev_device_unref(dev);
            }
        }else if (ret == 0) {
            //no event
        }else {
            RLOGE("select failed:%s",strerror(errno));
            break;
        }
        usleep(1*1000);
    }

    // 清理资源
    udev_monitor_unref(mon);
    udev_unref(udev);
}

const char *HotplugListener::getMountPath(const char *node_path)
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

