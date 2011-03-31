#include "buffer.h"
#include <winsock2.h>

Buffer::Buffer():bufferSize(DEFAULT_BUFFER_SIZE)
{
}

Buffer::Buffer(int size):bufferSize(size)
{
}

Buffer::~Buffer(){}

void Buffer::bufferPacket(char* packet){
    this->queueMutex.lock();
    this->queue.enqueue(QByteArray::fromRawData(packet, PACKETSIZE));
    this->bufferNotEmpty.wakeAll();
    this->queueMutex.unlock();
}

void Buffer::grabPacket(char* buf){
    ZeroMemory(buf, PACKETSIZE);
    this->queueMutex.lock();
    memcpy(buf, this->queue.dequeue().data(), PACKETSIZE);
    this->bufferNotFull.wakeAll();
    this->queueMutex.unlock();
}
