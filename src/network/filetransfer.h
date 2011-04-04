#ifndef FILETRANSFER_H
#define FILETRANSFER_H
#include <QThread>
#include <winsock2.h>



class FileReadThread : public QThread{
    Q_OBJECT

public:
    FileReadThread(QString file, char reqNum);

signals:
    void sendTCPPacket(char*, char);
    void endFT();

public slots:
    void send(char*, char);

protected:
    void run();

private:
    QString file_;
    char reqNum_;
};

class FileSendThread : public QThread{
    Q_OBJECT

public:
    FileSendThread();

signals:
   void sendPacket(char*, char);
protected:
    void run();
};

#endif // FILETRANSFER_H
