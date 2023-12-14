#工程编译及打包说明

编译脚本为工程源码目录下的make.sh，需要修改make.sh中的SDK_DIR为自己的RKSDK源码路径
修改完成后直接运行./make.sh编译工程

编译完成后需要将编译生成的Recorder可执行文件以及third-party/prebuilt/linux_arm32打包一起adb push到/userdata目录下
然后运行程序：
cd /userdata
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./linux_arm32
chmod 777 Recorder
./Recorder

如果程序运行后出现OpenGL相关报错，将linux_arm32/libmali.so.1.9.0拷贝到/usr/lib目录然后重新运行