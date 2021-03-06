#ifndef AUDIOINTERFACEFACTORY
#define	AUDIOINTERFACEFACTORY

#include "AudioInterface.h"
#include "RTAudioWrapper.h"


//Initialize names
enum class AudioProcessorNames { RTAUDIOWRAPPER };

/*!
 * Factory-class to provide an audio-handler object without needing to know the details of the implementation.
 */
class AudioInterfaceFactory
{
public:
    /*! Name for the RtAudioWrapper */
    static const std::string RTAUDIO_WRAPPER;

    /*!
     * \param name The name of the audio-handler to create
     * \param audioConfig The audio-configuration to initialize the handler with
     * \return an unique-pointer to the created handler
     * \throw an invalid-argument exception, if there was no audio-handler for the given name
     */
    static auto getAudioHandler(const std::string name, const AudioConfiguration& audioConfig)->std::unique_ptr<AudioInterface>;
    /*!
     * \param name The name of the audio-handler to create
     * \return an unique-pointer to the created handler
     * \throw an invalid-argument exception, if there was no audio-handler for the given name
     */
    static auto getAudioHandler(const std::string name) -> std::unique_ptr<AudioInterface>;

    /*!
     * \return a list of all available audio-handler names
     */
    static inline const std::vector<std::string> getAudioHandlerNames()
    {
        return allAudioHandlerNames;
    }

    /*!
     * \return the name of the default audio-handler
     */
    static inline const std::string getDefaultAudioHandlerName()
    {
        return allAudioHandlerNames[0];
    }
    
private:
    //! A list of all registered audio-handlers, the first audio-handler is used as default
    static const std::vector<std::string>allAudioHandlerNames;
};

#endif
