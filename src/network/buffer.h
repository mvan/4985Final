#ifndef BUFFER_H
#define BUFFER_H
#include <QQueue>
#include <QMutex>
#include <QObject>
#include "network.h"
#include <QWaitCondition>
#include <QByteArray>

#define DEFAULT_BUFFER_SIZE 10
#define DATA_SIZE 4095
class Buffer:public QObject
{
    Q_OBJECT
public:
    explicit Buffer(int size = DEFAULT_BUFFER_SIZE):bufferSize(size){}
    virtual ~Buffer(){}
    int bufferSize;
    QQueue<QByteArray> queue; //QQueue<QByteArray*>
    QMutex queueMutex;
    QWaitCondition bufferNotFull;
    QWaitCondition bufferNotEmpty;

    void bufferPacket(char* packet);
    char* grabPacket();
};

#endif // BUFFER_H
