#ifndef MICTHREAD_H
#define MICTHREAD_H
#include <QThread>
#include <QByteArray>
#include "audioout.h"

class MicThread : public QThread{
    Q_OBJECT

public:
    MicThread();

protected:
    void run();

signals:
    //void endStream();

};

#endif // MICTHREAD_H
