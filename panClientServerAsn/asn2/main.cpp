/** ---------------------------------------------------------------------------
  * Source file:    main.cpp - An entry point to the program.
  *
  * Program:        Winsock2 client - server
  *
  * Functions:      int main(int argc, char *argv[])
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
    --------------------------------------------------------------------------- */

#include <QtGui/QApplication>
#include "mainwindow.h"
#include "serverwindow.h"

/** ---------------------------------------------------------------------------
  * Function:       main - OEntry point of the program
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  * Starts the main window.
    --------------------------------------------------------------------------- */
int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    MainWindow w;
    w.show();

    return app.exec();
}
