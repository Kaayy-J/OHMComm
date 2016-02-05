#ifndef AUDIOPROCESSOR_H
#define	AUDIOPROCESSOR_H

#include <string>
#include "configuration.h"

/*!
 * Abstract super-type for all classes used for intermediate handling of the input/output stream.
 *
 * An implementation of this class may be used to encode/decode, filter or compress/decompress the input- and output-streams.
 *
 * Processors can be chained, i.e. an output stream can be filtered -> encoded -> compressed.
 * The appertaining input-stream then will be decompressed -> decoded.
 */
class AudioProcessor
{
public:
    AudioProcessor(const std::string name);

    virtual ~AudioProcessor()
    { }

    /*!
     * \return the unique name of this audio-processor
     */
    const std::string getName() const;

    /*!
     * Overwrite this method, if this AudioProcessor needs configuration
     * 
     * NOTE: the audio-configuration in the configuration-mode is not valid, use the audioConfig-parameter!
     *
     * \param audioConfig The valid AudioConfiguration
     * \param configMode The ConfigurationMode to retrieve custom configuration-values from
     *
     */
    virtual bool configure(const AudioConfiguration& audioConfig);

    /*!
     * Counterpart of configure(). This method is called, when the object is not needed any longer.
     */
    virtual bool cleanUp();


    /*!
     * The actual processing methods. processInputData is the counterpart of processInputData
     *
     * \param inputBuffer The buffer to read/write the data from
     *
     * \param inputBufferByteSize The actual number of valid bytes stored in the buffer
     *
     * \param userData Any additional data
     *
     * \return the new number of valid bytes in the inputBuffer, maximal StreamData#maxBufferSize
     */
    virtual unsigned int processInputData(void *inputBuffer, const unsigned int inputBufferByteSize, void *userData) = 0;

    /*!
     * The actual processing methods. processInputData is the counterpart of processInputData
     *
     * \param outputBuffer The buffer to read/write the data from
     *
     * \param outputBufferByteSize The actual number of valid bytes stored in the buffer
     *
     * \param userData Any additional data
     *
     * \return the new number of valid bytes in the outputBuffer, maximal StreamData#maxBufferSize
     */
    virtual unsigned int processOutputData(void *outputBuffer, const unsigned int outputBufferByteSize, void *userData) = 0;
private:
    const std::string name;
};



#endif
