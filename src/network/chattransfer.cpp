#include "chattransfer.h"
#include "network.h"
#include "buffer.h"

Buffer chatoutBuffer;
void ChatSendThread::run(){

    QMutex mutex;
    char* packet;
    packet = (char*)malloc(PACKETSIZE);

    while(1){

        chatoutBuffer.queueMutex.lock();
        if(chatoutBuffer.queue.size() != 0){

            strcpy(packet, chatoutBuffer.grabPacket());
            emit(sendChatPacket(packet));
            ZeroMemory(packet, PACKETSIZE);

        }
        chatoutBuffer.queueMutex.unlock();

    }
    free(packet);
}
