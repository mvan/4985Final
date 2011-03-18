#include "connectioncontrol.h"

ConnectionControl::ConnectionControl():
    tcpServer_(new tcpserver()), udpServer_(new udpserver) {
}

ConnectionControl::~ConnectionControl() {
}

bool ConnectionControl::startServer(int tcpPort, int udpPort) {
    //TODO: this gives segfault.
    //tcpServer_->run();
    udpServer_->run();
    return true;
}

bool ConnectionControl::connectToServer(QString ip, int port) {
    socket_.TCPSocket_Connect(ip.toAscii().data()); //<< if this fail return false to notify the gui
    return true;
}
