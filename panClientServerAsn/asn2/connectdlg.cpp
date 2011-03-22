/** ---------------------------------------------------------------------------
  * Source file:    connectdlg.cpp - A connection dialog.
  *
  * Program:        Winsock2 client - server
  *
  * Functions:      explicit connectDlg(QWidget *parent = 0);
  *                 ~connectDlg();
  *                 void connectToSrv();
  *                 bool connectToTcp();
  *                 bool connectToUdp();
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
    --------------------------------------------------------------------------- */
#include "clientwindow.h"
#include "connectdlg.h"
#include "ui_connectdlg.h"

using namespace std;

/** ---------------------------------------------------------------------------
  * Constructor:    connectDlg
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * @param          parent - The parent object
    --------------------------------------------------------------------------- */
connectDlg::connectDlg(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::connectDlg){
    ui->setupUi(this);

    connect(*(&ui->connectBtn), SIGNAL(clicked()), this, SLOT(connectToSrv()));
}

/** ---------------------------------------------------------------------------
  * Destructor:     ~connectDlg
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
    --------------------------------------------------------------------------- */
connectDlg::~connectDlg(){
    delete ui;
}

/** ---------------------------------------------------------------------------
  * Function:       connectToSrv - Connect to the server, according to user's
  *                     selected protocol.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
    --------------------------------------------------------------------------- */
void connectDlg::connectToSrv(){
    if(ui->tcpRadio->isChecked()){
        connectToTcp();
    } else {
        connectToUdp();
    }
}

/** ---------------------------------------------------------------------------
  * Function:       connectToTcp - Connect to a TCP server.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  *
  * @return         True if succesfully connected, false otherwise
    --------------------------------------------------------------------------- */
bool connectDlg::connectToTcp(){
    ClientWindow *parent = (ClientWindow *)this->parent();
    QString serverIP, serverPort;
    WSADATA wsaData;
    SOCKET clientSocket;
    struct hostent *hp;
    struct sockaddr_in server;
    WORD wVersionRequested = MAKEWORD (2,2);
    int WSAstatus, protocol, port;

    serverIP = ui->serverIP->toPlainText();
    serverPort = ui->serverPort->toPlainText();

    //Check if the user put the port
    if(serverIP.isEmpty() || serverPort.isEmpty()){
        QMessageBox::critical(this, QString("IP and port required"), QString("Please enter a desired ip address and port"), QMessageBox::Ok);
        return false;
    }

    if(parent != 0){
        parent->setStatusBar("Connecting to server ...");
    }

    //Start up WSA
    if ((WSAstatus = WSAStartup(wVersionRequested,&wsaData)) != 0){
        QMessageBox::critical(this, QString("WSAStartup failed"), QString("WSAStartup failed with error ").append(QString::number(WSAstatus, 10)), QMessageBox::Ok);
        WSACleanup();
        parent->clearStatusBar();
        return false;
    }

    //WSASocket(AF_INET)
    port = ui->serverPort->toPlainText().toInt();
    protocol = ui->tcpRadio->isChecked() ? IPPROTO_TCP : IPPROTO_UDP;

    parent->setHostIP(serverIP);
    parent->setPort(port);
    parent->setProtocol(protocol);

    //Get a socket
    if ((clientSocket = WSASocket(AF_INET, (protocol == IPPROTO_TCP) ? SOCK_STREAM : SOCK_DGRAM, protocol, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET){
        QMessageBox::critical(this, QString("Socket failed"), QString("Failed to get a socket: ").append(QString::number(WSAGetLastError(), 10)), QMessageBox::Ok);
        WSACleanup();
        parent->clearStatusBar();
        return false;
    }

    parent->setClientSocket(clientSocket);

    //Initialize and set up the address structure
    memset((char *)&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if ((hp = gethostbyname(serverIP.toAscii())) == NULL){
        QMessageBox::critical(this, QString("Unknown server address"), QString("Unable to resolve server address: ").append(serverIP), QMessageBox::Ok);
        WSACleanup();
        parent->clearStatusBar();
        return false;
    }

    //Copy the server address
    memcpy((char *)&server.sin_addr, hp->h_addr, hp->h_length);

    //Connecting to the server
    if (WSAConnect(clientSocket, (struct sockaddr *)&server, sizeof(server), NULL, NULL, NULL, NULL) == -1){
        QMessageBox::critical(this, QString("Connect failed"), QString("Can't connect to server"), QMessageBox::Ok);
        WSACleanup();
        parent->clearStatusBar();
        return false;
    }

    this->accept();

    return true;
}

/** ---------------------------------------------------------------------------
  * Function:       connectToUdp - Connect to a UDP server.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  *
  * @return         True if succesfully connected, false otherwise
    --------------------------------------------------------------------------- */
bool connectDlg::connectToUdp(){
    ClientWindow *parent = (ClientWindow *)this->parent();
    QString serverIP, serverPort;
    WSADATA wsaData;
    SOCKET clientSocket;
    struct hostent *hp;
    struct sockaddr_in server;
    WORD wVersionRequested = MAKEWORD (2,2);
    int WSAstatus, protocol, port;

    serverIP = ui->serverIP->toPlainText();
    serverPort = ui->serverPort->toPlainText();

    //Check if the user put the port
    if(serverIP.isEmpty() || serverPort.isEmpty()){
        QMessageBox::critical(this, QString("IP and port required"), QString("Please enter a desired ip address and port"), QMessageBox::Ok);
        return false;
    }

    if(parent != 0){
        parent->setStatusBar("Connecting to server ...");
    }

    //Start up WSA
    if ((WSAstatus = WSAStartup(wVersionRequested,&wsaData)) != 0){
        QMessageBox::critical(this, QString("WSAStartup failed"), QString("WSAStartup failed with error ").append(QString::number(WSAstatus, 10)), QMessageBox::Ok);
        WSACleanup();
        parent->clearStatusBar();
        return false;
    }

    //WSASocket(AF_INET)
    port = ui->serverPort->toPlainText().toInt();
    protocol = ui->tcpRadio->isChecked() ? IPPROTO_TCP : IPPROTO_UDP;

    parent->setHostIP(serverIP);
    parent->setPort(port);
    parent->setProtocol(protocol);

    //Get a socket
    if ((clientSocket = WSASocket(AF_INET, (protocol == IPPROTO_TCP) ? SOCK_STREAM : SOCK_DGRAM, protocol, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET){
        QMessageBox::critical(this, QString("Socket failed"), QString("Failed to get a socket: ").append(QString::number(WSAGetLastError(), 10)), QMessageBox::Ok);
        WSACleanup();
        parent->clearStatusBar();
        return false;
    }

    parent->setClientSocket(clientSocket);

    //Initialize and set up the address structure
    memset((char *)&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if ((hp = gethostbyname(serverIP.toAscii())) == NULL){
        QMessageBox::critical(this, QString("Unknown server address"), QString("Unable to resolve server address: ").append(serverIP), QMessageBox::Ok);
        WSACleanup();
        parent->clearStatusBar();
        return false;
    }

    //Copy the server address
    memcpy((char *)&server.sin_addr, hp->h_addr, hp->h_length);
    parent->setServerSockaddr(server);

    this->accept();

    return true;
}
