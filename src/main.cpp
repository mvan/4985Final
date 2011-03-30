#include <QtGui/QApplication>
#include "application/mainwindow.h"
#include "network/network.h"

int main(int argc, char *argv[])
{
    WinsockInit();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
