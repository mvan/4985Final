#ifndef SERVERLISTENTHREAD_H
#define SERVERLISTENTHREAD_H

#include <QThread>
#include <QMessageBox>

#include <winsock2.h>

class ServerListenThread : public QThread{
    Q_OBJECT

public:
    ServerListenThread(QObject *parent = 0);
    ~ServerListenThread();

    void run();

    SOCKET getServerSocket();
    void setServerSocket(SOCKET serverSocket);

signals:
    void errorOccurred(QString title, QString msg);
    void serverStopped();
    void clientConnected(SOCKET clientSocket);

private:
    SOCKET serverSocket_;
};

#endif // SERVERLISTENTHREAD_H
