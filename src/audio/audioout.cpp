#include "audioout.h"
#include "../network/audiotransfer.h"
void audioout::setupParams() {
    format_.setFrequency(frequency_);
    format_.setChannels(channels_);
    format_.setSampleSize(sampleSize_);
    format_.setCodec("audio/pcm");
    format_.setByteOrder(QAudioFormat::LittleEndian);
    format_.setSampleType(QAudioFormat::SignedInt);
}

int audioout::getParams(const char* params) {
    if(memcmp(params, hdr_, HDR_SIZE) != 0) {
        memcpy(hdr_, params, HDR_SIZE);
        memcpy(&frequency_, hdr_+24, 4);
        memcpy(&byteRate_, hdr_+28, 4);
        memcpy(&channels_, hdr_+22, 2);
        memcpy(&sampleSize_, hdr_+34, 2);
        if((byteRate_) != 0) {
            pause_ = (1/(byteRate_));
        }
        return 1;
    }
    return 0;
}

void audioout::createAudioDev() {
    if(output_ == NULL) {
        output_ = new QAudioOutput(format_);
        output_->setBufferSize(6000000);
        buffer_ = output_->start();
    }
}

void audioout::destroyAudioDev() {
    delete output_;
}

void audioout::playSound(char* sound){
    buffer_->write(sound+44, AUDIO_DATA_SIZE-4);
    Sleep(23);
}
