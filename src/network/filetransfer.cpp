#include "filetransfer.h"
#include "network.h"
#include "buffer.h"
#include <stdio.h>
#include <QFile>

Buffer fileoutBuffer;

FileReadThread::FileReadThread(QString file):file_(file){}

void FileReadThread::run(){

    int sizeOfFile = 0;
    int bytesRead = 0;
    int totalRead = 0;

    char* tempPacket, * tempBuf, *endPack;
    tempPacket = (char *)malloc(PACKETSIZE);
    tempBuf = (char *)malloc(DATA_SIZE);
    endPack = (char*)malloc(PACKETSIZE);

    ZeroMemory(tempPacket, PACKETSIZE);
    ZeroMemory(tempBuf, DATA_SIZE);
    ZeroMemory(endPack, PACKETSIZE);

    QFile file(file_);

    if(!file.open(QIODevice::ReadOnly)){
        //error
    }

    sizeOfFile = file.size();

    FileSendThread *thread = new FileSendThread();
    connect(thread, SIGNAL(sendPacket(char*)), this, SLOT(send(char*)), Qt::QueuedConnection);
    thread->start();

    while(totalRead <= sizeOfFile){
        if((sizeOfFile - totalRead) > DATA_SIZE){ //More than a packet left
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
            totalRead += bytesRead;
        } else if(sizeOfFile == totalRead) { // finished exactly
            memset(tempBuf, 0, sizeof(tempBuf));
            mkPacket(tempPacket, MSG_FTCOMPLETE, 0, 0,tempBuf);
            if(fileoutBuffer.queue.size() == fileoutBuffer.bufferSize){
                fileoutBuffer.queueMutex.lock();
                fileoutBuffer.bufferNotFull.wait(&fileoutBuffer.queueMutex);
                fileoutBuffer.queueMutex.unlock();
            }
            fileoutBuffer.bufferPacket(tempPacket);
            break;
        } else { //less than a full packet left
            if((bytesRead = file.read(tempBuf, sizeOfFile - totalRead)) == -1){
                //error
            }
            mkPacket(tempPacket, MSG_FT, (unsigned short)bytesRead, 0 ,tempBuf);
            if(fileoutBuffer.queue.size() == fileoutBuffer.bufferSize){
                fileoutBuffer.queueMutex.lock();
                fileoutBuffer.bufferNotFull.wait(&fileoutBuffer.queueMutex);
                fileoutBuffer.queueMutex.unlock();
            }
            fileoutBuffer.bufferPacket(tempPacket);
            //make EOT packet
            memset(tempBuf, 0, DATA_SIZE);
            mkPacket(endPack, MSG_FTCOMPLETE, 0, 0,tempBuf);
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
    free(tempPacket);
    free(tempBuf);
    free(endPack);
    connect(thread, SIGNAL(sendPacket(char*)), this, SLOT(send(char*)), Qt::QueuedConnection);
    emit(endFT());
    return;
}

void FileReadThread::send(char* packet){
    emit sendTCPPacket(packet);
}

FileSendThread::FileSendThread(){}

void FileSendThread::run(){

    char* packet;
    packet = (char *)malloc(PACKETSIZE);

    while(1){

        if(fileoutBuffer.queue.empty()){
            fileoutBuffer.queueMutex.lock();
            fileoutBuffer.bufferNotEmpty.wait(&fileoutBuffer.queueMutex);
            fileoutBuffer.queueMutex.unlock();
        }

        fileoutBuffer.grabPacket(packet);
        emit(sendPacket(packet));
        Sleep(100);
        if(packet[0] == MSG_FTCOMPLETE) {
            break;
        }
    }
    Sleep(100);
    free(packet);
}

