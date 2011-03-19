#include "connectioncontrol.h"

ConnectionControl::ConnectionControl():
    tcpServer_(new tcpserver()), udpServer_(new udpserver) {
}

ConnectionControl::~ConnectionControl() {
}

bool ConnectionControl::startServer(int tcpPort, int udpPort) {
    //TODO: this gives segfault.
    //tcpServer_->run();
    udpServer_->run(udpPort);
    return true;
}

bool ConnectionControl::connectToServer(QString tcpIp, int tcpPort, int udpPort) {
    //socket_.TCPSocket_Connect(ip.toAscii().data(), port); //<< if this fail return false to notify the gui
    memcpy(socket_.packet_, "test\0", 5);
    socket_.UDPSend_Multicast();
    return true;
}
