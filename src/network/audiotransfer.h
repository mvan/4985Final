#ifndef AUDIOTRANSFER_H
#define AUDIOTRANSFER_H
#include <QThread>
#include <winsock2.h>



class AudioReadThread : public QThread{
    Q_OBJECT

public:
    AudioReadThread(HANDLE handle);

protected:
    void run();

signals:
    void endStream();

private:
    HANDLE file_;
};

class AudioSendThread : public QThread{
    Q_OBJECT

public:
    AudioSendThread();

protected:
    void run();
};
#endif // AUDIOTRANSFER_H
