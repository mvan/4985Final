#include "udpserver.h"

void udpserver::run() {
    
    this->rdSock_->UDPSocket_Init(portNo);
    this->rdSock_->UDPSocket_Bind_Multicast();
    this->rdSock_->createOLEvent();
    while(1) {
        if(rdSock_->UDPRecv_Multicast() == 0) {
            break;
        }
        rdSock_->clrPacket();
    }
}
