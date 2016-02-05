#include "AudioInterfaceFactory.h"



auto AudioInterfaceFactory::getAudioHandler(AudioProcessorNames processorName) -> std::unique_ptr<AudioInterface>
{
	switch (processorName)
	{
		case AudioProcessorNames::RTAUDIOWRAPPER:
			std::unique_ptr<RtAudioWrapper> rtaudiowrapper(new RtAudioWrapper());
			return std::move(rtaudiowrapper);
		default:
			break;
	}
	return nullptr;
}
