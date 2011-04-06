#include "audioin.h"
#include "../network/externs.h"
#include "../network/network.h"
void audioin::setupParams() {
    format_.setFrequency(8000);
    format_.setChannels(2);
    format_.setSampleSize(16);
    format_.setCodec("audio/pcm");
    format_.setByteOrder(QAudioFormat::LittleEndian);
    format_.setSampleType(QAudioFormat::SignedInt);
}

void audioin::createAudioDev() {
    QAudioDeviceInfo i(QAudioDeviceInfo::defaultInputDevice());
    if(!i.isFormatSupported(format_)) {
        format_ = i.nearestFormat(format_);
    } else {
        input_ = new QAudioInput(format_);
        buffer_ = new QBuffer();
        buffer_->open(QIODevice::ReadWrite);
        input_->start(inbuf_);
    }
}

void audioin::destroyAudioDev() {
    delete input_;
    delete buffer_;
}

void audioin::readSound(){

    char buf[DATA_SIZE];
    char packet[PACKETSIZE];

    if(input_->bytesReady() >= AUDIO_DATA_SIZE) {
        mkVoiceHdr(buf);
        buffer_->seek(0);
        buffer_->read(buf+HDR_SIZE, AUDIO_DATA_SIZE);
        mkPacket(packet, MSG_MIC, DATA_SIZE, 0, buf);
        if(audiooutBuffer.queue.size() == audiooutBuffer.bufferSize) {
            audiooutBuffer.queueMutex.lock();
            audiooutBuffer.bufferNotFull.wait(&audiooutBuffer.queueMutex);
            audiooutBuffer.queueMutex.unlock();
        }
        audiooutBuffer.bufferPacket(packet);
    }
}

void audioin::mkVoiceHdr(char* buf) {
    int frequency = 8000;
    short channels = 2;
    short sampleSize = 16;
    int byteRate = ((frequency*channels*sampleSize)/8);

    memcpy(buf+24, &frequency, 4);
    memcpy(buf+28, &byteRate, 4);
    memcpy(buf+22, &channels, 2);
    memcpy(buf+34, &sampleSize, 2);
}
