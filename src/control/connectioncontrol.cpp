#include "connectioncontrol.h"

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

sock ConnectionControl::getTCPSocket() {
    return TCPSocket_;
}

sock ConnectionControl::getUDPSocket() {
    return UDPSocket_;
}
/*
 *these will be used later (maybe)
 */
void ConnectionControl::startFT() {

}

void ConnectionControl::endFT() {

}

void ConnectionControl::startStream() {

}

void ConnectionControl::endStream() {

}
