#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkInterface>
#include <QMessageBox>
#include <QFileDialog>

#include <winsock2.h>

#include "serverlistenthread.h"
#include "clientprocessthread.h"

namespace Ui {
    class ServerWindow;
}

class ServerWindow : public QMainWindow {
    Q_OBJECT

public slots:
    void displayError(QString title, QString msg);
    void cleanStoppedServer();
    void addClient(SOCKET clientSocket);
    void setStatusBar(QString text, int timeout = 0);
    void addPacket(unsigned int bytesReceived, int duration = 0);

public:
    explicit ServerWindow(QWidget *parent = 0);
    ~ServerWindow();

    QString getServerIP();
    void setServerIP(QList<QHostAddress> *serverIPList = 0);

    int getPort();
    void setPort(int port);

    int getProtocol();
    void setProtocol(int protocol);

    SOCKET getServerSocket();
    void setServerSocket(SOCKET serverSocket);

    void fillServerIP();
    void fillServerPort();
    void fillServerProtocol();

    void clearStatusBar();

signals:
    void selectedFile(QString fileName);

private slots:
    void runServer();
    void stopServer();
    void getSaveFileName(QEventLoop *eventLoop);

private:
    void runTCPServer();
    void runUDPServer();

    Ui::ServerWindow *ui;
    QList<QHostAddress> *serverIPList_;
    QList<ClientProcessThread *> *clientProcThreadList_;
    QString serverIPStr_;
    QString saveFileName_;
    int port_;
    int protocol_;
    unsigned int packetReceived_;
    unsigned int totalPacketReceived_;
    unsigned int bytesReceived_;
    unsigned int totalBytesReceived_;
    unsigned int lastPacketSize_;
    double avgPacketSize_;
    SOCKET serverSocket_;
    ServerListenThread listenThread_;
};

#endif // SERVERWINDOW_H
