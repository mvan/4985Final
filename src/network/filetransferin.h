#ifndef FILETRANSFERIN_H
#define FILETRANSFERIN_H
#include <QThread>
#include <QString>
#include <winsock2.h>

/**
  * Class: FileWriteThread
  *
  * Pulls packets off the fileinBuffer and writes them to file.
  * Qstring passed in constructor indicates file to write to.
  */
class FileWriteThread : public QThread{
    Q_OBJECT

public:
    FileWriteThread(QString file);

protected:
    /**
      * Function: run
      *
      * Entry point of thread. Pulls packets off the fileinBuffer and
      * writes the data portion to file.
      */
    void run();
signals:
    void endFT();
    /* Displays a message box with the given QString as text*/
    void displayMB(QString);
private:
    QString file_;
};

#endif // FILETRANSFERIN_H
