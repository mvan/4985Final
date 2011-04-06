/**
  * Class: AudioReadThread
  *
  * Description:
  * This class prepares the buffer for streaming. It reads
  * part of file and put it into the buffer.
  * Qstring passed into the constructor indicates file to be opened.
  */

/**
  * Class: AudioSendThread
  *
  * Description:
  * Sends audio packet.
  */
#ifndef AUDIOTRANSFER_H
#define AUDIOTRANSFER_H
#include <QThread>
#include <winsock2.h>
#define HDR_SIZE 44
#define AUDIO_DATA_SIZE (4092-HDR_SIZE)
class AudioSendThread;

class AudioReadThread : public QThread{
    Q_OBJECT

public:
    explicit AudioReadThread(QString file);
    virtual ~AudioReadThread();


    AudioSendThread* thread_;
protected:
    /**
      * Function: run()
      *
      * Description:
      * Entry point of the thread.
      * Spawns thread to send data to network.
      * Reads data from file and pushes onto outgoing audio buffer.
      */
    void run();

signals:
    void sendUDPPacket(char*);
    void endStream();

public slots:
    /**
      * Function: send
      *
      * @param char* - Packet to be sent over the network.
      *
      * Emits signal to send packet over the network
      */
    void send(char*);

private:
    QString file_;
};

class AudioSendThread : public QThread{
    Q_OBJECT

public:
    explicit AudioSendThread();
    virtual ~AudioSendThread(){}

signals:
    void sendPacket(char*);

protected:
    /**
      * Function: run
      *
      * Pulls packet off the outgoing audio buffer and sends them to the
      * network over the udp socket. Emits sendPacket. Connected to send
      * in FileReadThread.
      */
    void run();
};
#endif // AUDIOTRANSFER_H
