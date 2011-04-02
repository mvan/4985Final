#include "connectioncontrol.h"
#include <winsock2.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QList>
ConnectionControl::ConnectionControl() {

}

ConnectionControl::~ConnectionControl() {

}

bool ConnectionControl::startServer(int tcpPort, int udpPort) {
    udpServer_ = new udpserver(udpPort);
    udpServer_->start();
    tcpServer_ = new tcpserver(tcpPort);
    tcpServer_->start();
    connect(tcpServer_, SIGNAL(connectionRequest(char*)), this,
            SLOT(connectionSlot(char*)), Qt::QueuedConnection);
    return true;
}

bool ConnectionControl::connectToServer(QString tcpIp, int tcpPort) {
    char connectionPacket[PACKETSIZE];
    tcpPort_ = tcpPort;
    UDPSocket_.UDPSocket_Init();
    TCPSocket_.TCPSocket_Init();
    if(TCPSocket_.TCPSocket_Connect(tcpIp.toAscii().data(), tcpPort) == TRUE) {
        TCPSocket_.setLocalAddr();
        mkPacket(connectionPacket, MSG_CONN, PACKETSIZE, 0, TCPSocket_.getLocalAddr());
        TCPSocket_.setPacket(connectionPacket);
        TCPSocket_.TCPSend();
        TCPSocket_.clrPacket();
        return true;
    }
    return false;
}

void ConnectionControl::connectionSlot(char* ipaddr) {
    QMessageBox notification;
    QString msg;
    msg.append("Remote client connected at: ");
    msg.append(ipaddr);
    msg.append(", establishing connection to their server.");
    notification.setText(msg);
    TCPSocket_.TCPSocket_Connect(ipaddr, tcpPort_);
    notification.exec();
}

tcpserver* ConnectionControl::getTCPServer() {
    return tcpServer_;
}
udpserver* ConnectionControl::getUDPServer() {
    return udpServer_;
}
sock ConnectionControl::getTCPSocket() {
    return TCPSocket_;
}

sock ConnectionControl::getUDPSocket() {
    return UDPSocket_;
}
QString ConnectionControl::getFileName() {
    return QFileDialog::getOpenFileName(0, "Select a File");
}

/*
 *these will be used later (maybe)
 */
void ConnectionControl::incomingFT() {
    //fileInThread_ = new FileWriteThread(h);
    connect(fileInThread_, SIGNAL(endFT()), this, SLOT(endFTIn()));
}

void ConnectionControl::startFT() {
    fileOutThread_ = new FileReadThread(getFileName());
    connect(fileOutThread_, SIGNAL(sendTCPPacket(char*)), this,
            SLOT(sendFilePacket(char*)), Qt::QueuedConnection);
    connect(fileOutThread_, SIGNAL(endFT()), this,
            SLOT(endFTOut()), Qt::QueuedConnection);
    fileOutThread_->start();
}

void ConnectionControl::endFTOut() {
    disconnect(fileOutThread_, SIGNAL(sendTCPPacket(char*)), this,
            SLOT(sendFilePacket(char*)));
    disconnect(fileOutThread_, SIGNAL(endFT()), this,
            SLOT(endFTOut()));
    delete fileOutThread_;
}

void ConnectionControl::endFTIn() {
    delete fileInThread_;
}

void ConnectionControl::incomingStream() {
    audioInThread_ = new AudioWriteThread(QByteArray());
    connect(audioInThread_, SIGNAL(endStream()), this, SLOT(endStreamIn()));
}

void ConnectionControl::startStream() {
    //audioOutThread_ = new AudioReadThread(h);
   // connect(audioOutThread_, SIGNAL(endStream()), this, SLOT(endStreamOut()));
}

void ConnectionControl::endStreamOut() {
    delete audioOutThread_;
}

void ConnectionControl::endStreamIn() {
    delete audioInThread_;
}

void ConnectionControl::sendFilePacket(char* packet) {
    TCPSocket_.clrPacket();
    TCPSocket_.setPacket(packet);
    TCPSocket_.TCPSend();
    TCPSocket_.clrPacket();
}
