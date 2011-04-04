#ifndef AUDIOTRANSFER_H
#define AUDIOTRANSFER_H
#include <QThread>
#include <winsock2.h>



class AudioReadThread : public QThread{
    Q_OBJECT

public:
    AudioReadThread(QString file);

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
    AudioSendThread();

signals:
    void sendPacket(char*);

protected:
    void run();
};
#endif // AUDIOTRANSFER_H
