/*
 * File:   configuration.h
 * Author: daniel, jonas
 *
 * Created on April 1, 2015, 5:37 PM
 */

#ifndef CONFIGURATION_H
#define	CONFIGURATION_H

#include <string>

//the port should be a number greater than 1024
static const int DEFAULT_NETWORK_PORT = 12345;
//The program's current version as string
static const std::string OHMCOMM_VERSION = "0.7b";


struct NetworkConfiguration
{
    //we don't need local IP, because we listen on any address (of the local device)
    //Local port
    unsigned short localPort;
    //Remote IP address
    std::string remoteIPAddress;
    //Remote port
    unsigned short remotePort;
};

enum class AudioFormat 
{
	SIGNED_INT_8 = 1,
	SIGNED_INT_16 = 2,
	SIGNED_INT_24 = 3,
	SIGNED_INT_32 = 4,
	FLOAT_32 = 4,
	FLOAT_64 = 6
};

enum class BufferSizes
{
	BUFFER_64 = 64,
	BUFFER_128 = 128,
	BUFFER_256 = 256,
	BUFFER_512 = 512,
	BUFFER_1024 = 1024,
	BUFFER_2048 = 2048,
	BUFFER_4096 = 4096
};

struct AudioConfiguration
{
    // the library-specific ID of the audio output-device to use
    unsigned int outputDeviceID;
    // the library-specific ID of the input-device
    unsigned int inputDeviceID;

    // number of maximum output channels supported by the output device
    unsigned int outputDeviceChannels;
    // number of maximum input channels supported by the input device
    unsigned int inputDeviceChannels;

    // sample rate of the audio device
	unsigned int sampleRate;

	BufferSizes bufferSize;
    
	AudioFormat audioFormat;

    friend bool operator==(const AudioConfiguration& lhs, const AudioConfiguration& rhs)
    {
        if (lhs.outputDeviceID != rhs.outputDeviceID)
            return false;
        if (lhs.inputDeviceID != rhs.inputDeviceID)
            return false;
        if (lhs.outputDeviceChannels != rhs.outputDeviceChannels)
            return false;
        if (lhs.inputDeviceChannels != rhs.inputDeviceChannels)
            return false;
        if (lhs.audioFormatFlag != rhs.audioFormatFlag)
            return false;
        if (lhs.sampleRate != rhs.sampleRate)
            return false;
        if (lhs.framesPerPackage != rhs.framesPerPackage)
            return false;

        return true;
    }
};

#endif	/* CONFIGURATION_H */

