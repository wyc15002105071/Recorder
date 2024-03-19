#ifndef CONFIGUTILS_H
#define CONFIGUTILS_H

#include <QString>

class ConfigUtils
{
public:
    static QString ConfigFile;  //配置文件路径

    static QString ost_txt;
    static bool isPowOff;
    static int Fps;

    static QString MainMediaPath;

    //读写配置参数
    static void readConfig();   //读取配置参数
    static void writeConfig();  //写入配置参数

};

#endif // CONFIGUTILS_H
