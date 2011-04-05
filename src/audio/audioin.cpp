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
    QAudioDeviceInfo i(QAudioDeviceInfo::defaultOutputDevice());
    if(!i.isFormatSupported(format_)) {
        return;
    } else {
        input_ = new QAudioOutput(format_);
        buffer_ = input_->start();
        inbuf_ = (QBuffer*)buffer_;
    }
}

void audioin::destroyAudioDev() {
    delete input_;
}

void audioin::readSound(){
    char buf[DATA_SIZE];
    char packet[PACKETSIZE];
    mkVoiceHdr(buf);
    Sleep(8);
    inbuf_->read(buf+HDR_SIZE, AUDIO_DATA_SIZE);
    mkPacket(packet, MSG_MIC, DATA_SIZE, 0, buf);
    audiooutBuffer.bufferPacket(packet);
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
