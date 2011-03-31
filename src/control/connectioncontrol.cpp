#include "connectioncontrol.h"
#include <winsock2.h>
#include <QFileDialog>
#include <QStringList>
ConnectionControl::ConnectionControl() {
}

ConnectionControl::~ConnectionControl() {
}

bool ConnectionControl::startServer(int tcpPort, int udpPort) {
    udpServerThread_ = new UDPServerThread(udpPort);
    udpServerThread_->start();
    tcpServerThread_ = new TCPServerThread(tcpPort);
    tcpServerThread_->start();
    return true;
}

bool ConnectionControl::connectToServer(QString tcpIp, int tcpPort, int udpPort) {
    UDPSocket_.UDPSocket_Init();
    TCPSocket_.TCPSocket_Init();
    TCPSocket_.TCPSocket_Connect(tcpIp.toAscii().data(), tcpPort);
    return true;
}

TCPServerThread* ConnectionControl::getTCPServerThread() {
    return tcpServerThread_;
}

UDPServerThread* ConnectionControl::getUDPServerThread() {
    return udpServerThread_;
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
    HANDLE h;
    fileInThread_ = new FileWriteThread(h);
    connect(fileInThread_, SIGNAL(endFT()), this, SLOT(endFTIn()));
}

void ConnectionControl::startFT(QString fName) {
    HANDLE h;
    fileOutThread_ = new FileReadThread(h);
    connect(fileOutThread_, SIGNAL(endFT()), this, SLOT(endFTOut()));
}

void ConnectionControl::endFTOut() {
    delete fileOutThread_;
}

void ConnectionControl::endFTIn() {
    delete fileInThread_;
}

void ConnectionControl::incomingStream() {

}

void ConnectionControl::startStream() {

}

void ConnectionControl::endStream() {

}
