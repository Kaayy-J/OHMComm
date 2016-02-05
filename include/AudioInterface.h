#ifndef AUDIOINTERFACE_H
#define	AUDIOINTERFACE_H

#include "AudioProcessor.h"
#include <vector>
#include <memory>
#include <iostream>

/*!
 * Base class for Audio framework.
 *
 * Implementations of this class wrap a specific audio library
 */
class AudioInterface
{
public:
	AudioInterface();
    virtual ~AudioInterface();

	/*!
	 * Starts processing audio input from the microphone
	 */
	auto startRecordingMode() -> void;

	/*!
	 * Starts processing audio output to the speakers
	 */
    auto startPlaybackMode() -> void;

	/*!
	 * Starts processing audio input and output
	 */
    auto startDuplexMode() -> void;

	/*!
	 * Sets up an AudioConfiguration
	 */
    auto setConfiguration(AudioConfiguration audioConfiguration) -> void;

	/*!
	 * Suspends any processing
	 */
    auto suspend() -> void;

	/*!
	 * Resumes a suspended state
	 */
	auto resume() -> void;

	/*!
	 * Stops the communication process at all. The object cannot be resumed (Should be used on shutdown)
	 */
	auto stop() -> void;

	/*!
	 * Stops and resets the audio configuration (Instance can be reused with a new audio configuration)
	 */
	auto reset() -> void;

	/*!
	 * Loads a default audio configuration
	 */
	auto setDefaultAudioConfig() -> void;

    /*!
     * Will be called before the actual audio processing starts
     */
    auto prepareForExecution() -> bool;

	/*!
	 * Returns the actual buffer size, which can be different than in the AudioConfiguration
	 */
    auto getBufferSize() -> unsigned int;

	/*!
	 * Prints all included AudioProcessors in the processing order
	 *
	 * \param outputStream The outputStream for printing (default is std::cout)
	 */
	auto printAudioProcessorOrder(std::ostream& outputStream = std::cout) const -> void;

	/*!
	 * Adds an AudioProcessor to process chain
	 *
	 * \param the AudioProcessor to add
	 *
	 * \return true if it was added successfully
	 */
    auto addProcessor(AudioProcessor *audioProcessor) -> bool;

	/*!
	 * Removes the given AudioProcessor from the process chain
	 *
	 * \param The AudioProcessor to remove
	 *
	 * \return true if the AudioProcessor was removed successfully
	 */
    auto removeAudioProcessor(AudioProcessor *audioProcessor) -> bool;

	/*!
	 * Removes the given AudioProcessor from the process chain
	 *
	 * \param The name of the AudioProcessor to remove
	 *
	 * \return true if the AudioProcessor was removed successfully
	 */
    auto removeAudioProcessor(std::string nameOfAudioProcessor) -> bool;

	/*!
	 * Clears the process chain (removes all AudioProcessor's)
	 */
    auto clearAudioProcessors() -> void;

	/*!
	 * Returns wheater the given AudioProcessor is in the process chain
	 *
	 * \param audioProcessor AudioProcessor object to check
	 */
    auto hasAudioProcessor(AudioProcessor *audioProcessor) const -> bool;

	/*!
	 * Returns wheater the given AudioProcessor is in the process chain
	 *
	 * \param The name of the AudioProcessor
	 */
    auto hasAudioProcessor(std::string nameOfAudioProcessor) const -> bool;

	/*!
	 * Returns the current AudioConfiguration
	 */
    auto getAudioConfiguration() -> AudioConfiguration;

	/*!
	 * Returns wheater the instance has an AudioConfiguration
	 */
    auto getIsAudioConfigSet() const -> bool;

	/*!
	 * Returns wheater instance is ready for execution
	 */
    auto getIsExecutionReady() const -> bool;

	/*!
	 * Sets a standard OutputStream 
	 */
	auto setOutputStream(std::ostream* outputStream) -> void;

protected:
	virtual auto vStartRecordingMode() -> void = 0;

	virtual auto vStartPlaybackMode() -> void = 0;

	virtual auto vStartDuplexMode() -> void = 0;

	virtual auto vSetConfiguration(AudioConfiguration audioConfiguration) -> void = 0;

	virtual auto vSuspend() -> void = 0;

	virtual auto vResume() -> void = 0;

	virtual auto vStop() -> void = 0;

	virtual auto vReset() -> void = 0;

	virtual auto vSetDefaultAudioConfig() -> void = 0;

	virtual auto vPrepareForExecution() -> bool = 0;

	virtual auto vGetBufferSize() -> unsigned int = 0;


	/*!
	 * Gets called when the buffer of the speakers is empty
	 */
    auto processAudioOutput(void *outputBuffer, const unsigned int &outputBufferByteSize, void *userData) -> void;;

	/*!
	 * Gets called when the buffer of the microphone is full
	 */
	auto processAudioInput(void *inputBuffer, const unsigned int &inputBufferByteSize, void *userData) -> void;;
    
	/*!
	 * This method is called before the processing starts. 
	 * Intializes some basic settings.
	 */
	auto startSeqeunce() -> void;

	/*!
	 * Calls the configure method of all AudioProcessor in the process chain
	 *
	 * \return true if all AudioProcessor was configured successfully
	 */
    auto configureAudioProcessors() -> bool;

	/*!
	 * Calls the cleanUp method of all AudioProcessor in the process chain
	 *
	 * \return true if all AudioProcessor was cleaned up successfully
	 */
    auto cleanUpAudioProcessors() -> bool;

	bool isAudioConfigSet { false };
	bool isExecutionReady { false };
	AudioConfiguration audioConfiguration { 0 };
	std::ostream* outputStream { &std::cout };
	std::vector< std::unique_ptr<AudioProcessor> > audioProcessors;
};

#endif
