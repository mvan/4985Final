#ifndef MICTHREAD_H
#define MICTHREAD_H
#include <QThread>
#include <QByteArray>
#include "audioout.h"

class MicThread : public QThread{
    Q_OBJECT

public:
    MicThread();

public slots:
    void send(char*);
protected:
    void run();


signals:
    void sendUDPPacket(char*);
    void endMic();

};

#endif // MICTHREAD_H
