#include "filetransfer.h"
#include "network.h"
#include "buffer.h"

void FileTransferThread::run(){

    DWORD sizeOfFile;
    int numOfReads = 0;
    DWORD bytesRead;
    char tempPacket[PACKETSIZE];
    char tempBuf[DATA_SIZE];

    sizeOfFile = GetFileSize(file_, NULL);

    while((numOfReads * DATA_SIZE) < sizeOfFile){
        if((sizeOfFile - (numOfReads * DATA_SIZE)) > DATA_SIZE){ //More than a packet left
            if(!ReadFile(file_, tempBuf, DATA_SIZE, &bytesRead, NULL)){
                //error reading file
            }
            mkPacket(tempPacket, 0, 0, 0, tempBuf); //change with src and dest, msg type
            /*if(fileoutBuffer.queue.size() == fileoutBuffer.bufferSize){
                fileoutBuffer.bufferNotFull.wait(&fileoutBuffer.queueMutex);
            }
            fileoutBuffer.bufferPacket(tempPacket);*/
            ++numOfReads;
        } else if((sizeOfFile - (numOfReads * DATA_SIZE)) == 0) { // finished exactly
            CloseHandle(file_);
            break;
        } else { //less than a full packet left
            if(!ReadFile(file_, tempBuf, DATA_SIZE, &bytesRead, NULL)){
                //error reading file
            }
            mkPacket(tempPacket, 0, 0, 0, tempBuf); //change with src and dest, msg type
            /*if(fileoutBuffer.queue.size() == fileoutBuffer.bufferSize){
                fileoutBuffer.bufferNotFull.wait(&fileoutBuffer.queueMutex);
            }
            fileoutBuffer.bufferPacket(tempPacket);*/
            CloseHandle(file_);
            break;
        }
    }
}
