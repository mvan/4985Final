/**
  * Class: audioin
  *
  * Description:
  * AudioIn is the tool for all audio input. Via file streaming or microphone.
  */

#ifndef AUDIOIN_H
#define AUDIOIN_H
#include <QObject>
#include <QAudioInput>
#include <QAudioFormat>
#include <QAudioInput>
#include <QBuffer>
#include "../network/audiotransfer.h"
class audioin: public QObject {
    Q_OBJECT

private:
    /** Audio input object that performs operations for input.*/
    QAudioInput* input_;

    /** The format for the current audio input. */
    QAudioFormat format_;

    /** The buffer connected to AudioInput that all input gets written to. */
    QIODevice* buffer_;

    /** The buffer used instead of QIODevice for reading input. */
    QBuffer* inbuf_;

    /** Number of bytes to read from input. */
    qint64 numToRead_;

    /** The header for the audio. */
    char hdr_[HDR_SIZE];
public:
    explicit audioin():numToRead_(0){}
    virtual ~audioin(){}

    /**
      * Function: setupParams()
      *
      * @return void
      *
      * Description:
      * Sets up the audio format parameters for a wav file.
      */
    void setupParams();

    /**
      * Function: createAudioDev()
      *
      * @return void
      *
      * Description:
      * Sets up the default audio input device and the input buffers.
      */
    void createAudioDev();

    /**
      * Function: destroyAudioDev()
      *
      * @return void
      *
      * Description:
      * Cleans up the AudioInput and input buffers.
      */
    void destroyAudioDev();

    /**
      * Function: mkVoiceHdr(char* buf)
      *
      * @return void
      *
      * @param buf the buffer to append the voice header to.
      *
      * Description:
      * Sets up the audio format header for the microphone.
      */
    void mkVoiceHdr(char* buf);
public slots:

    /**
      * Slot: readSound()
      *
      * Description:
      * Reads all available data from the audio input device,
      * and writes it to a buffer.
      */
    void readSound();

    /**
      * Slot: writeReady(qint64 bytes)
      *
      * @param bytes Number of bytes available to read
      *
      * Description:
      * Sets numToRead_ to the available bytes to read.
      */
    void writeReady(qint64 bytes);
};
#endif
