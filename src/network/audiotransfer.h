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
