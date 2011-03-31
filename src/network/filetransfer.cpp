#include "filetransfer.h"
#include "network.h"
#include "buffer.h"
#include <stdio.h>

Buffer fileoutBuffer;

FileReadThread::FileReadThread(QString file):file_(file){}

void FileReadThread::run(){

    DWORD sizeOfFile = 0;
    DWORD numOfReads = 0;
    DWORD bytesRead = 0;
    DWORD bytesWritten = 0;
    char* tempPacket;
    char* tempBuf;
    QMutex mutex;
    wchar_t *wfilename = new wchar_t[file_.size() +1];
    wfilename[file_.size()] = '\0';

    tempPacket = (char *)malloc(PACKETSIZE);
    tempBuf = (char *)malloc(DATA_SIZE);
    ZeroMemory(tempPacket, PACKETSIZE);
    ZeroMemory(tempBuf, DATA_SIZE);


    HANDLE file;

    file_.toWCharArray(wfilename);
    file = CreateFile(wfilename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                      NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    sizeOfFile = GetFileSize(file, NULL);

    FileSendThread *thread = new FileSendThread();
    thread->start();

    while((numOfReads * DATA_SIZE) < sizeOfFile){
        if((sizeOfFile - (numOfReads * DATA_SIZE)) > DATA_SIZE){ //More than a packet left
            if(!ReadFile(file, tempBuf, DATA_SIZE, &bytesRead, NULL)){
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
            mkPacket(tempPacket, MSG_FTCOMPLETE, 0, 0,tempBuf);
            if(fileoutBuffer.queue.size() == fileoutBuffer.bufferSize){
                fileoutBuffer.queueMutex.lock();
                fileoutBuffer.bufferNotFull.wait(&fileoutBuffer.queueMutex);
                fileoutBuffer.queueMutex.unlock();
            }
            fileoutBuffer.bufferPacket(tempPacket);
            CloseHandle(file);
            free(tempPacket);
            free(tempBuf);
            break;
        } else { //less than a full packet left
            if(!ReadFile(file, tempBuf, sizeOfFile - (numOfReads * DATA_SIZE), &bytesRead, NULL)){
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
            mkPacket(tempPacket, MSG_FTCOMPLETE, 0, 0,tempBuf);
            if(fileoutBuffer.queue.size() == fileoutBuffer.bufferSize){
                fileoutBuffer.queueMutex.lock();
                fileoutBuffer.bufferNotFull.wait(&fileoutBuffer.queueMutex);
                fileoutBuffer.queueMutex.unlock();
            }
            fileoutBuffer.bufferPacket(tempPacket);
            CloseHandle(file);
            free(tempPacket);
            free(tempBuf);
            break;
        }
        thread->wait(10);
    }

}

FileSendThread::FileSendThread(){}

void FileSendThread::run(){
    HANDLE file;
    file = CreateFile(TEXT("C:\\Users\\Admin\\Desktop\\temp.txt"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                      NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    QMutex mutex;
    DWORD bytesWritten;
    char* packet;
    packet = (char *)malloc(PACKETSIZE * sizeof(char*));


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

