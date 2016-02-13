#include "RTAudioWrapper.h"

RtAudioWrapper::RtAudioWrapper() 
{ }

RtAudioWrapper::~RtAudioWrapper()
{ }

auto RtAudioWrapper::callbackHelper(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *rtAudioWrapperObject) -> int
{
    auto rtAudioWrapper = static_cast <RtAudioWrapper*> (rtAudioWrapperObject);
    return rtAudioWrapper->callback(outputBuffer, inputBuffer, nBufferFrames, streamTime, status, nullptr);
}

auto RtAudioWrapper::callback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *rtAudioWrapperObject) -> int
{
	switch (status)
	{
	case(RTAUDIO_INPUT_OVERFLOW) :
		*(this->outputStream) << "Overflow" << std::endl;
		break;
	case(RTAUDIO_OUTPUT_UNDERFLOW) :
		*(this->outputStream) << "Underflow" << std::endl;
		break;
	}
   
    if (inputBuffer != nullptr)
        this->processAudioInput(inputBuffer, this->inputBufferSizeInBytes, nullptr);

    if (outputBuffer != nullptr)
        this->processAudioOutput(outputBuffer, this->outputBufferSizeInBytes, nullptr);

    return 0;
}

auto getRtAudioFormat(const AudioConfiguration &audioConfig) -> RtAudioFormat
{
	RtAudioFormat rtAudioFormat;
	switch (audioConfig.audioFormat)
	{
	case(AudioFormat::SIGNED_INT_8) :
		rtAudioFormat = RTAUDIO_SINT8;
		break;
	case(AudioFormat::SIGNED_INT_16) :
		rtAudioFormat = RTAUDIO_SINT16;
		break;
	case(AudioFormat::SIGNED_INT_24) :
		rtAudioFormat = RTAUDIO_SINT24;
		break;
	case(AudioFormat::SIGNED_INT_32) :
		rtAudioFormat = RTAUDIO_SINT32;
		break;
	case(AudioFormat::FLOAT_32) :
		rtAudioFormat = RTAUDIO_FLOAT32;
		break;
	case(AudioFormat::FLOAT_64) :
		rtAudioFormat = RTAUDIO_FLOAT64;
		break;
	default:
		rtAudioFormat = RTAUDIO_SINT16;
	}
	return rtAudioFormat;
}

auto RtAudioWrapper::setActualBufferSizes() -> void
{
	int sizeAudioFormat = static_cast<int>(this->audioConfiguration.audioFormat);
	this->outputBufferSizeInBytes = this->bufferSizeInNumberOfSamples * sizeAudioFormat * this->audioConfiguration.outputDeviceChannels;
	this->inputBufferSizeInBytes = this->bufferSizeInNumberOfSamples * sizeAudioFormat * this->audioConfiguration.inputDeviceChannels;
}

auto RtAudioWrapper::vStartRecordingMode() -> void
{
	this->initializeRtAudioStreamParameters();

	RtAudioFormat rtAudioFormat = this->getRtAudioFormat(this->audioConfiguration);
	this->bufferSizeInNumberOfSamples = static_cast<int>(this->audioConfiguration.bufferSize);

    this->rtaudio.openStream(
		nullptr, 
		&(this->inputConfig), 
		rtAudioFormat, 
		audioConfiguration.sampleRate, 
		&(this->bufferSizeInNumberOfSamples),
		&RtAudioWrapper::callbackHelper, 
		this);

	this->setActualBufferSizes();
    this->rtaudio.startStream();
}

auto RtAudioWrapper::vStartPlaybackMode() -> void
{
	this->initializeRtAudioStreamParameters();

	RtAudioFormat rtAudioFormat = this->getRtAudioFormat(this->audioConfiguration);
	this->bufferSizeInNumberOfSamples = static_cast<int>(this->audioConfiguration.bufferSize);

	this->rtaudio.openStream(
		&(this->outputConfig),
		nullptr,
		rtAudioFormat,
		audioConfiguration.sampleRate,
		&(this->bufferSizeInNumberOfSamples),
		&RtAudioWrapper::callbackHelper,
		this);

	this->setActualBufferSizes();
	this->rtaudio.startStream();
}

auto RtAudioWrapper::vStartDuplexMode() -> void
{
	this->initializeRtAudioStreamParameters();

	RtAudioFormat rtAudioFormat = this->getRtAudioFormat(this->audioConfiguration);
	this->bufferSizeInNumberOfSamples = static_cast<int>(this->audioConfiguration.bufferSize);

	this->rtaudio.openStream(
		&(this->outputConfig),
		&(this->inputConfig),
		rtAudioFormat,
		audioConfiguration.sampleRate,
		&(this->bufferSizeInNumberOfSamples),
		&RtAudioWrapper::callbackHelper,
		this);

	this->setActualBufferSizes();
	this->rtaudio.startStream();
}


auto RtAudioWrapper::vSetConfiguration(AudioConfiguration audioConfig) -> void
{
    this->audioConfiguration = audioConfig;
    this->isAudioConfigSet = true;
}

auto RtAudioWrapper::vSuspend() -> void
{
    if (this->rtaudio.isStreamRunning())
        this->rtaudio.stopStream();
}

auto RtAudioWrapper::vStop() -> void
{
    this->suspend();
    this->rtaudio.closeStream();
	this->reset();
    this->disposeAudioProcessors();
}

auto RtAudioWrapper::vResume() -> void
{
    if (this->rtaudio.isStreamOpen() && this->rtaudio.isStreamRunning() == false)
        this->rtaudio.startStream();
}

auto RtAudioWrapper::vReset() -> void
{
    this->stop();
    this->audioConfiguration = { 0 };
    this->isAudioConfigSet = false;
    this->isExecutionReady = false;
}


auto RtAudioWrapper::initializeRtAudioStreamParameters() -> void
{
    this->inputConfig.deviceId = this->audioConfiguration.inputDeviceID;
    this->outputConfig.deviceId = this->audioConfiguration.outputDeviceID;
    this->inputConfig.nChannels = this->audioConfiguration.inputDeviceChannels;
    this->outputConfig.nChannels = this->audioConfiguration.outputDeviceChannels;

	this->inputConfig.firstChannel = 0;
	this->outputConfig.firstChannel = 0;
}

auto RtAudioWrapper::vSetDefaultAudioConfig() -> void
{
    AudioConfiguration audioConfig = { 0 };
    audioConfig.inputDeviceID = this->rtaudio.getDefaultInputDevice();
    audioConfig.outputDeviceID = this->rtaudio.getDefaultOutputDevice();

    audioConfig.inputDeviceChannels = 2;
    audioConfig.outputDeviceChannels = 2;
    

    audioConfig.audioFormat = AudioFormat::SIGNED_INT_16;
    audioConfig.sampleRate = 44100;
    audioConfig.bufferSize = BufferSizes::BUFFER_256;

    this->setConfiguration(audioConfig);
}


auto RtAudioWrapper::vPrepareForExecution() -> bool
{ }
