#include "tcpserver.h"
#include "socket.h"
void tcpserver::run(int portNo) {
    listenSock_->TCPSocket_Init();
    listenSock_->TCPSocket_Bind(portNo);
    listenSock_->TCPSocket_Listen();
    initSelect();

    while(1) {

        readySet_ = allSet_;

        numReady_ = select(NULL, &readySet_, NULL, NULL, NULL);

        if(FD_ISSET(listenSock_->getSock(), &readySet_)) {
            if(addSelectSock() <= 0) {
                continue;
            }
        }
        for(int i = 0; i < FD_SETSIZE; ++i) {
            int s, nRead = 0;
            sock so;

            so = socks_[i];
            if((s = selectSocks_[i]) < 0) {
                continue;
            }
            if(FD_ISSET(s, &readySet_)) {
                nRead = socks_[i].TCPRecv();
                if(nRead > 0) {
                    ProcessTCPPacket(socks_[i].packet_);
                }
            }
        }
    }
}

void tcpserver::initSelect() {

    socks_[numSocks_] = *listenSock_;

    memset(&selectSocks_, -1, FD_SETSIZE);

    FD_ZERO(&allSet_);
    FD_SET(listenSock_->getSock(), &allSet_);

}

int tcpserver::addSelectSock() {

    int i;
    sock s = listenSock_->TCPSocket_Accept();

    if(s.getSock() == 0) {
        return 0;
    }

    currentClients_.push_back(s);

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
    return --numReady_;

}

QList<sock> tcpserver::getAllClients() {
    return currentClients_;
}

