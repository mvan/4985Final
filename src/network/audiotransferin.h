#ifndef AUDIOTRANSFERIN_H
#define AUDIOTRANSFERIN_H
#include <QThread>
#include <winsock2.h>
#include <QByteArray>

class AudioWriteThread : public QThread{
    Q_OBJECT

public:
    AudioWriteThread();

protected:
    void run();

signals:
    void endStream();

};
#endif // AUDIOTRANSFERIN_H
