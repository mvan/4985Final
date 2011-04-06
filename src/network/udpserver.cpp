#include "udpserver.h"
#include "externs.h"

void udpserver::run() {
    
    this->rdSock_->UDPSocket_Init(portNo);
    this->rdSock_->UDPSocket_Bind_Multicast();
    this->rdSock_->createOLEvent();
    while(1) {
        if(rdSock_->UDPRecv_Multicast() == 0) {
            break;
        }
        ProcessUDPPacket(&(rdSock_->packet_[0]));
        rdSock_->clrPacket();
    }
}

void udpserver::ProcessUDPPacket(char* packet) {
    if(packet[0] == MSG_AUDIO || packet[0] == MSG_MIC) {
        if(audioinBuffer.queue.size() == audioinBuffer.bufferSize){
            audioinBuffer.queueMutex.lock();
            audioinBuffer.bufferNotFull.wait(&audioinBuffer.queueMutex);
            audioinBuffer.queueMutex.unlock();
        }
        audioinBuffer.bufferPacket(packet);
    }
}
