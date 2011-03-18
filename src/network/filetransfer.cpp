#include "filetransfer.h"
#include "network.h"
#include "buffer.h"


FileTransferThread::FileTransferThread(HANDLE handle):file_(handle){}

void FileTransferThread::run(){

    DWORD sizeOfFile;
    int numOfReads = 0;
    DWORD bytesRead;
    char* tempPacket;
    char* tempBuf;
    Buffer fileoutBuffer;


    tempPacket = (char *)malloc(PACKETSIZE * sizeof(char *));
    tempBuf = (char *)malloc(DATA_SIZE * sizeof(char *));
    ZeroMemory(tempPacket, PACKETSIZE);
    ZeroMemory(tempBuf, DATA_SIZE);
    sizeOfFile = GetFileSize(file_, NULL);

    HANDLE file;
    file = CreateFile(TEXT("C:\\Users\\Admin\\Desktop\\temp.txt"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                      NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    while((numOfReads * DATA_SIZE) < sizeOfFile){
        if((sizeOfFile - (numOfReads * DATA_SIZE)) > DATA_SIZE){ //More than a packet left
            if(!ReadFile(file_, tempBuf, DATA_SIZE, &bytesRead, NULL)){
                //error reading file
            }
            //mkPacket(tempPacket, 0, 0, 0, tempBuf); //change with src and dest, msg type
            if(fileoutBuffer.queue.size() == fileoutBuffer.bufferSize){
                fileoutBuffer.bufferNotFull.wait(&fileoutBuffer.queueMutex);
            }
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
