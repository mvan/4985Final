#include "filetransfer.h"
#include "network.h"
#include "buffer.h"
#include <stdio.h>
#include "errors.h"
#include <QFile>

Buffer fileoutBuffer;

FileReadThread::FileReadThread(QString file, char reqNum):file_(file), reqNum_(reqNum){}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: FileReadThread::run
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
void FileReadThread::run(){

    int sizeOfFile = 0;
    int bytesRead = 0;
    int totalRead = 0;
    FileSendThread *thread;
    char tempPacket[PACKETSIZE], tempBuf[DATA_SIZE], endPack[PACKETSIZE];

    QFile file(file_);

    if(!file.open(QIODevice::ReadOnly)){
        WSAError(FILE_ERROR);
    }

    sizeOfFile = file.size();

    thread = new FileSendThread();
    connect(thread, SIGNAL(sendPacket(char*, char)), this, SLOT(send(char*, char)), Qt::QueuedConnection);
    thread->start();

    while(totalRead <= sizeOfFile){

        ZeroMemory(tempPacket, PACKETSIZE);
        ZeroMemory(tempBuf, DATA_SIZE);

        if((sizeOfFile - totalRead) >= DATA_SIZE){ //More than a packet left
            if((bytesRead = file.read(tempBuf, DATA_SIZE)) == -1){
                WSAError(READ_ERROR);
            }
            mkPacket(tempPacket, MSG_FT, (unsigned short) bytesRead, reqNum_, tempBuf); //change with src and dest, msg type
            if(fileoutBuffer.queue.size() == fileoutBuffer.bufferSize){
                fileoutBuffer.queueMutex.lock();
                fileoutBuffer.bufferNotFull.wait(&fileoutBuffer.queueMutex);
                fileoutBuffer.queueMutex.unlock();
            }
            fileoutBuffer.bufferPacket(tempPacket);
            totalRead += bytesRead;
        } else if(sizeOfFile == totalRead) { // finished exactly
            memset(tempBuf, 0, sizeof(tempBuf));
            mkPacket(tempPacket, MSG_FTCOMPLETE, 0, reqNum_,tempBuf);
            if(fileoutBuffer.queue.size() == fileoutBuffer.bufferSize){
                fileoutBuffer.queueMutex.lock();
                fileoutBuffer.bufferNotFull.wait(&fileoutBuffer.queueMutex);
                fileoutBuffer.queueMutex.unlock();
            }
            fileoutBuffer.bufferPacket(tempPacket);
            break;
        } else { //less than a full packet left
            if((bytesRead = file.read(tempBuf, sizeOfFile - totalRead)) == -1){
                WSAError(READ_ERROR);
            }
            mkPacket(tempPacket, MSG_FT, (unsigned short)bytesRead, reqNum_,tempBuf);
            if(fileoutBuffer.queue.size() == fileoutBuffer.bufferSize){
                fileoutBuffer.queueMutex.lock();
                fileoutBuffer.bufferNotFull.wait(&fileoutBuffer.queueMutex);
                fileoutBuffer.queueMutex.unlock();
            }
            fileoutBuffer.bufferPacket(tempPacket);
            //make EOT packet
            memset(tempBuf, 0, DATA_SIZE);
            mkPacket(endPack, MSG_FTCOMPLETE, 0, reqNum_, tempBuf);
            if(fileoutBuffer.queue.size() == fileoutBuffer.bufferSize){
                fileoutBuffer.queueMutex.lock();
                fileoutBuffer.bufferNotFull.wait(&fileoutBuffer.queueMutex);
                fileoutBuffer.queueMutex.unlock();
            }
            fileoutBuffer.bufferPacket(endPack);
            break;
        }
        thread->wait(10);
    }
    thread->wait();
    file.close();
    disconnect(thread, SIGNAL(sendPacket(char*, char)), this, SLOT(send(char*, char)));
    emit(endFT());
    return;
}
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
void FileReadThread::send(char* packet, char req){
    emit sendTCPPacket(packet, req);
}

FileSendThread::FileSendThread(){}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: FileSendThread::run
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
void FileSendThread::run(){

    char packet[PACKETSIZE];

    while(1){

        ZeroMemory(packet, PACKETSIZE);
        if(fileoutBuffer.queue.empty()){
            fileoutBuffer.queueMutex.lock();
            fileoutBuffer.bufferNotEmpty.wait(&fileoutBuffer.queueMutex);
            fileoutBuffer.queueMutex.unlock();
        }

        fileoutBuffer.grabPacket(packet);
        emit(sendPacket(packet, packet[3]));
        Sleep(1);
        if(packet[0] == MSG_FTCOMPLETE) {
            break;
        }
    }
    Sleep(10);
}

