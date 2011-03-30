#include "tcpserver.h"
#include "socket.h"
void tcpserver::run(int portNo) {

    TIMEVAL tv;

    listenSock_->TCPSocket_Init();
    listenSock_->TCPSocket_Bind(portNo);
    listenSock_->TCPSocket_Listen();
    initSelect();

    tv.tv_sec = INFINITE;
    tv.tv_usec = 0;

    while(1) {

        FD_ZERO(&readySet_);
        this->readySet_ = this->allSet_;

        if((numReady_ = select(NULL, &readySet_, NULL, NULL, NULL)) == SOCKET_ERROR) {
            int balls = WSAGetLastError();
            continue;
        }

        if(FD_ISSET(listenSock_->getSock(), &readySet_)) {
            if(addSelectSock() <= 0) {
                continue;
            }
        }

        for(int i = 0; i < FD_SETSIZE; ++i) {
            int nRead = 0;
            SOCKET s;
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
    socks_[0] = *listenSock_;

    for(int i = 0; i < FD_SETSIZE; ++i) {
        selectSocks_[i] = -1;
    }

    FD_ZERO(&allSet_);
    FD_SET(listenSock_->getSock(), &allSet_);

}

int tcpserver::addSelectSock() {

    int i;
    SOCKET s = listenSock_->TCPSocket_Accept();
    sock so(s);

    if(s == 0) {
        return 0;
    }

    currentClients_.push_back(so);

    for(i = 0; i < FD_SETSIZE; ++i) {
        if(selectSocks_[i] < 0) {
            socks_[i] = so;
            selectSocks_[i] = s;
            break;
        }
    }
    FD_SET(s, &allSet_);
    return --numReady_;

}

QList<sock> tcpserver::getAllClients() {
    return currentClients_;
}

