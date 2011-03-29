#include "buffer.h"
#include<winsock2.h>

void Buffer::bufferPacket(char* packet){
    this->queueMutex.lock();
    this->queue.enqueue(QByteArray::fromRawData(packet, PACKETSIZE));
    this->queueMutex.unlock();
}

char* Buffer::grabPacket(){
    char* temp = (char*)malloc(PACKETSIZE);
    this->queueMutex.lock();
    strcpy(temp, this->queue.dequeue().data());
    this->bufferNotFull.wakeAll();
    this->queueMutex.unlock();
    return temp;
}
