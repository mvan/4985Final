#ifndef BUFFER_H
#define BUFFER_H
#include <QQueue>
#include <QMutex>
#include <QObject>
#include "network.h"
#include <QWaitCondition>
#include <QByteArray>

#define DEFAULT_BUFFER_SIZE 10
#define DATA_SIZE 4092

class Buffer:public QObject
{
    Q_OBJECT
public:
    explicit Buffer(int size = DEFAULT_BUFFER_SIZE);
    virtual ~Buffer();
    int bufferSize;
    QQueue<QByteArray> queue; //QQueue<QByteArray*>
    QMutex queueMutex;
    QWaitCondition bufferNotFull;
    QWaitCondition bufferNotEmpty;

    void bufferPacket(char* packet);
    void grabPacket(char* buf);
};

#endif // BUFFER_H
