#include "filetransfer.h"
#include "network.h"
#include "buffer.h"

Buffer fileoutBuffer;

FileReadThread::FileReadThread(HANDLE handle):file_(handle){}

void FileReadThread::run(){

    DWORD sizeOfFile;
    int numOfReads = 0;
    DWORD bytesRead;
    char* tempPacket;
    char* tempBuf;
    QMutex mutex;

    tempPacket = (char *)malloc(PACKETSIZE * sizeof(char *));
    tempBuf = (char *)malloc(DATA_SIZE * sizeof(char *));
    ZeroMemory(tempPacket, PACKETSIZE);
    ZeroMemory(tempBuf, DATA_SIZE);
    sizeOfFile = GetFileSize(file_, NULL);

    HANDLE file;
    file = CreateFile(TEXT("C:\\Users\\Admin\\Desktop\\temp.txt"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                      NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    FileSendThread *thread = new FileSendThread();
    thread->start();

    while((numOfReads * DATA_SIZE) < sizeOfFile){
        if((sizeOfFile - (numOfReads * DATA_SIZE)) > DATA_SIZE){ //More than a packet left
            if(!ReadFile(file_, tempBuf, DATA_SIZE, &bytesRead, NULL)){
                //error reading file
            }
            //mkPacket(tempPacket, 0, 0, 0, tempBuf); //change with src and dest, msg type
            /*if(fileoutBuffer.queue.size() == fileoutBuffer.bufferSize){
                mutex.lock();
                fileoutBuffer.bufferNotFull.wait(&mutex);
                mutex.unlock();
            }*/
            fileoutBuffer.bufferPacket(tempBuf);
            ++numOfReads;
        } else if((sizeOfFile - (numOfReads * DATA_SIZE)) == 0) { // finished exactly
            CloseHandle(file_);
            break;
        } else { //less than a full packet left
            if(!ReadFile(file_, tempBuf, DATA_SIZE, &bytesRead, NULL)){
                //error reading file
            }
            //mkPacket(tempPacket, 0, 0, 0, tempBuf); //change with src and dest, msg type
            if(fileoutBuffer.queue.size() == fileoutBuffer.bufferSize){
                fileoutBuffer.bufferNotFull.wait(&fileoutBuffer.queueMutex);
            }
            fileoutBuffer.bufferPacket(tempBuf);
            CloseHandle(file_);
            break;
        }
    }
}

FileSendThread::FileSendThread(){}

void FileSendThread::run(){
    HANDLE file;
    file = CreateFile(TEXT("C:\\Users\\Admin\\Desktop\\temp.txt"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                      NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    QMutex mutex;
    DWORD bytesWritten;

   while(1){
        /*mutex.lock();
        fileoutBuffer.bufferNotEmpty.wait(&mutex);
        mutex.unlock();*/
        mutex.lock();
        if(fileoutBuffer.queue.size() != 0){
            //Send fileoutBuffer.grabPacket()
            //WriteFile(file, fileoutBuffer.grabPacket(), DATA_SIZE, &bytesWritten, NULL);
        }
        mutex.unlock();
        //fileoutBuffer.bufferNotFull.wakeAll();
    }
}

