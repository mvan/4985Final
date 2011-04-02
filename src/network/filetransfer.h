#ifndef FILETRANSFER_H
#define FILETRANSFER_H
#include <QThread>
#include <winsock2.h>



class FileReadThread : public QThread{
    Q_OBJECT

public:
    FileReadThread(QString file);

signals:
    void sendTCPPacket(char*);

public slots:
    void send(char*);

signals:
    void endFT();

protected:
    void run();

private:
    QString file_;
};

class FileSendThread : public QThread{
    Q_OBJECT

public:
    FileSendThread();

signals:
   void sendPacket(char*);
protected:
    void run();
};

#endif // FILETRANSFER_H
