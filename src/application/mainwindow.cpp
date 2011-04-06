#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutwindow.h"
#include "manwindow.h"
#include "appwindow.h"
#include <QString>
#include <QBitmap>
#include "../control/connectioncontrol.h"
QString userName;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setWindowFlags(Qt::FramelessWindowHint);
    QPixmap pixmap(":/files/mainWindowBg.png");
    this->setMask(pixmap.mask());

    connectionControl_ = new ConnectionControl();
    aw = new AboutWindow();
    mw = new ManWindow();
    apw = new AppWindow(connectionControl_);
    this->applyStyleSheet(QString(":/files/styles.qss"));
    ui->setupUi(this);
    ui->manMenu->setEnabled(TRUE);
    ui->aboutMenu->setEnabled(TRUE);
    ui->connect->setEnabled(TRUE);

    connect(ui->manual, SIGNAL(clicked()), this, SLOT(openManual()));
    connect(ui->about, SIGNAL(clicked()), this, SLOT(openAbout()));
    connect(ui->connect, SIGNAL(clicked()), this, SLOT(openApp()));
    connect(ui->quit, SIGNAL(clicked()), this, SLOT(close()));

    QShortcut* connectShortcut = new QShortcut(QKeySequence("ALT+c"), this);
    connect(connectShortcut, SIGNAL(activated()), this, SLOT(openApp()));
}

void MainWindow::openAbout() {
    aw->applyStyleSheet(QString(":/files/styles.qss"));
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
                                            ui->clientTCPPortValue->text().toInt(),
                                            ui->clientUDPPortValue->text().toInt())) {
        QMessageBox::warning(this, QString("Failed to connect to server"),
                     QString("No server available. This will only run as a server."),
                     QMessageBox::Ok);
    }
    apw->addUser(userName);
    apw->applyStyleSheet(QString(":/files/styles.qss"));
    apw->show();
    this->hide();
}

void MainWindow::openManual() {
    mw->applyStyleSheet(QString(":/files/styles.qss"));
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
