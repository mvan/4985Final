#ifndef AUDIOTRANSFERIN_H
#define AUDIOTRANSFERIN_H
#include <QThread>
#include <winsock2.h>
#include <QByteArray>
/**
  * Class: AudioWriteThread
  *
  * Pulls packets off incoming audio buffer, parses them, and plays
  * to speakers.
  */
class AudioWriteThread : public QThread{
    Q_OBJECT

public:
    AudioWriteThread();

protected:
    /**
      * Function: run
      *
      * Entry point of thread. Pulls packets off incoming audio buffer,
      * parses audio header, and uses methods from audioin to play sound to the speakers.
      */
    void run();

signals:
    void endStream();

};
#endif // AUDIOTRANSFERIN_H
