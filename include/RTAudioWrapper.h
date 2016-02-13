#ifndef RTAUDIOWRAPPER_H
#define	RTAUDIOWRAPPER_H

#include "AudioInterface.h"
#include "RtAudio.h"

/*!
 * Implementation of concrete AudioInterface (Wrapper class of RtAudio)
 */
class RtAudioWrapper : public AudioInterface
{
public:
    RtAudioWrapper();
    ~RtAudioWrapper();

    /* Callbacks */
    auto callback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *userData) -> int;
    static auto callbackHelper(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *rtAudioWrapperObject) -> int;

protected:
    RtAudio rtaudio;
    RtAudio::StreamParameters inputConfig, outputConfig;
	unsigned int bufferSizeInNumberOfSamples, inputBufferSizeInBytes, outputBufferSizeInBytes;

	auto getRtAudioFormat(const AudioConfiguration &audioConfig) -> RtAudioFormat;
	auto setActualBufferSizes() -> void;
	auto initializeRtAudioStreamParameters() -> void;

	auto vStartRecordingMode() -> void;
	auto vStartPlaybackMode() -> void;
	auto vStartDuplexMode() -> void;
	auto vSetConfiguration(AudioConfiguration audioConfiguration) -> void;
	auto vSuspend() -> void;
	auto vResume() -> void;
	auto vStop() -> void;
	auto vReset() -> void;
	auto vSetDefaultAudioConfig() -> void;
	auto vPrepareForExecution() -> bool;
	auto vGetBufferSize() -> unsigned int;
};

#endif
