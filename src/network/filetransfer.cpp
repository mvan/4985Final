#include "filetransfer.h"
#include "network.h"
#include "buffer.h"

Buffer fileoutBuffer;

FileReadThread::FileReadThread(HANDLE handle):file_(handle){}

void FileReadThread::run(){

    DWORD sizeOfFile;
    DWORD numOfReads = 0;
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
            mkPacket(tempPacket, MSG_FT, (unsigned short) bytesRead, 0, tempBuf); //change with src and dest, msg type
            if(fileoutBuffer.queue.size() == fileoutBuffer.bufferSize){
                mutex.lock();
                fileoutBuffer.bufferNotFull.wait(&mutex);
                mutex.unlock();
            }
            fileoutBuffer.bufferPacket(tempPacket);
            ++numOfReads;
        } else if((sizeOfFile - (numOfReads * DATA_SIZE)) == 0) { // finished exactly
            memset(tempBuf, 0, sizeof(tempBuf));
            mkPacket(tempPacket, MSG_FTCOMPLETE, (unsigned short) bytesRead, 0,tempBuf);
            fileoutBuffer.bufferPacket(tempPacket);
            CloseHandle(file_);
            break;
        } else { //less than a full packet left
            if(!ReadFile(file_, tempBuf, sizeOfFile - (numOfReads * DATA_SIZE), &bytesRead, NULL)){
                //error reading file
            }
            mkPacket(tempPacket, MSG_FT, (unsigned short)bytesRead,0 ,tempBuf); //change with src and dest, msg type
            if(fileoutBuffer.queue.size() == fileoutBuffer.bufferSize){
                fileoutBuffer.bufferNotFull.wait(&fileoutBuffer.queueMutex);
            }
            fileoutBuffer.bufferPacket(tempPacket);
            //make EOT packet
            memset(tempBuf, 0, sizeof(tempBuf));
            mkPacket(tempPacket, MSG_FTCOMPLETE, (unsigned short)bytesRead, 0,tempBuf);
            fileoutBuffer.bufferPacket(tempPacket);
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
    char* packet;

   while(1){
        fileoutBuffer.queueMutex.lock();
        if(fileoutBuffer.queue.size() != 0){
            packet = fileoutBuffer.grabPacket();
            //Send fileoutBuffer.grabPacket()
            if(packet[0] == MSG_FTCOMPLETE){
                fileoutBuffer.queueMutex.unlock();
                break;
            }
            //WriteFile(file, packet, DATA_SIZE, &bytesWritten, NULL);
        }
        fileoutBuffer.queueMutex.unlock();
    }
}

