#ifndef AUDIOTRANSFERIN_H
#define AUDIOTRANSFERIN_H
#include <QThread>
#include <winsock2.h>
#include <QByteArray>

class AudioWriteThread : public QThread{
    Q_OBJECT

public:
    AudioWriteThread(QByteArray array);

protected:
    void run();

signals:
    void endStream();

private:
    QByteArray audioArray_;
};
#endif // AUDIOTRANSFERIN_H
