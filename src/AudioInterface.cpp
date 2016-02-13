#include "AudioInterface.h"

AudioInterface::AudioInterface()
{
	this->workingBufferInput = new char[this->maxSizeWorkingBufferInput];
}

AudioInterface::~AudioInterface()
{
	delete[] this->workingBufferInput;
}

auto AudioInterface::startSeqeunce() -> void
{
	if (this->isAudioConfigSet == false)
		this->setDefaultAudioConfig();

	if (this->isExecutionReady == false)
		this->prepareForExecution();
}

auto AudioInterface::startRecordingMode() -> void
{
	this->startSeqeunce();
	this->vStartRecordingMode();
}

auto AudioInterface::startPlaybackMode() -> void
{
	this->startSeqeunce();
	this->vStartPlaybackMode();
}

auto AudioInterface::startDuplexMode() -> void
{
	this->startSeqeunce();
	this->vStartDuplexMode();
}

auto AudioInterface::setConfiguration(AudioConfiguration audioConfiguration) -> void
{
	this->audioConfiguration = audioConfiguration;
	this->isAudioConfigSet = true;
	this->vSetConfiguration(audioConfiguration);
}

auto AudioInterface::suspend() -> void
{
	this->vSuspend();
}

auto AudioInterface::resume() -> void
{
	this->vResume();
}

auto AudioInterface::stop() -> void
{
	this->isExecutionReady = false;
	this->vStop();
}

auto AudioInterface::reset() -> void
{
	this->stop();
	this->audioConfiguration = { 0 };
	this->isAudioConfigSet = false;
	this->vReset();
}

auto AudioInterface::setDefaultAudioConfig() -> void
{
	this->isAudioConfigSet = true;
	this->vSetDefaultAudioConfig();
}

auto AudioInterface::prepareForExecution() -> bool
{
	this->initializeAudioProcessors();
	this->vPrepareForExecution();
}

auto AudioInterface::getBufferSize() -> unsigned int
{
	return this->vGetBufferSize();
}

auto AudioInterface::setOutputStream(std::shared_ptr<std::ostream> outputStream) -> void
{
	this->outputStream = outputStream;
}

void AudioInterface::printAudioProcessorOrder() const
{
    for (const auto& processor : audioProcessors)
    {
		*outputStream << processor->getName() << std::endl;
    }
}

auto AudioInterface::addProcessor(AudioProcessor *audioProcessor) -> bool
{
    if (hasAudioProcessor(audioProcessor) == false) {
        audioProcessors.push_back(std::unique_ptr<AudioProcessor>(audioProcessor));
        return true; 
    }
    return false;
}

auto AudioInterface::removeAudioProcessor(AudioProcessor *audioProcessor) -> bool
{
    for (size_t i = 0; i < audioProcessors.size(); i++)
    {
        if ((audioProcessors.at(i))->getName() == audioProcessor->getName()) {
            audioProcessors.erase(audioProcessors.begin() + i);
            return true;
        }	
    }
    return false;
}

auto AudioInterface::removeAudioProcessor(std::string nameOfAudioProcessor) -> bool
{
    for (size_t i = 0; i < audioProcessors.size(); i++)
    {
        if (audioProcessors.at(i)->getName() == nameOfAudioProcessor)
        {
            audioProcessors.erase(audioProcessors.begin() + i);
            return true;
        }
    }
    return false;
}

auto AudioInterface::clearAudioProcessors() -> void
{
    this->audioProcessors.clear();
	this->isExecutionReady = false;
}

auto AudioInterface::initializeAudioProcessors() -> bool
{
    for (const auto& processor : audioProcessors)
    {
        bool result = processor->configure(audioConfiguration);
		if (result == false)
		{
			*outputStream << "Initializing of audio-processor '" << processor->getName() << "' failed." << std::endl;
			return false;
		}
    }
    return true;
}

auto AudioInterface::disposeAudioProcessors() -> bool
{
	for (const auto& processor : audioProcessors)
	{
		bool result = processor->cleanUp();
		if (result == false)
		{
			*outputStream << "Disposing of audio-processor '" << processor->getName() << "' failed." << std::endl;
			return false;
		}
	}
	return true;
}

auto AudioInterface::hasAudioProcessor(AudioProcessor *audioProcessor) const -> bool
{
    for (const auto& processor : audioProcessors)
    {
        if ( processor->getName() == audioProcessor->getName() )
            return true;
    }
    return false;
}

auto AudioInterface::hasAudioProcessor(std::string nameOfAudioProcessor) const -> bool
{
    for (const auto& processor : audioProcessors)
    {
        if ( processor->getName() == nameOfAudioProcessor )
            return true;
    }
    return false;
}

auto AudioInterface::getAudioConfiguration()->AudioConfiguration
{
    return this->audioConfiguration;
}

auto AudioInterface::processAudioOutput(void *outputBuffer, unsigned int outputBufferByteSize, void *userData) -> void
{
    unsigned int bufferSize = outputBufferByteSize;
    for (unsigned int i = audioProcessors.size(); i > 0; i--)
    {
        bufferSize = audioProcessors.at(i-1)->processOutputData(outputBuffer, bufferSize, userData);
    }
}
 
auto AudioInterface::processAudioInput(void *inputBuffer, unsigned int inputBufferByteSize, void *userData) -> void
{
	memcpy(this->workingBufferInput, inputBuffer, inputBufferByteSize);
    unsigned int bufferSize = inputBufferByteSize;
    for (unsigned int i = 0; i < audioProcessors.size(); i++)
    {
        bufferSize = audioProcessors.at(i)->processInputData(this->workingBufferInput, bufferSize, userData);
    }
}

auto AudioInterface::getIsAudioConfigSet() const -> bool
{
    return this->isAudioConfigSet;
}