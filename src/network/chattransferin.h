#ifndef CHATTRANSFERIN_H
#define CHATTRANSFERIN_H
#include <QThread>

/**
  * Class: ChatWriteThread
  *
  * Adds chat packets to the display window.
  */
class ChatWriteThread : public QThread{
    Q_OBJECT

signals:
    void addChatToDisplay(char* packet);
protected:
    /**
      * Function: run
      *
      * Entry point of thread. Waits until there is a packet on the chatinBuffer,
      * dequeues it, and displays the data to the chat window.
      */
    void run();
};
#endif // CHATTRANSFERIN_H
