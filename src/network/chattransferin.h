#ifndef CHATTRANSFERIN_H
#define CHATTRANSFERIN_H
#include <QThread>


class ChatWriteThread : public QThread{
    Q_OBJECT


protected:
    void run();
};
#endif // CHATTRANSFERIN_H
