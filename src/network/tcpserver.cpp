#include "tcpserver.h"
#include "socket.h"
void tcpserver::run() {
    listenSock_->TCPSocket_Init();
    listenSock_->TCPSocket_Bind();
    listenSock_->TCPSocket_Listen();
    while(1) {
        readySet_ = allSet_;
        numReady_ = select(selectSocks_[numSocks_-1]+1, &readySet_, NULL, NULL, NULL);
        if(FD_ISSET(&listenSock_, &readySet_)) {

            int i;
            sock s = listenSock_->TCPSocket_Accept();

            for(i = 0; i < FD_SETSIZE; ++i) {
                if(selectSocks_[i] < 0) {
                    socks_[i] = s;
                    selectSocks_[i] = s.getSock();
                    break;
                }
            }
            FD_SET(s.getSock(), &allSet_);
            if(s.getSock() > maxSock_) {
                maxSock_ = s.getSock();
            }
            if(i > sockIndex_) {
                sockIndex_ = i;
            }
            if(--numReady_ <= 0) {
                continue;
            }
        }
        for(int i = 0; i < FD_SETSIZE; ++i) {
            int s;
            sock so;

            so = socks_[i];
            if((s = selectSocks_[i]) < 0) {
                continue;
            }
            if(FD_ISSET(s, &readySet_)) {
                //do some reads and shit.
            }
        }
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
