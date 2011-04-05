#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutwindow.h"
#include "manwindow.h"
#include "appwindow.h"
#include <QString>
#include "../control/connectioncontrol.h"
QString userName;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    connectionControl_ = new ConnectionControl();
    aw = new AboutWindow();
    mw = new ManWindow();
    apw = new AppWindow(connectionControl_);
    this->applyStyleSheet(QString(":/files/styles.qss"));
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
    userName = ui->userValue->text();
    if(!connectionControl_->startServer(ui->serverTCPPortValue->text().toInt(),
                                        ui->serverUDPPortValue->text().toInt())) {
        QMessageBox::warning(this, QString("Failed to start server"),
                    QString("This will only run as a client."),
                    QMessageBox::Ok);
    }
    if(!connectionControl_->connectToServer(ui->tcpIpValue->text(),
                                            ui->clientTCPPortValue->text().toInt())) {
        QMessageBox::warning(this, QString("Failed to connect to server"),
                     QString("No server available. This will only run as a server."),
                     QMessageBox::Ok);
    }
    apw->applyStyleSheet(":/files/styles.qss");
    apw->show();
    this->hide();
}

void MainWindow::openManual() {
    mw->show();
}

MainWindow::~MainWindow() {
    delete ui;
}

ConnectionControl* MainWindow::getConnectionControl() {
    return connectionControl_;
}

void MainWindow::applyStyleSheet(QString path) {
    QFile f(path);
    f.open(QIODevice::ReadOnly);
    this->setStyleSheet(QString(f.readAll()));
    f.close();
}
