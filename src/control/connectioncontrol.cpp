#include "connectioncontrol.h"

ConnectionControl::ConnectionControl() {
    tcpserv_ = new tcpserver();
    udpserv_ = new udpserver();
}

ConnectionControl::~ConnectionControl() {
    delete tcpserv_;
    delete udpserv_;
}

bool ConnectionControl::startServer(int tcpPort, int udpPort) {
    //TODO: put this in a thread.
    //tcpServer_->run();
    udpServer_->run(udpPort);
    return true;
}

bool ConnectionControl::connectToServer(QString tcpIp, int tcpPort, int udpPort) {
    //socket_.TCPSocket_Connect(ip.toAscii().data(), port); //<< if this fail return false to notify the gui
    memcpy(socket_.packet_, "test\0", 5);
    socket_.UDPSend_Multicast();
    //TODO: Duncan, we might need to be able to choose the port too.
    //Also, if you can return a boolean of success/failure, it would be nice.
    socket_.TCPSocket_Init();
    socket_.TCPSocket_Connect(ip.toAscii().data(), port); //<< if this fail return false to notify the gui
    return true;
}
