#include "udpserver.h"

void udpserver::run() {
    this->rdSock_ = new sock();
    
    this->rdSock_->UDPSocket_Init();
    this->rdSock_->UDPSocket_Bind_Multicast();
    while(1) {
        if(this->rdSock_->UDPRecv_Multicast() == 0) {
            break;
        }
    }
}
