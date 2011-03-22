#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serverwindow.h"
#include "clientwindow.h"

/** ---------------------------------------------------------------------------
  * Constructor:    MainWindow
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * @param          parent - The parent object
    --------------------------------------------------------------------------- */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Start server
    ServerWindow *sWindow = new ServerWindow();
    connect(*(&ui->startServerBtn), SIGNAL(clicked()), sWindow, SLOT(show()));

    //Start client
    ClientWindow *cWindow = new ClientWindow();
    connect(*(&ui->startClientBtn), SIGNAL(clicked()), cWindow, SLOT(show()));
}

/** ---------------------------------------------------------------------------
  * Destructor:     ~MainWindow
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
    --------------------------------------------------------------------------- */
MainWindow::~MainWindow()
{
    delete ui;
}
