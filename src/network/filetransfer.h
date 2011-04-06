#ifndef FILETRANSFER_H
#define FILETRANSFER_H
#include <QThread>
#include <winsock2.h>
/**
  * Class: FileReadThread
  *
  * Thread that reads data from file and pushes onto the
  * outgoing file buffer. Qstring passed into constructor indicates
  * file to be opened.
  *
  */


class FileReadThread : public QThread{
    Q_OBJECT

public:
    FileReadThread(QString file, char reqNum);

signals:
    void sendTCPPacket(char*, char);
    void endFT();

public slots:
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: FileReadThread::send
    --
    -- DATE: April 5, 2011
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Daniel Wright
    --
    -- PROGRAMMER: Daniel Wright
    --
    -- INTERFACE: void FileReadThread::send(char* packet, char req)
    --              - packet - Packet data to be sent
    --              - req - ID of client to send to
    --
    -- RETURNS: void
    --
    -- NOTES:
    -- Emits Signal to send packet over the TCP connection.
    ----------------------------------------------------------------------------------------------------------------------*/
    void send(char*, char);

protected:
    /*------------------------------------------------------------------------------------------------------------------
    -- FUNCTION: run
    --
    -- DATE: April 5, 2011
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Daniel Wright
    --
    -- PROGRAMMER: Daniel Wright
    --
    -- INTERFACE: void FileReadThread::run()
    --
    -- RETURNS: void
    --
    -- NOTES:
    -- Spawns thread to send data to network.
    -- Reads data from file and pushes onto outgoing file buffer.
    ----------------------------------------------------------------------------------------------------------------------*/
    void run();

private:
    QString file_;
    char reqNum_;
};
/**
  * Class: FileSendThread
  *
  * Thread that pulls data off the outgoing file buffer
  * and sends it out over the network.
  *
  */
class FileSendThread : public QThread{
    Q_OBJECT

public:
    FileSendThread();

signals:
   void sendPacket(char*, char);

protected:
   /*------------------------------------------------------------------------------------------------------------------
   -- FUNCTION: run
   --
   -- DATE: April 5, 2011
   --
   -- REVISIONS: (Date and Description)
   --
   -- DESIGNER: Daniel Wright
   --
   -- PROGRAMMER: Daniel Wright
   --
   -- INTERFACE: void FileSendThread::run()
   --
   -- RETURNS: void
   --
   -- NOTES:
   -- Pulls data packets off the outgoing file buffer and sends them out over the TCP connection.
   -- Emits sendPacket, connected to FileReadThread::send
   ----------------------------------------------------------------------------------------------------------------------*/
    void run();
};

#endif // FILETRANSFER_H
