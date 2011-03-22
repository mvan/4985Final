#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QActionGroup>
#include <QTime>
#include <string>

#include <winsock2.h>

#include "connectdlg.h"

namespace Ui {
    class ClientWindow;
}

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void displayError(QString title, QString msg);

public:
    explicit ClientWindow(QWidget *parent = 0);
    ~ClientWindow();

    QString getHostIPStr();
    void setHostIP(QString hostIP);

    int getPort();
    void setPort(int port);

    int getProtocol();
    void setProtocol(int protocol);

    SOCKET getClientSocket();
    void setClientSocket(SOCKET clientSocket);

    struct sockaddr_in getServerSockaddr();
    void setServerSockaddr(struct sockaddr_in sockaddr);

    unsigned int getPacketSize();
    void setPacketSize(unsigned int packetSize);

    unsigned int getTimesToSend();
    void setTimesToSend(unsigned int packetSize);

    void clearStatusBar();
    void setStatusBar(QString text, int timeout = 0);

private slots:
    bool selectFile();
    bool sendFile();
    void runClient();
    void disconnectFromSrv();
    void resetWindow();

private:
    bool loadFile(QString fileName);
    void setFileSize(int fileSize);
    int getFileSize();
    static void CALLBACK sendRoutine(DWORD error, DWORD bytesTransferred, LPWSAOVERLAPPED overlapped, DWORD inFlags);

    Ui::ClientWindow *ui;
    QString hostIPStr_;
    std::string filenameStr_;
    FILE *file_;
    int port_;
    int protocol_;
    unsigned int fileSize_;
    unsigned int totalBytesSent_;
    unsigned int packetSent_;
    unsigned int packetSize_;
    unsigned int timesToSend_;
    unsigned int totalPacketSent_;
    unsigned int lastPacketSize_;
    double avgPacketSize_;
    DWORD bytesSent_;
    DWORD flags_;
    WSAOVERLAPPED overlapped_;
    SOCKET clientSocket_;
    struct sockaddr_in serverSockaddr_;
    QActionGroup *packetSizeGroup_;
    QActionGroup *timesToSendGroup_;
};

#endif // CLIENTWINDOW_H
