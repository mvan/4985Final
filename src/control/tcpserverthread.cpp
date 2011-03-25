#include "tcpserverthread.h"

TCPServerThread::TCPServerThread(int port, QObject *parent) :
    QThread(parent), port_(port) {
    tcpServer_ = new tcpserver();
}

TCPServerThread::~TCPServerThread() {
}

void TCPServerThread::run() {
    tcpServer_->run(port_);
}

tcpserver* TCPServerThread::getTCPServer() {
    return tcpServer_;
}
