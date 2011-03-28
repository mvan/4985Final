#ifndef CHATTRANSFER_H
#define CHATTRANSFER_H
#include <QThread>
#include <winsock2.h>

class ChatSendThread : public QThread {
    Q_OBJECT

public:
    ChatSendThread();
signals:
    void sendChatPacket(char* packet);
protected:
    void run();
};
#endif // CHATTRANSFER_H
