#ifndef AUDIOPROCESSOR_H
#define	AUDIOPROCESSOR_H

#include <string>
#include <vector>
#include "Statistics.h"

#include "configuration.h"

/*!
 * This AudioProcessor supports any buffer-length
 */
const int BUFFER_SIZE_ANY = -1;

/*!
 * Information about the stream to be passed to the process-methods of AudioProcessor.
 * Details of the values are specified by the used AudioIO-implementation
 */
struct StreamData
{
    /*!
     * The number of frames in the buffer
     */
    unsigned int nBufferFrames;

    /*!
     * The current time in the stream (i.e. elasped microseconds)
     */
    unsigned int streamTime;
    
    /*!
     * The maximum number of bytes to be stored in the input/output-buffer
     */
    unsigned int maxBufferSize;
};

/*!
 * Abstract supertype for all classes used for intermediate handling of the input/output stream.
 * 
 * An implementation of this class may be used to encode/decode, filter or compress/decompress the input- and output-streams.
 * 
 * Processors can be chained, i.e. an output stream can be filtered -> encoded -> compressed.
 * The appertaining input-stream then will be decompressed -> decoded.
 */
class AudioProcessor
{
public:
    AudioProcessor(std::string name);

    const std::string getName();
    void setName(std::string name);
    
    /*!
     * This method returns the OR'ed flags of the supported audio-formats
     * 
     * Available flags are all AUDIO_FORMAT_XXX from AudioConfiguration
     * 
     * \return The supported audio-formats
     */
    virtual unsigned int getSupportedAudioFormats() = 0;
    
    /*!
     * This method returns the OR'ed flags of the supported sample-rates
     * 
     * Available flags are all SAMPLE_RATE_XXX from AudioConfiguration
     * 
     * \return The supported sample-rates
     */
    virtual unsigned int getSupportedSampleRates() = 0;
    
    /*!
     * This method returns all supported buffer-sizes for the given sample-rate.
     * The buffer-size specified here is the number of samples buffered to be processed together in one processXXX(...) call.
     * 
     * Any AudioProcessor which supports an arbitrary buffer-size should include BUFFER_SIZE_ANY at lowest priority.
     * 
     * For example a processor supporting all buffer-sizes without any preferences returns {BUFFER_SIZE_ANY}. 
     * Any processor preferring specific buffer-sizes returns them in descending priority, e.g. {512, 256, BUFFER_SIZE_ANY}
     * prefers 512 then 256 and if none of these can be matched, any other size.
     * 
     * \param sampleRate The chosen sample-rate
     * 
     * \return A list of buffer-sizes sorted in descending priority
     */
    virtual std::vector<int> getSupportedBufferSizes(unsigned int sampleRate) = 0;

    /*!
     * Overwrite this method, if this AudioProcessor needs configuration
     * 
     * \param audioConfig The valid AudioConfiguration
     * 
     * Any implementation of this method can use the methods from UserInput.h
     */
    virtual bool configure(AudioConfiguration audioConfig);

    /*
     * The actual processing methods. processInputData is the counterpart of processInputData 
     * 
     * \param inputBuffer The buffer to read/write the data from
     * 
     * \param inputBufferByteSize The actual number of valid bytes stored in the buffer
     * 
     * \param userData A StreamData-object storing additional information about the stream
     * 
     * \return the new number of valid bytes in the inputBuffer, maximal StreamData#maxBufferSize
     */
    virtual unsigned int processInputData(void *inputBuffer, const unsigned int inputBufferByteSize, StreamData *userData) = 0;
    
    /*
     * The actual processing methods. processInputData is the counterpart of processInputData 
     * 
     * \param outputBuffer The buffer to read/write the data from
     * 
     * \param outputBufferByteSize The actual number of valid bytes stored in the buffer
     * 
     * \param userData A StreamData-object storing additional information about the stream
     * 
     * \return the new number of valid bytes in the outputBuffer, maximal StreamData#maxBufferSize
     */
    virtual unsigned int processOutputData(void *outputBuffer, const unsigned int outputBufferByteSize, StreamData *userData) = 0;
private:
    std::string name;
};



#endif