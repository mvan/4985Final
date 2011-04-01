#include "filetransfer.h"
#include "network.h"
#include "buffer.h"
#include <stdio.h>

Buffer fileoutBuffer;

FileReadThread::FileReadThread(HANDLE handle):file_(handle){}

void FileReadThread::run(){

    DWORD sizeOfFile;
    DWORD numOfReads = 0;
    DWORD bytesRead;
    DWORD bytesWritten;
    char* tempPacket;
    char* tempBuf;
    HANDLE file;

    tempPacket = (char *)malloc(PACKETSIZE);
    tempBuf = (char *)malloc(DATA_SIZE);

    ZeroMemory(tempPacket, PACKETSIZE);
    ZeroMemory(tempBuf, DATA_SIZE);
    sizeOfFile = GetFileSize(file_, NULL);

    file = CreateFile(TEXT("C:\\Users\\Daniel\\Desktop\\temp2.txt"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                      NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    FileSendThread *thread = new FileSendThread();
    thread->start();

    while((numOfReads * DATA_SIZE) < sizeOfFile){
        if((sizeOfFile - (numOfReads * DATA_SIZE)) > DATA_SIZE){ //More than a packet left
            if(!ReadFile(file_, tempBuf, DATA_SIZE, &bytesRead, NULL)){
                //error reading file
            }
            mkPacket(tempPacket, MSG_FT, (unsigned short) bytesRead, 0, tempBuf); //change with src and dest, msg type
            if(fileoutBuffer.queue.size() == fileoutBuffer.bufferSize){
                fileoutBuffer.queueMutex.lock();
                fileoutBuffer.bufferNotFull.wait(&fileoutBuffer.queueMutex);
                fileoutBuffer.queueMutex.unlock();
            }
            fileoutBuffer.bufferPacket(tempPacket);
            WriteFile(file, (tempPacket+4), dataLength(tempPacket), &bytesWritten, NULL);
            ++numOfReads;
        } else if((sizeOfFile - (numOfReads * DATA_SIZE)) == 0) { // finished exactly
            memset(tempBuf, 0, sizeof(tempBuf));
            mkPacket(tempPacket, MSG_FTCOMPLETE, (unsigned short) bytesRead, 0,tempBuf);
            if(fileoutBuffer.queue.size() == fileoutBuffer.bufferSize){
                fileoutBuffer.queueMutex.lock();
                fileoutBuffer.bufferNotFull.wait(&fileoutBuffer.queueMutex);
                fileoutBuffer.queueMutex.unlock();
            }
            fileoutBuffer.bufferPacket(tempPacket);
            CloseHandle(file_);
            free(tempPacket);
            free(tempBuf);
            break;
        } else { //less than a full packet left
            if(!ReadFile(file_, tempBuf, sizeOfFile - (numOfReads * DATA_SIZE), &bytesRead, NULL)){
                //error reading file
            }
            mkPacket(tempPacket, MSG_FT, (unsigned short)bytesRead,0 ,tempBuf); //change with src and dest, msg type
            if(fileoutBuffer.queue.size() == fileoutBuffer.bufferSize){
                fileoutBuffer.queueMutex.lock();
                fileoutBuffer.bufferNotFull.wait(&fileoutBuffer.queueMutex);
                fileoutBuffer.queueMutex.unlock();
            }
            fileoutBuffer.bufferPacket(tempPacket);
            WriteFile(file, (tempPacket+4), dataLength(tempPacket), &bytesWritten, NULL);
            //make EOT packet
            memset(tempBuf, 0, sizeof(tempBuf));
            mkPacket(tempPacket, MSG_FTCOMPLETE, (unsigned short)bytesRead, 0,tempBuf);
            if(fileoutBuffer.queue.size() == fileoutBuffer.bufferSize){
                fileoutBuffer.queueMutex.lock();
                fileoutBuffer.bufferNotFull.wait(&fileoutBuffer.queueMutex);
                fileoutBuffer.queueMutex.unlock();
            }
            fileoutBuffer.bufferPacket(tempPacket);
            CloseHandle(file_);
            free(tempPacket);
            free(tempBuf);
            emit(endFT());
            break;
        }
        thread->wait(1);
    }
    thread->wait();
}

FileSendThread::FileSendThread(){}

void FileSendThread::run(){
    HANDLE file;
    DWORD bytesWritten;
    char* packet;

    file = CreateFile(TEXT("C:\\Users\\Daniel\\Desktop\\temp.txt"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                      NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    packet = (char *)malloc(PACKETSIZE);

    while(1){
        if(fileoutBuffer.queue.size() == 0){
            fileoutBuffer.queueMutex.lock();
            fileoutBuffer.bufferNotEmpty.wait(&fileoutBuffer.queueMutex);
            fileoutBuffer.queueMutex.unlock();
        }
        fileoutBuffer.grabPacket(packet);
        //Send fileoutBuffer.grabPacket()
        if(packet[0] == MSG_FTCOMPLETE){
            free(packet);
            printf("file complete");
            break;
        }
        WriteFile(file, (packet+4), dataLength(packet), &bytesWritten, NULL);
        printf("packet written");
    }
}

