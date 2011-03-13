#include "udpserver.h"

void udpserver::run() {
    this->rdSock_ = new sock();
    
    this->rdSock_->UDPSocket_Init();
    this->rdSock_->UDPSocket_Bind_Multicast();
    //begin reading.
    this->rdSock_->wait();
}
