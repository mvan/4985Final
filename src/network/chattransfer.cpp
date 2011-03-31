#include "chattransfer.h"
#include "network.h"
#include "buffer.h"

Buffer chatoutBuffer;
void ChatSendThread::run(){

    QMutex mutex;
    char* packet;
    char* tempPacket;
    packet = (char*)malloc(PACKETSIZE);
    tempPacket = (char*)malloc(PACKETSIZE);

    while(1){
        if(chatoutBuffer.queue.size() == 0){
            chatoutBuffer.queueMutex.lock();
            chatoutBuffer.bufferNotEmpty.wait(&chatoutBuffer.queueMutex);
            chatoutBuffer.queueMutex.unlock();
        }
        chatoutBuffer.grabPacket(tempPacket);
        strcpy(packet, tempPacket);
        emit(sendChatPacket(packet));
        ZeroMemory(packet, PACKETSIZE);

    }
    free(packet);
    free(tempPacket);
}
