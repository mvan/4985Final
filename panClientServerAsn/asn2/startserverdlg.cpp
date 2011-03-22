/** ---------------------------------------------------------------------------
  * Source file:    startserverdlg.cpp - A connection dialog.
  *
  * Program:        Winsock2 client - server
  *
  * Functions:      explicit startServerDlg(QWidget *parent = 0);
  *                 ~startServerDlg();
  *                 bool startServer();
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
    --------------------------------------------------------------------------- */
#include "startserverdlg.h"
#include "ui_startserverdlg.h"

/** ---------------------------------------------------------------------------
  * Constructor:    startServerDlg
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * @param          parent - The parent object
    --------------------------------------------------------------------------- */
startServerDlg::startServerDlg(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::startServerDlg){
    ui->setupUi(this);
    connect(*(&ui->startServerBtn), SIGNAL(clicked()), this, SLOT(startServer()));
}

/** ---------------------------------------------------------------------------
  * Destructor:     ~startServerDlg
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
    --------------------------------------------------------------------------- */
startServerDlg::~startServerDlg(){
    delete ui;
}

/** ---------------------------------------------------------------------------
  * Function:       startServer - Start the server for user's spcified port and
  *                     protocol.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  *
  * @return         True if succesfully started, false otherwise
    --------------------------------------------------------------------------- */
bool startServerDlg::startServer(){
    ServerWindow *parent = (ServerWindow *)this->parent();
    WSADATA wsaData;
    SOCKET serverSocket;
    SOCKADDR_IN serverAddr;
    WORD wVersionRequested = MAKEWORD (2,2);
    int WSAstatus, protocol, port, setSockOptRet, soRcvBuf = 122880;

    //Check if the user put the port
    if(ui->port->toPlainText().isEmpty()){
        QMessageBox::critical(this, QString("Port required"), QString("Please enter a desired port"), QMessageBox::Ok);
        return false;
    }

    if(parent != 0){
        parent->setStatusBar("Starting server ...");
    }

    //Start up WSA
    if ((WSAstatus = WSAStartup(wVersionRequested,&wsaData)) != 0){
        QMessageBox::critical(this, QString("WSAStartup failed"), QString("WSAStartup failed with error ").append(QString::number(WSAstatus, 10)), QMessageBox::Ok);
        WSACleanup();
        parent->clearStatusBar();
        return false;
    }

    //WSASocket(AF_INET)
    port = ui->port->toPlainText().toInt();
    protocol = ui->tcpRadio->isChecked() ? IPPROTO_TCP : IPPROTO_UDP;

    parent->setPort(port);
    parent->setProtocol(protocol);

    //Get a socket
    if ((serverSocket = WSASocket(AF_INET, (protocol == IPPROTO_TCP) ? SOCK_STREAM : SOCK_DGRAM, protocol, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET){
        QMessageBox::critical(this, QString("Socket failed"), QString("Failed to get a socket: ").append(QString::number(WSAGetLastError(), 10)), QMessageBox::Ok);
        WSACleanup();
        parent->clearStatusBar();
        return false;
    }

    //Bind the socket
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);
    if (bind(serverSocket, (PSOCKADDR) &serverAddr, sizeof(serverAddr)) == SOCKET_ERROR){
        QMessageBox::critical(this, QString("Bind failed"), QString("bind() failed with error ").append(QString::number(WSAGetLastError(), 10)), QMessageBox::Ok);
        closesocket(serverSocket);
        WSACleanup();
        parent->clearStatusBar();
        return false;
    }

    //Set socket options
    setSockOptRet = setsockopt(serverSocket, SOL_SOCKET, SO_RCVBUF, (char *)&soRcvBuf, sizeof(int));
    if(setSockOptRet == SOCKET_ERROR){
        QMessageBox::critical(this, QString("Socket setting failed"), QString("setsockopt() failed with error ").append(QString::number(WSAGetLastError(), 10)), QMessageBox::Ok);
        WSACleanup();
        parent->clearStatusBar();
        return false;
    }

    parent->setServerSocket(serverSocket);

    parent->fillServerPort();
    parent->fillServerProtocol();
    this->accept();

    return true;
}
