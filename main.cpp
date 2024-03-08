#include "main_widget.h"

#include <QApplication>
#include "utils/configutils.h"

bool checkOnly() {
  const char filename[] = "/tmp/recorder_qy";
  int fd = open(filename, O_WRONLY | O_CREAT, 0644);
  int flock = lockf(fd, F_TLOCK, 0);
  if (fd == -1) {
    qDebug() << "open lockfile";
    return false;
  }
  //给文件加锁
  if (flock == -1) {
    qDebug() << "lock file error/n";
    return false;
  }
  //程序退出后，文件自动解锁
  return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!checkOnly()) return 0;

    MainWidget w;

    //init config
    ConfigUtils::ConfigFile = QCoreApplication::applicationDirPath() + "/config.ini";
    ConfigUtils::readConfig();

    struct sigaction interrupt;

    interrupt.sa_handler = SignalHandler::intSignalHandler;
    sigemptyset(&interrupt.sa_mask);
    interrupt.sa_flags = 0;
    interrupt.sa_flags |= SA_RESTART;
    sigaction(SIGINT, &interrupt, 0);

    w.show();
    return a.exec();
}
