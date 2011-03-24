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
    QMutex mutex;

    tempPacket = (char *)malloc(PACKETSIZE * sizeof(char *));
    tempBuf = (char *)malloc(DATA_SIZE * sizeof(char *));
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
            mkPacket(tempPacket, MSG_AUDIO, tempBuf); //change with src and dest, msg type
            if(audiooutBuffer.queue.size() == audiooutBuffer.bufferSize){
                mutex.lock();
                audiooutBuffer.bufferNotFull.wait(&mutex);
                mutex.unlock();
            }
            audiooutBuffer.bufferPacket(tempPacket);
            ++numOfReads;
        } else if((sizeOfFile - (numOfReads * DATA_SIZE)) == 0) { // finished exactly
            memset(tempBuf, 0, sizeof(tempBuf));
            mkPacket(tempPacket, MSG_FTCOMPLETE, tempBuf);
            audiooutBuffer.bufferPacket(tempPacket);
            CloseHandle(file_);
            break;
        } else { //less than a full packet left
            if(!ReadFile(file_, tempBuf, sizeOfFile - (numOfReads * DATA_SIZE), &bytesRead, NULL)){
                //error reading file
            }
            mkPacket(tempPacket, MSG_AUDIO, tempBuf); //change with src and dest, msg type
            if(audiooutBuffer.queue.size() == audiooutBuffer.bufferSize){
                audiooutBuffer.bufferNotFull.wait(&audiooutBuffer.queueMutex);
            }
            audiooutBuffer.bufferPacket(tempPacket);
            //make EOT packet
            memset(tempBuf, 0, sizeof(tempBuf));
            mkPacket(tempPacket, MSG_FTCOMPLETE, tempBuf);
            audiooutBuffer.bufferPacket(tempPacket);
            CloseHandle(file_);
            break;
        }
    }
}

AudioSendThread::AudioSendThread(){}

void AudioSendThread::run(){

    QMutex mutex;
    DWORD bytesWritten;
    char* packet;

   while(1){
        audiooutBuffer.queueMutex.lock();
        if(audiooutBuffer.queue.size() != 0){
            packet = audiooutBuffer.grabPacket();
            //Send fileoutBuffer.grabPacket()
            if(packet[0] == MSG_FTCOMPLETE){
                audiooutBuffer.queueMutex.unlock();
                break;
            }
        }
        audiooutBuffer.queueMutex.unlock();
    }
}
