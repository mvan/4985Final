#include "udpserver.h"

void udpserver::run() {
    this->rdSock_ = new sock();
    
    this->rdSock_->UDPSocket_Init();
    this->rdSock_->UDPSocket_Bind_Multicast();
    this->rdSock_->UDPSend_Multicast(this->rdSock_->packet_);
    this->rdSock_->wait();
}
