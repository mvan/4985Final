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
    socket_.UDPSocket_Init();
    socket_.createOLEvent();
    socket_.UDPSend_Multicast();
    //TODO: Duncan, if you can return a boolean of success/failure, it would be nice.
    socket_.TCPSocket_Init();
    socket_.TCPSocket_Connect(tcpIp.toAscii().data(), tcpPort); //<< if this fail return false to notify the gui
    return true;
}
