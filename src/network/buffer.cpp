#include "buffer.h"
#include <winsock2.h>


Buffer::Buffer(int size):bufferSize(size)
{
}

Buffer::~Buffer(){}


void Buffer::bufferPacket(char* &packet){
    char* tempPacket;
    tempPacket = (char*)malloc(PACKETSIZE);
    memmove(tempPacket, packet, PACKETSIZE);
    this->queueMutex.lock();
    this->queue.enqueue(QByteArray::fromRawData(tempPacket, PACKETSIZE));
    this->bufferNotEmpty.wakeAll();
    this->queueMutex.unlock();
    //free(tempPacket);
}

void Buffer::grabPacket(char* buf){
    ZeroMemory(buf, PACKETSIZE);
    this->queueMutex.lock();
    memmove(buf, this->queue.dequeue().data(), PACKETSIZE);
    this->bufferNotFull.wakeAll();
    this->queueMutex.unlock();
}
