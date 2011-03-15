#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutwindow.h"
#include "manwindow.h"
#include "appwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    aw = new AboutWindow();
    mw = new ManWindow();
    apw = new AppWindow();

    ui->setupUi(this);
    ui->manMenu->setEnabled(TRUE);
    ui->aboutMenu->setEnabled(TRUE);
    ui->connect->setEnabled(TRUE);

    connect(ui->manMenu, SIGNAL(triggered()), this, SLOT(openManual()));
    connect(ui->aboutMenu, SIGNAL(triggered()), this, SLOT(openAbout()));
    connect(ui->connect, SIGNAL(clicked()), this, SLOT(openApp()));
}

void MainWindow::openAbout() {
    aw->show();
}

void MainWindow::openApp() {
    apw->show();
}

void MainWindow::openManual() {
    mw->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
