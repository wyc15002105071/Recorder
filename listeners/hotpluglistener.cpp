#include "hotpluglistener.h"
#include <libudev.h>

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
