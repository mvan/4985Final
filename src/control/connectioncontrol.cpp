#include "connectioncontrol.h"

ConnectionControl::ConnectionControl() {
    tcpserv_ = new tcpserver();
    udpserv_ = new udpserver();
}
ConnectionControl::~ConnectionControl() {
    delete tcpserv_;
    delete udpserv_;
}
void ConnectionControl::startUDPServer(int port) {
    udpserv_->run(port);
}
void ConnectionControl::startTCPServer(int port) {
    tcpserv_->run(port);
}
bool ConnectionControl::connectToServer(QString ip, int port) {
    //TODO: Duncan, we might need to be able to choose the port too.
    //Also, if you can return a boolean of success/failure, it would be nice.
    socket_.TCPSocket_Init();
    socket_.TCPSocket_Connect(ip.toAscii().data(), port); //<< if this fail return false to notify the gui
    return true;
}
