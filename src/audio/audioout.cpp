#include "audioout.h"
void audioout::setupParams() {
    format_.setFrequency(8000);
    format_.setChannels(1);
    format_.setSampleSize(8);
    format_.setCodec("audio/pcm");
    format_.setByteOrder(QAudioFormat::LittleEndian);
    format_.setSampleType(QAudioFormat::SignedInt);
}

int audioout::getParams(char* params) {
    if(memcmp(params, hdr_, HDR_SIZE) != 0) {
        memcpy(hdr_, params, HDR_SIZE);
        memcpy(&frequency_, params+24, 4);
        memcpy(&channels_, params+22, 2);
        memcpy(&sampleSize_, params+34, 2);
        return 1;
    }
    return 0;
}

void audioout::createAudioDev() {
    output_ = new QAudioOutput(format_);
    buffer_ = output_->start();
}

void audioout::destroyAudioDev() {
    delete output_;
}

void audioout::playSound(char* sound){
    buffer_->write(sound);
}
