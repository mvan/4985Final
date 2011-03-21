#include "udpserverthread.h"

UDPServerThread::UDPServerThread(int port, QObject *parent) :
    QThread(parent), port_(port) {
    udpServer_ = new udpserver();
}

UDPServerThread::~UDPServerThread() {
}

void UDPServerThread::run() {
    udpServer_->run(port_);
}
