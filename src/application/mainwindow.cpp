#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutwindow.h"
#include "manwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    aw = new AboutWindow();
    mw = new ManWindow();

    ui->setupUi(this);
    ui->manMenu->setEnabled(TRUE);
    ui->aboutMenu->setEnabled(TRUE);

    connect(ui->manMenu, SIGNAL(triggered()), this, SLOT(openManual()));
    connect(ui->aboutMenu, SIGNAL(triggered()), this, SLOT(openAbout()));
}

void MainWindow::openAbout() {
    aw->show();
}

void MainWindow::openManual() {
    mw->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
