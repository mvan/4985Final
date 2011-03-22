/** ---------------------------------------------------------------------------
  * Source file:    clientprocessthread.cpp - A client application that will
  *                     send a file to a server through a Windows socket.
  *
  * Program:        Winsock2 client - server
  *
  * Functions:      void displayError(QString title, QString msg);
  *                 explicit ClientWindow(QWidget *parent = 0);
  *                 ~ClientWindow();
  *                 QString getHostIPStr();
  *                 void setHostIP(QString hostIP);
  *                 int getPort();
  *                 void setPort(int port);
  *                 int getProtocol();
  *                 void setProtocol(int protocol);
  *                 SOCKET getClientSocket();
  *                 void setClientSocket(SOCKET clientSocket);
  *                 struct sockaddr_in getServerSockaddr();
  *                 void setServerSockaddr(struct sockaddr_in sockaddr);
  *                 unsigned int getPacketSize();
  *                 void setPacketSize(unsigned int packetSize);
  *                 unsigned int getTimesToSend();
  *                 void setTimesToSend(unsigned int packetSize);
  *                 void clearStatusBar();
  *                 void setStatusBar(QString text, int timeout = 0);
  *                 bool selectFile();
  *                 bool sendFile();
  *                 void runClient();
  *                 void disconnectFromSrv();
  *                 void resetWindow();
  *                 bool loadFile(QString fileName);
  *                 void setFileSize(int fileSize);
  *                 int getFileSize();
  *                 static void CALLBACK sendRoutine(DWORD error, DWORD bytesTransferred, LPWSAOVERLAPPED overlapped, DWORD inFlags);
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  * The user needs to specify the server IP address, and a server port. You
  * can choose the protocol(TCP/UDP), and the packet size. Once you successfully
  * connected, you will be prompted to choose a file to send. Then the program
  * will automatically disconnect.
    --------------------------------------------------------------------------- */
#include "clientwindow.h"
#include "ui_clientwindow.h"

using namespace std;

/** ---------------------------------------------------------------------------
  * Initialize static data member
    --------------------------------------------------------------------------- */
ClientWindow *mySelf = 0;

/** ---------------------------------------------------------------------------
  * Constructor:    ClientWindow
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * @param          parent - The parent object
    --------------------------------------------------------------------------- */
ClientWindow::ClientWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::ClientWindow),
        totalBytesSent_(0),
        packetSent_(0),
        packetSize_(0),
        totalPacketSent_(0),
        lastPacketSize_(0),
        avgPacketSize_(0),
        bytesSent_(0),
        flags_(0){
    ui->setupUi(this);

    mySelf = this;
    ZeroMemory(&overlapped_, sizeof(WSAOVERLAPPED));

    //Connect slots/signals
    connectDlg *connDlg = new connectDlg(this);
    QObject::connect(*(&ui->selectFileBtn), SIGNAL(triggered()), this, SLOT(resetWindow()));
    QObject::connect(*(&ui->selectFileBtn), SIGNAL(triggered()), connDlg, SLOT(show()));
    QObject::connect(connDlg, SIGNAL(accepted()), this, SLOT(runClient()));
    QObject::connect(*(&ui->sendFileBtn), SIGNAL(clicked()), this, SLOT(sendFile()));

    //Group the menus together, so you can select only 1 or the other
    packetSizeGroup_ = new QActionGroup(this);
    packetSizeGroup_->addAction(ui->pkSize1024B);
    packetSizeGroup_->addAction(ui->pkSize120KB);
    packetSizeGroup_->addAction(ui->pkSize20KB);
    packetSizeGroup_->addAction(ui->pkSize4096B);
    packetSizeGroup_->addAction(ui->pkSize60KB);

    timesToSendGroup_ = new QActionGroup(this);
    timesToSendGroup_->addAction(ui->send1000Times);
    timesToSendGroup_->addAction(ui->send100Times);
    timesToSendGroup_->addAction(ui->send10Times);
}

/** ---------------------------------------------------------------------------
  * Destructor:     ~ClientWindow
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
    --------------------------------------------------------------------------- */
ClientWindow::~ClientWindow(){
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
void ClientWindow::displayError(QString title, QString msg){
    QMessageBox::critical(this, title, msg, QMessageBox::Ok);
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
QString ClientWindow::getHostIPStr(){
    return hostIPStr_;
}

void ClientWindow::setHostIP(QString hostIP){
    hostIPStr_ = hostIP;
}

int ClientWindow::getPort(){
    return port_;
}

void ClientWindow::setPort(int port){
    port_ = port;
}

int ClientWindow::getProtocol(){
    return protocol_;
}

void ClientWindow::setProtocol(int protocol){
    protocol_ = protocol;
}

SOCKET ClientWindow::getClientSocket(){
    return clientSocket_;
}

void ClientWindow::setClientSocket(SOCKET clientSocket){
    clientSocket_ = clientSocket;
}

struct sockaddr_in ClientWindow::getServerSockaddr(){
    return serverSockaddr_;
}

void ClientWindow::setServerSockaddr(sockaddr_in sockaddr){
    serverSockaddr_ = sockaddr;
}

int ClientWindow::getFileSize(){
    return fileSize_;
}
/** ---------------------------------------------------------------------------
  * ========   End of getters and setters   ========
    --------------------------------------------------------------------------- */

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
void ClientWindow::clearStatusBar(){
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
void ClientWindow::setStatusBar(QString text, int timeout){
    ui->statusbar->showMessage(text, timeout);
}

/** ---------------------------------------------------------------------------
  * Function:       resetWindow - Clear the textboxes in the window. Prepareing
  *                     for a new connection statistics.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
    --------------------------------------------------------------------------- */
void ClientWindow::resetWindow(){
    ui->bytesTransfered->clear();
    ui->fileName->clear();
    ui->fileSize->clear();
    ui->totalTransferTimeMin->clear();
    ui->totalTransferTimeMs->clear();
    ui->totalTransferTimeSec->clear();
    ui->serverIP->clear();
    ui->packetsTransfered->clear();
    ui->totalPacketsSent->clear();
    ui->totalBytesSent->clear();
    ui->avgPacketSize->clear();
    ui->lastPacketSize->clear();
}

/** ---------------------------------------------------------------------------
  * Function:       selectFile - Open a file chooser dialog box and let the user
  *                     select a file.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  *
  * @return         True if the user selected a file, false otherwise
    --------------------------------------------------------------------------- */
bool ClientWindow::selectFile(){
    QString fileName = QFileDialog::getOpenFileName(this);
    loadFile(fileName);
    ui->fileName->setPlainText(fileName);
    return true;
}

/** ---------------------------------------------------------------------------
  * Function:       sendFile - Read the user selected file to our local buffer,
  *                     then send the file using Winsock assynchronize IO.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  *
  * @return         True if it successfully read and send a file, false otherwise
    --------------------------------------------------------------------------- */
bool ClientWindow::sendFile(){
    WSABUF data;
    char *buff;
    int setSockOptRet, ret = 0, timeMs, serverLen = sizeof(sockaddr);
    double timeSec, timeMin;
    QTime *totalTimer = new QTime();

    ui->selectFileBtn->setEnabled(false);

    packetSize_ = packetSizeGroup_->checkedAction()->whatsThis().toUInt();
    timesToSend_ = timesToSendGroup_->checkedAction()->text().toUInt();
    buff = new char[packetSize_];
    packetSent_ = 0;
    bytesSent_  = 0;

    //Set socket options
    setSockOptRet = setsockopt(clientSocket_, SOL_SOCKET, SO_SNDBUF, (char *)&packetSize_, sizeof(int));
    if(setSockOptRet == SOCKET_ERROR){
        displayError(QString("Socket setting failed"), QString("setsockopt() failed with error ").append(QString::number(WSAGetLastError(), 10)));
        closesocket(clientSocket_);
        WSACleanup();
        clearStatusBar();
        return false;
    }

    totalTimer->restart();
    //check if there's more data to be sent
    while(totalBytesSent_ < fileSize_){
        ZeroMemory(buff, packetSize_);
        //Read from file
        fread(buff, 1, packetSize_, file_);
        data.len = ((fileSize_-totalBytesSent_) > packetSize_) ? packetSize_ : (fileSize_-totalBytesSent_);
        data.buf = buff;

        //Send according to the protocol the user chose
        if(protocol_ == IPPROTO_TCP){
            ret = WSASend(clientSocket_, &data, 1, &bytesSent_, flags_, &overlapped_, &sendRoutine);
        } else if(protocol_ == IPPROTO_UDP){
            ret = WSASendTo(clientSocket_, &data, 1, &bytesSent_, flags_, (struct sockaddr *)&serverSockaddr_, serverLen, &overlapped_, &sendRoutine);
        }

        if(ret == SOCKET_ERROR){
            switch(WSAGetLastError()){
            case WSA_IO_PENDING:
                break;
            default:
                displayError(QString("Sending failed"), QString("WSASend() failed with error ").append(QString::number(WSAGetLastError(), 10)));
                return false;
            }
        }

        //Wait for a completion routine to be completed
        SleepEx(INFINITE, true);
        ui->progressBar->setValue(totalBytesSent_);
    }

    //Display total transfer time
    timeMs = totalTimer->elapsed();
    timeSec = (double)timeMs/1000;
    timeMin = (double)timeMs/1000/60;
    ui->totalTransferTimeMs->setPlainText(QString::number(timeMs, 10).append(" ms"));
    ui->totalTransferTimeSec->setPlainText(QString::number(timeSec, 'f', 4).append(" sec"));
    ui->totalTransferTimeMin->setPlainText(QString::number(timeMin, 'f', 4).append(" min"));

    //Notify the server about the end of transfer
    if(protocol_ == IPPROTO_UDP){
        *data.buf = 0x04;
        data.len = 1;
        ret = WSASendTo(clientSocket_, &data, 1, &bytesSent_, flags_, (struct sockaddr *)&serverSockaddr_, serverLen, &overlapped_, &sendRoutine);
    }

    //Disconnect
    disconnectFromSrv();
    return true;
}

/** ---------------------------------------------------------------------------
  * Function:       loadFile - Open user's selected file.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  *
  * @param          fileName -  The name of the file to be opened
  * @return         True if successfully opened, false otherwise
    --------------------------------------------------------------------------- */
bool ClientWindow::loadFile(QString fileName){
    filenameStr_ = fileName.toStdString();

    file_ = fopen(filenameStr_.c_str(), "rb");
    if(file_ != NULL){
        fseek(file_, 0, SEEK_END);
        setFileSize(ftell(file_));
        rewind(file_);
    }

    ui->sendFileBtn->setEnabled(true);
    totalBytesSent_ = 0;

    return true;
}

/** ---------------------------------------------------------------------------
  * Function:       setFileSize - Set and display the file size.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  *
  * @param          fileSize -  The name of the file to be opened
    --------------------------------------------------------------------------- */
void ClientWindow::setFileSize(int fileSize){
    fileSize_ = fileSize;
    char tmpStr[64];
    QString qtmpStr;

    qtmpStr = itoa(fileSize, tmpStr, 10);
    qtmpStr.append(" Bytes");
    ui->fileSize->setPlainText(qtmpStr);
    ui->progressBar->setMaximum(fileSize_);
}

/** ---------------------------------------------------------------------------
  * Function:       sendRoutine - The complete routine for WSASend().
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  * This method display the statistics on the window.
  *
  * @param          error -  WSAerror
  * @param          bytesTransferred - Number of bytes received from WSARecv()
  * @param          overlapped - An overlapped structure, used internally by the
  *                     system.
  * @param          inFlags - Flags used to modify the behevior of WSARecv()
    --------------------------------------------------------------------------- */
void CALLBACK ClientWindow::sendRoutine(DWORD error, DWORD bytesTransferred, LPWSAOVERLAPPED overlapped, DWORD inFlags){
    mySelf->bytesSent_ += bytesTransferred;
    mySelf->totalBytesSent_ += bytesTransferred;
    mySelf->packetSent_++;
    mySelf->totalPacketSent_++;
    mySelf->lastPacketSize_ = bytesTransferred;
    mySelf->avgPacketSize_ = (double)mySelf->totalBytesSent_/mySelf->totalPacketSent_;

    mySelf->ui->bytesTransfered->setPlainText(QString::number(mySelf->bytesSent_, 10).append(" Bytes"));
    mySelf->ui->packetsTransfered->setPlainText(QString::number(mySelf->packetSent_, 10));
    mySelf->setStatusBar(QString("Sent ").append(QString::number(bytesTransferred, 10)), 1000);
    mySelf->ui->totalPacketsSent->setPlainText(QString::number((mySelf->totalPacketSent_), 10));
    mySelf->ui->totalBytesSent->setPlainText(QString::number((mySelf->totalBytesSent_), 10).append(" Bytes"));
    mySelf->ui->lastPacketSize->setPlainText(QString::number((mySelf->lastPacketSize_), 10));
    mySelf->ui->avgPacketSize->setPlainText(QString::number((mySelf->avgPacketSize_), 'f', 4));
}

/** ---------------------------------------------------------------------------
  * Function:       runClient - Notify user that client is running.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  * Set the server IP to a textfield in the window, and remind the user(in the
  * status bar) that the program is ready to send a file.
    --------------------------------------------------------------------------- */
void ClientWindow::runClient(){
    ui->serverIP->setPlainText(hostIPStr_);
    selectFile();

    setStatusBar("Client is running. Ready to for file transfer.");
}

/** ---------------------------------------------------------------------------
  * Function:       disconnectFromSrv - Disconnect the client.
  *
  * Date:           February 22, 2011
  *
  * Designer:       Paninee Khantidhara
  * Programmer:     Paninee Khantidhara
  *
  * Notes:
  * Close and clean the socket.
    --------------------------------------------------------------------------- */
void ClientWindow::disconnectFromSrv(){
    closesocket(clientSocket_);
    WSACleanup();
    setStatusBar("Disconnected", 10000);
    ui->selectFileBtn->setEnabled(true);
    ui->sendFileBtn->setEnabled(false);
}
