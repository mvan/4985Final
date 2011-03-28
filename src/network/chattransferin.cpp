#include "chattransferin.h"
#include "buffer.h"
#include <winsock2.h>
#include <QMessageBox>

Buffer chatinBuffer;


void ChatWriteThread::run(){

    char* packet;
    packet = (char *)malloc(PACKETSIZE);

    while(1){
        chatinBuffer.queueMutex.lock();
        if(chatinBuffer.queue.size() != 0){

            strcpy(packet, chatinBuffer.grabPacket());
            chatinBuffer.queueMutex.unlock();

            emit(addChatToDisplay(packet));
            ZeroMemory(packet, PACKETSIZE);

        }
        chatinBuffer.queueMutex.unlock();
    }

    free(packet);
}


