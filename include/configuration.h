/* 
 * File:   configuration.h
 * Author: daniel, jonas
 *
 * Created on April 1, 2015, 5:37 PM
 */

#ifndef CONFIGURATION_H
#define	CONFIGURATION_H

#ifdef __linux__
#include <sys/socket.h>
#include <arpa/inet.h>
#else
#include <winsock2.h>
#endif

#include <string>


struct NetworkConfiguration {
    
    /*!
     * the remote address
     */
    sockaddr remoteAddr;
    
    /*!
     * the local address
     */
    sockaddr localAddr;
    
    /*!
     * The type of socket (Stream, Datagram, ...)
     * 
     * 
     */
    unsigned int socketType;
    
    /*!
     * The protocol used
     * 
     * See netinet/in.h (Linux) or winsock2.h (Windows) for available protocols
     */
    unsigned int protocol;
    
    /*!
     * the socket-ID.
     * 
     * Unlike the other values, this is set by the NetworkWrapper
     */
    int socket;
    
};

struct AudioConfiguration {
    
	// Output Audio Device ID
	unsigned int OutputDeviceID;

	// Input Audio Device ID
	unsigned int InputDeviceID;

	// The Name of the Output Audio Device
	std::string OutputDeviceName;
	
	// The Name of the Input Audio Device
	std::string InputDeviceName;

	// Number of maximum output Channels supported by Device
	unsigned int OutputDeviceChannels;

	// Number of maximum input Channels supported by Device
	unsigned int InputDeviceChannels;





	/*!
     * The audio format
     * 
     * See RtAudioFormat in RtAudio.h for more details
     */
    unsigned long audioFormat;
    
};

//Configurations are declared in OhmComm.cpp
extern NetworkConfiguration networkConfiguration;
extern AudioConfiguration audioConfiguration;

#endif	/* CONFIGURATION_H */

