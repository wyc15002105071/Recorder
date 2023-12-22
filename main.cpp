#include "main_widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;

    struct sigaction interrupt;

    interrupt.sa_handler = SignalHandler::intSignalHandler;
    sigemptyset(&interrupt.sa_mask);
    interrupt.sa_flags = 0;
    interrupt.sa_flags |= SA_RESTART;
    sigaction(SIGINT, &interrupt, 0);

    w.show();
    return a.exec();
}
