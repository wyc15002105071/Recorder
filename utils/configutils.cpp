#include "configutils.h"
#include <QDebug>
#include <QFile>
#include <QSettings>
#include <QTextCodec>


QString ConfigUtils::ConfigFile = "config.ini";
bool ConfigUtils::isPowOff = false;
bool ConfigUtils::isUsbMedia = false;
QString ConfigUtils::usbName = "";
QString ConfigUtils::usbMonut = "";
QString ConfigUtils::usbLabel = "";
int ConfigUtils::Fps = 2;
int ConfigUtils::time = 0;
QString ConfigUtils::MainMediaPath = "/mnt/storage";
QString ConfigUtils::ost_txt = "清阅技术";

bool checkIniFile(const QString &iniFile) {
  //如果配置文件大小为0,则以初始值继续运行,并生成配置文件
  QFile file(iniFile);
  if (file.size() == 0) {
    return false;
  }
  //file.setPermissions(QFile::ReadOwner|QFile::WriteOwner|QFile::ReadGroup|QFile::WriteGroup|QFile::ReadUser|QFile::WriteUser);
  //如果配置文件不完整,则以初始值继续运行,并生成配置文件
  if (file.open(QFile::ReadOnly)) {
    bool ok = true;
    while (!file.atEnd()) {
      QString line = file.readLine();
      line.replace("\r", "");
      line.replace("\n", "");
      QStringList list = line.split("=");

      if (list.count() == 2) {
        if (list.at(1) == "") {
            if(list.at(0)=="Department"||list.at(0)=="Department"||list.at(0)=="Department"){

            }else{
                qDebug() << "ini node no value" << list.at(0);
                ok = false;
            }
          break;
        }
      }
    }

    if (!ok) {
      return false;
    }
  } else {
    return false;
  }

  return true;
}


void ConfigUtils::readConfig()
{
    if (!checkIniFile(ConfigUtils::ConfigFile)) {
        writeConfig();
        return;
    }
    QSettings set(ConfigUtils::ConfigFile, QSettings::IniFormat);
    set.setIniCodec(QTextCodec::codecForName("GBK"));
    set.beginGroup("system");
    ConfigUtils::ost_txt = set.value("ost_txt", ConfigUtils::ost_txt).toString();
    ConfigUtils::Fps = set.value("Fps", ConfigUtils::Fps).toInt();
    ConfigUtils::time = set.value("time", ConfigUtils::time).toInt();
    set.endGroup();

}

void ConfigUtils::writeConfig()
{
    QSettings set(ConfigUtils::ConfigFile,QSettings::IniFormat);
    set.setIniCodec(QTextCodec::codecForName("GBK"));
    set.beginGroup("system");
    set.setValue("ost_txt", ConfigUtils::ost_txt);
    set.setValue("Fps", ConfigUtils::Fps);
    set.setValue("time", ConfigUtils::time);
    set.endGroup();
}
