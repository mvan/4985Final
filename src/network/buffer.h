#ifndef BUFFER_H
#define BUFFER_H
#include <QQueue>
#include <QMutex>
#include "network.h"
#include <QWaitCondition>

#define DEFAULT_BUFFER_SIZE 10
#define DATA_SIZE 4093
class Buffer
{
public:
    Buffer();
    Buffer(int size);
    ~Buffer();
    int bufferSize;
    QQueue<char*> queue; //QQueue<QByteArray*>
    QMutex queueMutex;
    QWaitCondition bufferNotFull;
    QWaitCondition bufferNotEmpty;

    void bufferPacket(char* packet);
    char* grabPacket();
};

#endif // BUFFER_H
