#include "audiotransfer.h"
#include "network.h"
#include "buffer.h"

Buffer audiooutBuffer;

AudioReadThread::AudioReadThread(HANDLE handle):file_(handle){}

void AudioReadThread::run(){

    DWORD sizeOfFile;
    DWORD numOfReads = 0;
    DWORD bytesRead;
    char* tempPacket;
    char* tempBuf;

    tempPacket = (char *)malloc(PACKETSIZE);
    tempBuf = (char *)malloc(DATA_SIZE);
    ZeroMemory(tempPacket, PACKETSIZE);
    ZeroMemory(tempBuf, DATA_SIZE);
    sizeOfFile = GetFileSize(file_, NULL);

    HANDLE file;
    file = CreateFile(TEXT("C:\\Users\\Admin\\Desktop\\temp.txt"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                      NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    AudioSendThread *thread = new AudioSendThread();
    thread->start();

    while((numOfReads * DATA_SIZE) < sizeOfFile){
        if((sizeOfFile - (numOfReads * DATA_SIZE)) > DATA_SIZE){ //More than a packet left
            if(!ReadFile(file_, tempBuf, DATA_SIZE, &bytesRead, NULL)){
                //error reading file
            }
            mkPacket(tempPacket, MSG_AUDIO, (unsigned short)bytesRead, 0,tempBuf); //change with src and dest, msg type
            if(audiooutBuffer.queue.size() == audiooutBuffer.bufferSize){
                audiooutBuffer.queueMutex.lock();
                audiooutBuffer.bufferNotFull.wait(&audiooutBuffer.queueMutex);
                audiooutBuffer.queueMutex.unlock();
            }
            audiooutBuffer.bufferPacket(tempPacket);
            ++numOfReads;
        } else if((sizeOfFile - (numOfReads * DATA_SIZE)) == 0) { // finished exactly
            memset(tempBuf, 0, sizeof(tempBuf));
            mkPacket(tempPacket, MSG_FTCOMPLETE, (unsigned short)bytesRead, 0, tempBuf);
            if(audiooutBuffer.queue.size() == audiooutBuffer.bufferSize){
                audiooutBuffer.queueMutex.lock();
                audiooutBuffer.bufferNotFull.wait(&audiooutBuffer.queueMutex);
                audiooutBuffer.queueMutex.unlock();
            }
            audiooutBuffer.bufferPacket(tempPacket);
            CloseHandle(file_);
            break;
        } else { //less than a full packet left
            if(!ReadFile(file_, tempBuf, sizeOfFile - (numOfReads * DATA_SIZE), &bytesRead, NULL)){
                //error reading file
            }
            mkPacket(tempPacket, MSG_AUDIO, (unsigned short)bytesRead, 0, tempBuf); //change with src and dest, msg type
            if(audiooutBuffer.queue.size() == audiooutBuffer.bufferSize){
                audiooutBuffer.queueMutex.lock();
                audiooutBuffer.bufferNotFull.wait(&audiooutBuffer.queueMutex);
                audiooutBuffer.queueMutex.unlock();
            }
            audiooutBuffer.bufferPacket(tempPacket);
            //make EOT packet
            memset(tempBuf, 0, sizeof(tempBuf));
            mkPacket(tempPacket, MSG_FTCOMPLETE, (unsigned short)bytesRead, 0, tempBuf);
            if(audiooutBuffer.queue.size() == audiooutBuffer.bufferSize){
                audiooutBuffer.queueMutex.lock();
                audiooutBuffer.bufferNotFull.wait(&audiooutBuffer.queueMutex);
                audiooutBuffer.queueMutex.unlock();
            }
            audiooutBuffer.bufferPacket(tempPacket);
            emit(endStream());
            CloseHandle(file_);
            break;
        }
        thread->wait(1);
    }
    thread->wait();
}

AudioSendThread::AudioSendThread(){}

void AudioSendThread::run(){

    char* packet = (char *)malloc(PACKETSIZE);

    while(1){
        if(audiooutBuffer.queue.size() == 0){
            audiooutBuffer.queueMutex.lock();
            audiooutBuffer.bufferNotEmpty.wait(&audiooutBuffer.queueMutex);
            audiooutBuffer.queueMutex.unlock();
        }
        audiooutBuffer.grabPacket(packet);
        //Send fileoutBuffer.grabPacket()
        if(packet[0] == MSG_FTCOMPLETE){
            free(packet);
            break;
        }

    }
}
