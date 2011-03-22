#ifndef CLIENTPROCESSTHREAD_H
#define CLIENTPROCESSTHREAD_H

#include <QThread>
#include <QEventLoop>
#include <QTime>

#include <winsock2.h>
#include <stdio.h>

class ClientProcessThread : public QThread{
    Q_OBJECT
public:
    explicit ClientProcessThread(SOCKET clientSocket, QObject *parent = 0);
    ~ClientProcessThread();

    void run();

signals:
    void errorOccurred(QString title, QString msg);
    void serverStopped();
    void receivedData(QEventLoop *eventLoop);
    void receivedMoreData(unsigned int bytesReceived, int duration = 0);
    void changeStatus(QString);

public slots:

private slots:
    bool openFile(QString filename);
    void getFileName(QString filename);

private:
    static void CALLBACK receivedRoutine(DWORD error, DWORD bytesTransferred, LPWSAOVERLAPPED overlapped, DWORD inFlags);

    SOCKET clientSocket_;
    OVERLAPPED overlapped_;
    QString fileName_;
    char *buff_;
    LPWSABUF data_;
    unsigned int buffsize_;
    DWORD bytesReceived_;
    DWORD flags_;
    FILE *file_;
    struct sockaddr_in clientSockaddr_;
    QTime *time_;

};

#endif // CLIENTPROCESSTHREAD_H
