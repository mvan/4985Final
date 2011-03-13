#include "udpserver.h"

void server::run() {
    rdSock_ = new socket();
    
    rdSock_->UDPSocket_Init();
    rdSock_->UDPSocket_Bind_Multicast();
    //begin reading.
    rdSock_->wait();
}