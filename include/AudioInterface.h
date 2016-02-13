#ifndef AUDIOINTERFACE_H
#define	AUDIOINTERFACE_H

#include "AudioProcessor.h"
#include <vector>
#include <memory>
#include <iostream>


/*!
 * Base class for Audio framework.
 * Implementations of this class wrap a specific audio library.
 */
class AudioInterface
{
public:
	AudioInterface();
    virtual ~AudioInterface();

	/*!
	 * Start processing audio input from the microphone.
	 */
	auto startRecordingMode() -> void;

	/*!
	 * Start processing audio output to the speakers.
	 */
    auto startPlaybackMode() -> void;

	/*!
	 * Start processing audio input and output.
	 */
    auto startDuplexMode() -> void;

	/*!
	 * Set an AudioConfiguration.
	 */
    auto setConfiguration(AudioConfiguration audioConfiguration) -> void;

	/*!
	 * Suspend any processing.
	 */
    auto suspend() -> void;

	/*!
	 * Resume processing.
	 */
	auto resume() -> void;

	/*!
	 * Stop processing. Cannot be resumed (Should be used on shutdown).
	 */
	auto stop() -> void;

	/*!
	 * Calls stop() and reset the audio configuration.
	 */
	auto reset() -> void;

	/*!
	 * Set default AudioConfiguration. 
	 * This method is called internally, if setConfiguration() was not called.
	 */
	auto setDefaultAudioConfig() -> void;

    /*!
     * Called when an AudioConfiguration is set and before the audio processing starts.
     */
    auto prepareForExecution() -> bool;

	/*!
	 * Return the actual buffer size.
	 */
    auto getBufferSize() -> unsigned int;

	/*!
	 * Prints all included AudioProcessors in the processing order.
	 */
	auto printAudioProcessorOrder() const -> void;

	/*!
	 * Add an AudioProcessor to process chain.
	 * \param AudioProcessor to add.
	 * \return True if AudioProcessor was added successfully.
	 */
    auto addProcessor(AudioProcessor *audioProcessor) -> bool;

	/*!
	 * Remove the given AudioProcessor from the process chain.
	 * \param AudioProcessor to remove.
	 * \return True if AudioProcessor was removed successfully.
	 */
    auto removeAudioProcessor(AudioProcessor *audioProcessor) -> bool;

	/*!
	 * Remove the given AudioProcessor from the process chain.
	 * \param The name of the AudioProcessor to remove.
	 * \return True if AudioProcessor was removed successfully.
	 */
    auto removeAudioProcessor(std::string nameOfAudioProcessor) -> bool;

	/*!
	 * Clear the process chain - removes all AudioProcessors.
	 */
    auto clearAudioProcessors() -> void;

	/*!
	 * Return wheater the given AudioProcessor is in the process chain.
	 */
    auto hasAudioProcessor(AudioProcessor *audioProcessor) const -> bool;

	/*!
	 * Return wheater the given AudioProcessor is in the process chain.
	 */
    auto hasAudioProcessor(std::string nameOfAudioProcessor) const -> bool;

	/*!
	 * Return the current AudioConfiguration.
	 */
    auto getAudioConfiguration() -> AudioConfiguration;

	/*!
	 * Return the status wheater an AudioConfiguration was set.
	 */
    auto getIsAudioConfigSet() const -> bool;

	/*!
	 * Set a standard OutputStream.
	 */
	auto setOutputStream(std::shared_ptr<std::ostream> outputStream) -> void;

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
	 * Is called when the speakers are ready to play audio data.
	 */
    auto processAudioOutput(void *outputBuffer, unsigned int outputBufferByteSize, void *userData) -> void;;

	/*!
	 * Is called when data is available from the microphone.
	 */
	auto processAudioInput(void *inputBuffer, unsigned int inputBufferByteSize, void *userData) -> void;;
    
	/*!
	 * Is called before the processing starts. 
	 * Intializes some basic settings.
	 */
	auto startSeqeunce() -> void;

	/*!
	 * Call initialize() method of all AudioProcessors in the process chain.
	 * \return True if all AudioProcessor was initialized successfully.
	 */
    auto initializeAudioProcessors() -> bool;

	/*!
	 * Call dispose() method of all AudioProcessors in the process chain.
	 * \return True if all AudioProcessor was disposed successfully.
	 */
    auto disposeAudioProcessors() -> bool;

	bool isAudioConfigSet { false };
	bool isExecutionReady { false };

	void* workingBufferInput { nullptr };
	int maxSizeWorkingBufferInput{ 25000 };	// TODO This should be dynamic

	AudioConfiguration audioConfiguration { 0 };
	
	std::vector< std::unique_ptr<AudioProcessor> > audioProcessors;

	std::shared_ptr< std::ostream > outputStream{ &std::cout };
};

#endif
