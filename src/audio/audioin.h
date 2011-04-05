#ifndef AUDIOIN_H
#define AUDIOIN_H
#include <QObject>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QBuffer>
#include "../network/audiotransfer.h"
class audioin: public QObject {
    Q_OBJECT

    private:
        QAudioOutput* input_;
        QAudioFormat format_;
        QIODevice* buffer_;
        QBuffer* inbuf_;

        char hdr_[HDR_SIZE];
    public:
        explicit audioin(){}
        virtual ~audioin(){}
        void setupParams();
        void createAudioDev();
        void destroyAudioDev();
        void mkVoiceHdr(char* buf);
    public slots:
        void readSound();
};
#endif
