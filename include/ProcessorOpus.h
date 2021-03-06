#ifndef PROCESSOROPUS_H

#define PROCESSOROPUS_H
#include "AudioProcessor.h"
#include "opus.h"
#include ""

#include <iostream>

enum class OpusCodingModes 
{
	APPLICATION_VOIP = OPUS_APPLICATION_VOIP,
	APPLICATION_AUDIO = OPUS_APPLICATION_AUDIO,
	APPLICATION_RESTRICTED_LOWDELAY = OPUS_APPLICATION_RESTRICTED_LOWDELAY
};

class ProcessorOpus : public AudioProcessor
{
public:
    ProcessorOpus(const std::string name, OpusCodingModes opusApplication);

    //returns supported Audio Formats by Opus: only rtaudio SINT16 or FLOAT32 supported
    unsigned int getSupportedAudioFormats() const;
    //returns supported Sample Rates by Opus: Opus supports this sampleRates: 8000, 12000, 16000, 24000, or 48000.
    unsigned int getSupportedSampleRates() const;

    //configure the Opus Processor, this creates OpusEncoder and OpusDecoderObject and initializes private variables: outputDeviceChannels, rtaudioFormat and ErrorCode
    bool configure(const AudioConfiguration& audioConfig);

    //encodes the Data in inputBuffer(only Signed 16bit or float 32bit PCM and one frame supported) and writes the encoded Data in inputBuffer
    //supported size of one frame(2.5, 5, 10, 20, 40 or 60 ms) of audio data; at 48 kHz the permitted values are 120, 240, 480(10ms), 960(20ms), 1920, and 2880(60ms)
    //returns lenght of encodedPacket in Bytes
    unsigned int processInputData(void *inputBuffer, const unsigned int inputBufferByteSize, void *userData);

    //decodes the Data in outputBuffer and writes it to the outputBuffer(only Signed 16bit or float 32bit PCM and one frame supported)
    //supported size of one frame(2.5, 5, 10, 20, 40 or 60 ms) of audio data; at 48 kHz the permitted values are 120, 240, 480(10ms), 960(20ms), 1920, and 2880(60ms)
    //returns size of outputBuffer in Bytes
    unsigned int processOutputData(void *outputBuffer, const unsigned int outputBufferByteSize, void *userData);

    //destructor: destroys OpusEncoder and OpusDecoderObject
    ~ProcessorOpus();

private:
	OpusEncoder *OpusEncoderObject;
	OpusDecoder *OpusDecoderObject;
	OpusCodingModes OpusApplication;
	AudioConfiguration audioConfig;
	int encodeErrorCode;
	int decodeErrorCode;
};
#endif
