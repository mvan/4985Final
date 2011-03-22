/** ---------------------------------------------------------------------------
  * Source file:    servewindow.cpp - A server application that will
  *                     receive a file to a server through a Windows socket.
  *
  * Program:        Winsock2 client - server
  *
  * Functions:      void displayError(QString title, QString msg);
  *                 void cleanStoppedServer();
  *                 void addClient(SOCKET clientSocket);
  *                 void setStatusBar(QString text, int timeout = 0);
  *                 void addPacket(unsigned int bytesReceived, int duration = 0);
  *                 explicit ServerWindow(QWidget *parent = 0);
  *                 ~ServerWindow();
  *                 QString getServerIP();
  *                 void setServerIP(QList<QHostAddress> *serverIPList = 0);
  *                 int getPort();
  *                 void setPort(int port);
  *                 int getProtocol();
  *                 void setProtocol(int protocol);
  *                 SOCKET getServerSocket();
  *                 void setServerSocket(SOCKET serverSocket);
  *                 void fillServerIP();
  *                 void fillServerPort();
  *                 void fillServerProtocol();
  *                 void clearStatusBar();
  *                 void runServer();
  *                 void stopServer();
  *                 void getSaveFileName(QEventLoop *eventLoop);
  *                 void runTCPServer();
  *                 void runUDPServer();
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  * Server listens to a specific socket. Once there's an incomming connection
  * from user, it read the packets and save to a file.
    --------------------------------------------------------------------------- */
#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "startserverdlg.h"

using namespace std;

/** ---------------------------------------------------------------------------
  * Constructor:    ServerWindow
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * @param          parent - The parent object
    --------------------------------------------------------------------------- */
ServerWindow::ServerWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::ServerWindow),
        serverIPList_(new QList<QHostAddress>),
        protocol_(SOCK_STREAM),
        packetReceived_(0),
        totalPacketReceived_(0),
        bytesReceived_(0),
        totalBytesReceived_(0),
        lastPacketSize_(0),
        avgPacketSize_(0),
        listenThread_(this){
    ui->setupUi(this);

    clientProcThreadList_ = new QList<ClientProcessThread *>;

    startServerDlg *startSrvDlg = new startServerDlg(this);

    qRegisterMetaType<SOCKET>("SOCKET");

    QObject::connect(*(&ui->startServerBtn), SIGNAL(triggered()), startSrvDlg, SLOT(show()));
    QObject::connect(*(&ui->stopServerBtn), SIGNAL(triggered()), this, SLOT(stopServer()));
    QObject::connect(startSrvDlg, SIGNAL(accepted()), this, SLOT(runServer()));
    QObject::connect(&listenThread_, SIGNAL(errorOccurred(QString,QString)), this, SLOT(displayError(QString,QString)));
    QObject::connect(&listenThread_, SIGNAL(serverStopped()), this, SLOT(cleanStoppedServer()));
    QObject::connect(&listenThread_, SIGNAL(clientConnected(SOCKET)), this, SLOT(addClient(SOCKET)));

    //Show the server IP address
    setServerIP();
    fillServerIP();
}

/** ---------------------------------------------------------------------------
  * Destructor:     ~ServerWindow
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
    --------------------------------------------------------------------------- */
ServerWindow::~ServerWindow(){
    delete ui;
}

/** ---------------------------------------------------------------------------
  * Function:       displayError - Display an error.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  * This method displays a dialog box, with an error sign as an icon.
  *
  * @param          title -  The title of the dialog
  * @param          msg - The body message of the dialog
    --------------------------------------------------------------------------- */
void ServerWindow::displayError(QString title, QString msg){
    QMessageBox::critical(this, title, msg, QMessageBox::Ok);
}

/** ---------------------------------------------------------------------------
  * Function:       cleanStoppedServer - Clean server window.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
    --------------------------------------------------------------------------- */
void ServerWindow::cleanStoppedServer(){
    setStatusBar("Server stopped", 10000);

    ui->serverPort->clear();
    ui->serverProtocol->clear();
}

/** ---------------------------------------------------------------------------
  * Function:       addClient - Start a client thread.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  *
  * @param          clientSocket -  An opened client's socket
    --------------------------------------------------------------------------- */
void ServerWindow::addClient(SOCKET clientSocket){
    packetReceived_ = 0;
    bytesReceived_ = 0;
    totalPacketReceived_--;
    ClientProcessThread *thread = new ClientProcessThread(clientSocket, this);
    clientProcThreadList_->push_back(thread);
    QObject::connect(thread, SIGNAL(errorOccurred(QString,QString)), this, SLOT(displayError(QString,QString)));
    QObject::connect(thread, SIGNAL(serverStopped()), this, SLOT(cleanStoppedServer()));
    QObject::connect(thread, SIGNAL(receivedData(QEventLoop*)), this, SLOT(getSaveFileName(QEventLoop*)));
    QObject::connect(thread, SIGNAL(changeStatus(QString)), this, SLOT(setStatusBar(QString)));
    QObject::connect(thread, SIGNAL(receivedMoreData(uint)), this, SLOT(addPacket(uint)));
    thread->start();
    ui->numClient->setPlainText(QString::number(clientProcThreadList_->size(), 10));
}

/** ---------------------------------------------------------------------------
  * Function:       setServerIP - Translate the user's input into an IP address.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  *
  * @param          serverIPList -  IP address
    --------------------------------------------------------------------------- */
void ServerWindow::setServerIP(QList<QHostAddress> *serverIPList){
    if(serverIPList == 0){
        *serverIPList_ = QNetworkInterface::allAddresses();
        for (int i = 0; i < serverIPList_->size(); ++i) {
            if (serverIPList_->at(i) != QHostAddress::LocalHost && serverIPList_->at(i).toIPv4Address()) {
                serverIPStr_ = serverIPList_->at(i).toString();
                break;
            }
        }
    } else {
        serverIPList_ = serverIPList;
    }
}

/** ---------------------------------------------------------------------------
  * Function:       getters and setters for private data members
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  *
    --------------------------------------------------------------------------- */
QString ServerWindow::getServerIP(){
    return serverIPStr_;
}

int ServerWindow::getPort(){
    return port_;
}

void ServerWindow::setPort(int port){
    port_ = port;
}

int ServerWindow::getProtocol(){
    return protocol_;
}

void ServerWindow::setProtocol(int protocol){
    protocol_ = protocol;
}

SOCKET ServerWindow::getServerSocket(){
    return serverSocket_;
}

void ServerWindow::setServerSocket(SOCKET serverSocket){
    serverSocket_ = serverSocket;
}
/** ---------------------------------------------------------------------------
  * ========   End of getters and setters   ========
    --------------------------------------------------------------------------- */

/** ---------------------------------------------------------------------------
  * Function:       fillServerIP - Fill the IP address into the textbox in the
  *                     window.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
    --------------------------------------------------------------------------- */
void ServerWindow::fillServerIP(){
    ui->serverIP->setPlainText(serverIPStr_);
}

/** ---------------------------------------------------------------------------
  * Function:       fillServerPort - Fill the server port into the textbox in the
  *                     window.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
    --------------------------------------------------------------------------- */
void ServerWindow::fillServerPort(){
    ui->serverPort->setPlainText(QString::number(port_, 10));
}

/** ---------------------------------------------------------------------------
  * Function:       fillServerProtocol - Fill the server protocol into the
  *                     textbox in the window.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
    --------------------------------------------------------------------------- */
void ServerWindow::fillServerProtocol(){
    ui->serverProtocol->setPlainText(QString::fromAscii((protocol_ == IPPROTO_TCP) ? "TCP" : "UDP"));
}

/** ---------------------------------------------------------------------------
  * Function:       clearStatusBar - Clear the status bar of the window.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
    --------------------------------------------------------------------------- */
void ServerWindow::clearStatusBar(){
    ui->statusbar->clearMessage();
}

/** ---------------------------------------------------------------------------
  * Function:       setStatusBar - Set text to the status bar of the window.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  *
  * @param          text -  The text to be displayed in the
  * @param          timeout - The duration, in millisecond, for the text to show
  *                     in the status bar.
    --------------------------------------------------------------------------- */
void ServerWindow::setStatusBar(QString text, int timeout){
    ui->statusbar->showMessage(text, timeout);
}

/** ---------------------------------------------------------------------------
  * Function:       addPacket - Set statistic data for each packet received.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  *
  * @param          bytesReceived -  The number of bytes received from the last
  *                     transfer.
  * @param          duration - The duration, in millisecond, for how long the
  *                     last packet takes to send.
    --------------------------------------------------------------------------- */
void ServerWindow::addPacket(unsigned int bytesReceived, int duration){
    packetReceived_++;
    totalPacketReceived_++;
    bytesReceived_ += bytesReceived;
    totalBytesReceived_ += bytesReceived;
    lastPacketSize_ = bytesReceived;
    avgPacketSize_ = (double)totalBytesReceived_/totalPacketReceived_;

    ui->packetsReceived->setPlainText(QString::number((packetReceived_-1), 10));
    ui->totalPacketsReceived->setPlainText(QString::number((totalPacketReceived_), 10));
    ui->bytesReceived->setPlainText(QString::number((bytesReceived_), 10).append(" Bytes"));
    ui->totalBytesReceived->setPlainText(QString::number((totalBytesReceived_), 10).append(" Bytes"));
    ui->lastPacketSize->setPlainText(QString::number((lastPacketSize_), 10));
    ui->avgPacketSize->setPlainText(QString::number((avgPacketSize_), 'f', 4));
}


/** ---------------------------------------------------------------------------
  * Function:       runServer - Start the server according the the protocol.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
    --------------------------------------------------------------------------- */
void ServerWindow::runServer(){
    ui->stopServerBtn->setEnabled(true);
    ui->startServerBtn->setEnabled(false);
    ui->numClient->setPlainText("0");
    setStatusBar("Server is running");

    switch(protocol_){
    case IPPROTO_TCP:
        runTCPServer();
        break;
    case IPPROTO_UDP:
        runUDPServer();
        break;
    default:
        displayError(QString("Invalid protocol"), QString("Make sure you choose correct protocol"));
        return;
    }
}

/** ---------------------------------------------------------------------------
  * Function:       stopServer - Stop the running server.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
    --------------------------------------------------------------------------- */
void ServerWindow::stopServer(){
    listenThread_.terminate();
    closesocket(serverSocket_);
    WSACleanup();
    setStatusBar("Server stopped", 10000);
    ui->stopServerBtn->setEnabled(false);
    ui->startServerBtn->setEnabled(true);
    clientProcThreadList_->empty();
    ui->numClient->setPlainText("0");
    ui->packetsReceived->clear();
    ui->totalPacketsReceived->clear();
    ui->bytesReceived->clear();
    ui->totalBytesReceived->clear();
    ui->lastPacketSize->clear();
    ui->avgPacketSize->clear();
    ui->serverProtocol->clear();
    ui->serverPort->clear();
    packetReceived_ = 0;
    bytesReceived_ = 0;
    totalPacketReceived_ = 0;
    totalBytesReceived_ = 0;
    lastPacketSize_ = 0;
    avgPacketSize_ = 0;
}

/** ---------------------------------------------------------------------------
  * Function:       getSaveFileName - Open a file dialog, and save the file name
  *                     into a private data member.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  * @param          eventLoop - An event loop using to stop blocking the thread.
    --------------------------------------------------------------------------- */
void ServerWindow::getSaveFileName(QEventLoop *eventLoop){
    saveFileName_ = QFileDialog::getSaveFileName();
    emit selectedFile(saveFileName_);
    eventLoop->exit();
}

/** ---------------------------------------------------------------------------
  * Function:       runServer - Start the TCP server.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
    --------------------------------------------------------------------------- */
void ServerWindow::runTCPServer(){
    //Listen for a client connection
    if (listen(serverSocket_, 5)){
        displayError(QString("Listen failed"), QString("listen() failed with error ").append(QString::number(WSAGetLastError(), 10)));
        closesocket(serverSocket_);
        WSACleanup();
        setStatusBar("Server stopped", 10000);
        return;
    }

    listenThread_.setServerSocket(serverSocket_);
    listenThread_.start();
}

/** ---------------------------------------------------------------------------
  * Function:       runServer - Start the UDP server.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
    --------------------------------------------------------------------------- */
void ServerWindow::runUDPServer(){
    ClientProcessThread *thread = new ClientProcessThread(serverSocket_, this);
    QObject::connect(thread, SIGNAL(errorOccurred(QString,QString)), this, SLOT(displayError(QString,QString)));
    QObject::connect(thread, SIGNAL(serverStopped()), this, SLOT(cleanStoppedServer()));
    QObject::connect(thread, SIGNAL(receivedData(QEventLoop*)), this, SLOT(getSaveFileName(QEventLoop*)));
    QObject::connect(thread, SIGNAL(changeStatus(QString)), this, SLOT(setStatusBar(QString)));
    QObject::connect(thread, SIGNAL(receivedMoreData(uint)), this, SLOT(addPacket(uint)));
    thread->start();
    ui->numClient->setPlainText(QString("-"));
}
