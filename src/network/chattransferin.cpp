#include "chattransferin.h"
#include "buffer.h"
#include <winsock2.h>
#include <QMessageBox>

Buffer chatinBuffer;


void ChatWriteThread::run(){

    char* packet;
    char* tempPacket;
    packet = (char *)malloc(PACKETSIZE);
    tempPacket = (char *)malloc(PACKETSIZE);

    while(1){

        if(chatinBuffer.queue.size() == 0){
            chatinBuffer.queueMutex.lock();
            chatinBuffer.bufferNotEmpty.wait(&chatinBuffer.queueMutex);
            chatinBuffer.queueMutex.unlock();
        }
        chatinBuffer.grabPacket(tempPacket);
        strcpy(packet, tempPacket);
        emit(addChatToDisplay(packet));
        ZeroMemory(packet, PACKETSIZE);

    }

    free(packet);
    free(tempPacket);
}


