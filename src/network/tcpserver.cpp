/*#include "tcpserver.h"
void tcpserver::run() {
    listenSock_->TCPSocket_Init();
    listenSock_->TCPSocket_Bind();
    listenSock_->TCPSocket_Listen();
    while(1) {
        socks_[numSocks_++] = (listenSock_->TCPSocket_Accept());
    }
}

void tcpserver::initSelect() {
    selectSocks_[numSocks_] = listenSock_->getSock();
    socks_[numSocks_++] = *listenSock_;

    memset(&selectSocks_, -1, FD_SETSIZE);

    FD_ZERO(&allSet_);
    FD_SET(listenSock_->getSock(), &allSet_);
}

void tcpserver::addSelectSock(sock* sock) {

}
*/
