#include "buffer.h"
#include <winsock2.h>


Buffer::Buffer(int size):bufferSize(size){}

Buffer::~Buffer(){}

void Buffer::bufferPacket(char* packet){
    char tempPacket[PACKETSIZE];
    memmove(tempPacket, packet, PACKETSIZE);
    QByteArray tmp(tempPacket, PACKETSIZE);
    this->queueMutex.lock();
    this->queue.enqueue(tmp);
    this->bufferNotEmpty.wakeAll();
    this->queueMutex.unlock();
}

void Buffer::grabPacket(char* buf){
    ZeroMemory(buf, PACKETSIZE);
    this->queueMutex.lock();
    if(!this->queue.empty()){
        memmove(buf, this->queue.dequeue().data(), PACKETSIZE);
        this->bufferNotFull.wakeAll();
    }
    this->queueMutex.unlock();
}
