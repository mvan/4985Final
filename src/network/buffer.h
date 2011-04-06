#ifndef BUFFER_H
#define BUFFER_H
#include <QQueue>
#include <QMutex>
#include <QObject>
#include "network.h"
#include <QWaitCondition>
#include <QByteArray>
/**
  * Default limit for number of packets allowed in buffer
  */
#define DEFAULT_BUFFER_SIZE 1000000
#define DATA_SIZE 4092

/**
  * Class: Buffer
  *
  * Implementation of a buffer for our audio player.
  */
class Buffer:public QObject
{
    Q_OBJECT
public:
    explicit Buffer(int size = DEFAULT_BUFFER_SIZE);
    virtual ~Buffer();

    /**
      * Number of packets allowed in buffer. Initialized
      * to DEFAULT_BUFFER_SIZE
      */
    int bufferSize;

    /**
      * QQueue contains the packets in the buffer
      */
    QQueue<QByteArray> queue;

    /**
      * Mutex used for protecting reads/writes on the buffer.
      */
    QMutex queueMutex;

    /**
      * Wait condition preventing packets from being buffered when QQueue is full.
      * Wakes when a packet is removed from the queue.
      */
    QWaitCondition bufferNotFull;

    /**
      * Wait condition preventing packets from being taken from the queue
      * when no packets have been queued.
      * Wakes when a packet is pushed onto the buffer.
      */
    QWaitCondition bufferNotEmpty;

    /**
      * Function: bufferPacket
      *
      * @param packet - packet to be pushed onto the buffer.
      *
      * Buffers one packet by appending it to the queue.
      * Wakes any threads waiting on bufferNotEmpty.
      */
    void bufferPacket(char* packet);

    /**
      * Function: grabPacket
      *
      * @param buf - Buffer to write packet into
      *
      * Removes the packet at the head of the queue and copies it into buf.
      * Wakes any threads waiting on bufferNotFull.
      */
    void grabPacket(char* buf);
};

#endif // BUFFER_H
