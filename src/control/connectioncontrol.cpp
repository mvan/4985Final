#include "connectioncontrol.h"
#include "../network/externs.h"
#include <winsock2.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QList>
bool transferringIn = false;
bool transferringOut = false;
bool streamingIn = false;
bool streamingOut = false;
char ClientNum = 0;
ConnectionControl::ConnectionControl(): numConnections_(0){

}

ConnectionControl::~ConnectionControl() {

}

bool ConnectionControl::startServer(int tcpPort, int udpPort) {
    udpServer_ = new udpserver(udpPort);
    udpServer_->start();
    tcpServer_ = new tcpserver(tcpPort);
    tcpServer_->start();

    //CONNECTION SIGNAL AND SLOT
    connect(tcpServer_, SIGNAL(connectionRequest(char*)), this,
            SLOT(connectionSlot(char*)), Qt::QueuedConnection);

    //FILE TRANSFER SIGNALS AND SLOTS
    connect(tcpServer_, SIGNAL(FTReq(char*, char)), this,
            SLOT(startFTFromReq(char*, char)), Qt::QueuedConnection);

    //LIST SIGNALS AND SLOTS
    connect(tcpServer_, SIGNAL(updateList(char*)), this,
            SLOT(updateList(char*)), Qt::QueuedConnection);

    //STREAM SIGNALS AND SLOTS
    connect(tcpServer_, SIGNAL(StreamReq(char*)), this,
            SLOT(startStreamFromReq(char*)), Qt::QueuedConnection);

    return true;
}

bool ConnectionControl::connectToServer(QString tcpIp, int tcpPort, int udpPort) {
    char connectionPacket[PACKETSIZE];
    tcpPort_ = tcpPort;
    UDPSocket_.UDPSocket_Init(udpPort);
    TCPSocket_.TCPSocket_Init();
    if(TCPSocket_.TCPSocket_Connect(tcpIp.toAscii().data(), tcpPort) == TRUE) {
        TCPSocket_.setLocalAddr();
        mkPacket(connectionPacket, MSG_CONN, PACKETSIZE, 0, TCPSocket_.getLocalAddr());
        TCPSocket_.setPacket(connectionPacket);
        TCPSocket_.TCPSend();
        TCPSocket_.clrPacket();
        return true;
    }
    TCPSocket_.socket_close();
    TCPSocket_.setSock(0);
    return false;
}

void ConnectionControl::connectionSlot(char* ipaddr) {
    QMessageBox notification;
    QString msg;
    char buf[PACKETSIZE];

    msg.append("Remote client connected at: ");
    msg.append(ipaddr);
    msg.append(", establishing connection to their server.");
    notification.setText(msg);
    connections_[numConnections_].TCPSocket_Init();
    connections_[numConnections_].TCPSocket_Connect(ipaddr, tcpPort_);
    connections_[numConnections_].clrPacket();
    mkPacket(buf, MSG_CONNBACK, PACKETSIZE, numConnections_, (char*)"");
    connections_[numConnections_].setPacket(buf);
    connections_[numConnections_].TCPSend();
    numConnections_++;
    notification.exec();
}

QString ConnectionControl::getFileName() {
    return QFileDialog::getSaveFileName(0, "Select a File");
}

void ConnectionControl::requestFT(char* fileName) {
    char packet[PACKETSIZE];
    QString fname;
    if(TCPSocket_.getSock() == 0) {
        QMessageBox m;
        m.setText(QString("You are not connected to a server."));
        m.exec();
        return;
    }

    mkPacket(packet, MSG_FTREQ, strlen(fileName), ClientNum, fileName);

    fname = getFileName();
    if(fname.size() == 0) {
        QMessageBox m;
        m.setText(QString("No save file selected, aborting transfer."));
        m.exec();
        return;
    }
    fileInThread_ = new FileWriteThread(fname);
    fileInThread_->start();

    connect(fileInThread_, SIGNAL(endFT()), this,
            SLOT(endFTIn()), Qt::QueuedConnection);

    TCPSocket_.clrPacket();
    TCPSocket_.setPacket(packet);
    TCPSocket_.TCPSend();
}

void ConnectionControl::startFTFromReq(char* fileName, char clientNo) {
    if(transferringOut == true) {
        return;
    }
    fileOutThread_ = new FileReadThread(QString(fileName), clientNo);
    //FILE TRANSFER SIGNALS AND SLOTS
    connect(fileOutThread_, SIGNAL(sendTCPPacket(char*, char)), this,
            SLOT(sendFilePacket(char*, char)), Qt::QueuedConnection);
    connect(fileOutThread_, SIGNAL(endFT()), this,
            SLOT(endFTOut()), Qt::QueuedConnection);
    transferringOut = true;
    fileOutThread_->start();
}

void ConnectionControl::endFTOut() {
    Sleep(100);
    disconnect(fileOutThread_, SIGNAL(sendTCPPacket(char*, char)), this,
            SLOT(sendFilePacket(char*, char)));
    disconnect(fileOutThread_, SIGNAL(endFT()), this,
            SLOT(endFTOut()));
    transferringOut = false;
    delete fileOutThread_;
}

void ConnectionControl::endFTIn() {
    Sleep(100);
    delete fileInThread_;
}

void ConnectionControl::startStreamFromReq(char* fName) {
   audioOutThread_ = new AudioReadThread(QString(fName));
   connect(audioOutThread_, SIGNAL(sendUDPPacket(char*)), this,
           SLOT(sendAudioPacket(char*)), Qt::QueuedConnection);
   connect(audioOutThread_, SIGNAL(endStream()), this,
           SLOT(endStreamOut()), Qt::QueuedConnection);
   audioOutThread_->start();
}

void ConnectionControl::requestStream(char* fileName) {
    char packet[PACKETSIZE];
    if(TCPSocket_.getSock() == 0) {
        QMessageBox m;
        m.setText(QString("You are not connected to a server."));
        m.exec();
        return;
    }

    mkPacket(packet, MSG_STREAMREQ, strlen(fileName), ClientNum, fileName);

    audioInThread_ = new AudioWriteThread();
    audioInThread_->start();

    connect(audioInThread_, SIGNAL(endStream()), this,
            SLOT(endStreamIn()), Qt::QueuedConnection);

    TCPSocket_.clrPacket();
    TCPSocket_.setPacket(packet);
    TCPSocket_.TCPSend();

}

void ConnectionControl::endStreamOut() {
    Sleep(100);
    delete audioOutThread_;
}

void ConnectionControl::endStreamIn() {
    Sleep(100);
    delete audioInThread_;
}

void ConnectionControl::sendFilePacket(char* packet, char req) {
    if(TCPSocket_.getSock() != 0) {
        TCPSocket_.clrPacket();
        TCPSocket_.setPacket(packet);
        TCPSocket_.TCPSend();
    }
    connections_[(int)req].clrPacket();
    connections_[(int)req].setPacket(packet);
    connections_[(int)req].TCPSend();
}

void ConnectionControl::sendAudioPacket(char* packet){
    UDPSocket_.clrPacket();
    UDPSocket_.setPacket(packet);
    UDPSocket_.UDPSend_Multicast();
}

void ConnectionControl::sendChatPacket(char* packet) {
    if(TCPSocket_.getSock() != 0) {
        TCPSocket_.clrPacket();
        TCPSocket_.setPacket(packet);
        TCPSocket_.TCPSend();
    }
    for(int i = 0; i < numConnections_; ++i) {
        connections_[i].clrPacket();
        connections_[i].setPacket(packet);
        connections_[i].TCPSend();
    }
}

void ConnectionControl::updateList(char* fname) {
    emit listUpdate(fname);
}

void ConnectionControl::addAudioFile(QString filename) {

    char buf[PACKETSIZE];

    mkPacket(buf, MSG_LIST, filename.size(),
                        0, filename.toAscii().data());
    if(TCPSocket_.getSock() != 0) {
        TCPSocket_.clrPacket();
        TCPSocket_.setPacket(buf);
        TCPSocket_.TCPSend();
    }
    for(int i = 0; i < numConnections_; ++i) {
        connections_[i].clrPacket();
        connections_[i].setPacket(buf);
        connections_[i].TCPSend();
    }
}
