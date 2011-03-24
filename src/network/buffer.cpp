#include "buffer.h"

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
    this->queueMutex.unlock();
}

char* Buffer::grabPacket(){
    char* temp;
    this->queueMutex.lock();
    temp = this->queue.dequeue().data();
    this->bufferNotFull.wakeAll();
    this->queueMutex.unlock();
    return temp;
}
