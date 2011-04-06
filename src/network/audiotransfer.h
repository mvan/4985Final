/**
  * Class: AudioReadThread
  *
  * Description:
  * This class prepares the buffer for streaming. It reads
  * part of file and put it into the buffer.
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


class AudioReadThread : public QThread{
    Q_OBJECT

public:
    explicit AudioReadThread(QString file);
    virtual ~AudioReadThread(){};


protected:
    /**
      * Function: run()
      *
      * Description:
      * Entry point of the thread.
      */
    void run();

signals:
    void sendUDPPacket(char*);
    void endStream();

public slots:
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
    void run();
};
#endif // AUDIOTRANSFER_H
