#include "filetransfer.h"
#include "network.h"
#include "buffer.h"
#include <stdio.h>
#include <QFile>

Buffer fileoutBuffer;

FileReadThread::FileReadThread(QString file):file_(file){}

void FileReadThread::run(){

    DWORD sizeOfFile = 0;
    DWORD numOfReads = 0;
    DWORD bytesRead = 0;

    char* tempPacket;
    char* tempBuf;

    tempPacket = (char *)malloc(PACKETSIZE);
    tempBuf = (char *)malloc(DATA_SIZE);
    ZeroMemory(tempPacket, PACKETSIZE);
    ZeroMemory(tempBuf, DATA_SIZE);

    QFile file(file_);

    if(!file.open(QIODevice::ReadOnly)){
        //error
    }


    sizeOfFile = file.size();

    FileSendThread *thread = new FileSendThread();
    thread->start();

    while((numOfReads * DATA_SIZE) < sizeOfFile){
        if((sizeOfFile - (numOfReads * DATA_SIZE)) > DATA_SIZE){ //More than a packet left
            if((bytesRead = file.read(tempBuf, DATA_SIZE)) == -1){
                //error reading file
            }
            mkPacket(tempPacket, MSG_FT, (unsigned short) bytesRead, 0, tempBuf); //change with src and dest, msg type
            if(fileoutBuffer.queue.size() == fileoutBuffer.bufferSize){
                fileoutBuffer.queueMutex.lock();
                fileoutBuffer.bufferNotFull.wait(&fileoutBuffer.queueMutex);
                fileoutBuffer.queueMutex.unlock();
            }
            fileoutBuffer.bufferPacket(tempPacket);
            ++numOfReads;
        } else if((sizeOfFile - (numOfReads * DATA_SIZE)) == 0) { // finished exactly
            memset(tempBuf, 0, sizeof(tempBuf));
            mkPacket(tempPacket, MSG_FTCOMPLETE, 0, 0,tempBuf);
            if(fileoutBuffer.queue.size() == fileoutBuffer.bufferSize){
                fileoutBuffer.queueMutex.lock();
                fileoutBuffer.bufferNotFull.wait(&fileoutBuffer.queueMutex);
                fileoutBuffer.queueMutex.unlock();
            }
            fileoutBuffer.bufferPacket(tempPacket);
            file.close();
            free(tempPacket);
            free(tempBuf);
            break;
        } else { //less than a full packet left
            if((bytesRead = file.read(tempBuf, sizeOfFile - (numOfReads * DATA_SIZE))) == -1){
                //error
            }
            mkPacket(tempPacket, MSG_FT, (unsigned short)bytesRead,0 ,tempBuf); //change with src and dest, msg type
            if(fileoutBuffer.queue.size() == fileoutBuffer.bufferSize){
                fileoutBuffer.queueMutex.lock();
                fileoutBuffer.bufferNotFull.wait(&fileoutBuffer.queueMutex);
                fileoutBuffer.queueMutex.unlock();
            }
            fileoutBuffer.bufferPacket(tempPacket);
            //make EOT packet
            memset(tempBuf, 0, sizeof(tempBuf));
            mkPacket(tempPacket, MSG_FTCOMPLETE, 0, 0,tempBuf);
            if(fileoutBuffer.queue.size() == fileoutBuffer.bufferSize){
                fileoutBuffer.queueMutex.lock();
                fileoutBuffer.bufferNotFull.wait(&fileoutBuffer.queueMutex);
                fileoutBuffer.queueMutex.unlock();
            }
            fileoutBuffer.bufferPacket(tempPacket);
            file.close();
            free(tempPacket);
            free(tempBuf);
            break;
        }
        thread->wait(10);
    }

    thread->wait();
}

void FileReadThread::send(char* packet){
    emit send(packet);
}

FileSendThread::FileSendThread(){}

void FileSendThread::run(){

    char* packet;
    packet = (char *)malloc(PACKETSIZE);


   while(1){
        if(fileoutBuffer.queue.size() == 0){
            fileoutBuffer.queueMutex.lock();
            fileoutBuffer.bufferNotEmpty.wait(&fileoutBuffer.queueMutex);
            fileoutBuffer.queueMutex.unlock();
        }
        fileoutBuffer.grabPacket(packet);
        if(packet[0] == MSG_FTCOMPLETE){
            free(packet);
            break;
        };
    }
}

