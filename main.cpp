#include "main_widget.h"

#include <QApplication>
#include "utils/configutils.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
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
