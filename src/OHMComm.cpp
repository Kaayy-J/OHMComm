/* 
 * File:   OHMComm.cpp
 * Author: daniel, jonas
 *
 * Created on March 29, 2015, 1:49 PM
 */

#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>
#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h> // sockaddr_in
#endif

#include "../include/configuration.h"

//dependencies for rtaudio
#include "../lib/rtaudio-4.1.1/RtAudio.h"
#pragma comment(lib, "lib/rtaudio_static.lib") //Automated Linking to rtaudio lib (Todo: relative path for cmake correct?)

//Declare Configurations
NetworkConfiguration networkConfiguration;
AudioConfiguration audioConfiguration;

using namespace std;

//conversion method
//returns -1 on error
inline int convertToInt(const std::string& s)
{
    std::istringstream i(s);
    int x;
    if (!(i >> x))
        return -1;
    return x;
}

inline void createAddress(sockaddr *addr, int addressType, std::string ipString, int port)
{
    sockaddr_in *address = reinterpret_cast<sockaddr_in*>(addr);
    //IPv4 or IPv6
    address->sin_family = addressType;
    
    if(ipString == "")
    {
        //listen on any address
        address->sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
        //the remote IP address - inet_addr() is a convenience-method to convert from aaa.bbb.ccc.ddd (octal representation) to a 4-byte unsigned long
        address->sin_addr.s_addr = inet_addr(ipString.c_str());
    }
    //network byte order is big-endian, so we must convert the port-number
    address->sin_port = htons(port);
}

void configureNetwork()
{
    string ipString;
    string destPortString, localPortString;
    string protocolString;
    
    cout << "Network configuration:" << endl;
    
    //1. remote address
    cout << "1. Input destination IP address: ";
    cin >> ipString;
    
    //2. remote and local ports
    cout << "2. Input destination port: ";
    cin >> destPortString;
    cout << "3. Input local port: ";
    cin >> localPortString;
    
    //3. protocol
    cout << "4. Choose protocol (TCP/UDP) [defaults to UDP]: ";
    cin >> protocolString;
    
    //4. parse arguments
    int destPort = convertToInt(destPortString);
    int localPort = convertToInt(localPortString);
    
    if(protocolString == "TCP")
    {
        // SOCK_STREAM - creating a stream-socket
        // IPPROTO_TCP - use TCP/IP protocol
        networkConfiguration.socketType = SOCK_STREAM;
        networkConfiguration.protocol = IPPROTO_TCP;
        cout << "-> Using TCP (stream-socket)" << endl;
    }
    else
    {
        // SOCK_DGRAM - creating a datagram-socket
        // IPPROTO_UDP - use UDP protocol
        networkConfiguration.socketType = SOCK_DGRAM;
        networkConfiguration.protocol = IPPROTO_UDP;
        cout << "-> Using UDP (datagram-socket)" << endl;
    }
    
    //5. create addresses
    //local address
    createAddress(&networkConfiguration.localAddr, AF_INET, "", localPort);
    //remote address
    createAddress(&networkConfiguration.remoteAddr, AF_INET, ipString, destPort);
    
    cout << "Network configured!" << endl;
    cout << endl;
}

void configureAudioDevices()
{
	cout << endl;
	cout << "+++Audio Device configuration+++" << endl;
	cout << endl;

	RtAudio AudioDevices;
	// Determine the number of available Audio Devices 
	unsigned int availableAudioDevices = AudioDevices.getDeviceCount();

	cout << "Available Audio Devices: " << endl;
	cout << endl;
	// printing available Audio Devices
	RtAudio::DeviceInfo DeviceInfo;

	unsigned int DefaultOutputDeviceID;
	unsigned int DefaultInputDeviceID;

	for (unsigned int i = 0 ; i < availableAudioDevices ; i++)
	{
		DeviceInfo = AudioDevices.getDeviceInfo(i);
		if (DeviceInfo.probed == true) //Audio Device successfully probed
		{
			cout << "Device ID: " << i << endl;
			cout << "Device Name = " << DeviceInfo.name << endl;
			cout << "Maximum output channels = " << DeviceInfo.outputChannels << endl;
			cout << "Maximum input channels = " << DeviceInfo.inputChannels << endl;
			cout << "Maximum duplex channels = " << DeviceInfo.duplexChannels << endl;
			cout << "Default output: ";
			if (DeviceInfo.isDefaultOutput == true)
			{
				cout << "Yes" << endl;
				DefaultOutputDeviceID = i;

			}
			else
			{
				cout << "No" << endl;
			}
			cout << "Default input: ";
			if (DeviceInfo.isDefaultInput == true)
			{
				cout << "Yes" << endl;
				DefaultInputDeviceID = i;
			}
			else
			{
				cout << "No" << endl;
			}
			cout << endl;
		}
	}

	unsigned int OutputDeviceID;
	
	//Choose output Device
	cout << "Choose output Device ID [default is " << DefaultOutputDeviceID << " ]: ";
	cin >> OutputDeviceID;

	RtAudio::DeviceInfo OutputDeviceInfo = AudioDevices.getDeviceInfo(OutputDeviceID);

	//Configure ID of the Output Audio Device
	audioConfiguration.OutputDeviceID = OutputDeviceID;
	cout << "-> Using output Device ID: " << audioConfiguration.OutputDeviceID << endl;

	//Configure Name of the Output Audio Device
	audioConfiguration.OutputDeviceName = OutputDeviceInfo.name;
	cout << "-> Using output Device Name: " << audioConfiguration.OutputDeviceName << endl;

	//Configure Number of Maximum output Channels
	audioConfiguration.OutputDeviceChannels = OutputDeviceInfo.outputChannels;
	cout << "-> Number of maximum output Channels supported from this Device: " << audioConfiguration.OutputDeviceChannels << endl;

	unsigned int InputDeviceID;

	//Choose input Device
	cout << "Choose input Device ID [default is " << DefaultInputDeviceID << " ]: ";
	cin >> InputDeviceID;

	RtAudio::DeviceInfo InputDeviceInfo = AudioDevices.getDeviceInfo(InputDeviceID);

	//Configure ID of the Input Audio Device
	audioConfiguration.InputDeviceID = InputDeviceID;
	cout << "-> Using input Device ID " << audioConfiguration.InputDeviceID << endl;

	//Configure Name of the Input Audio Device
	audioConfiguration.InputDeviceName = InputDeviceInfo.name;
	cout << "-> Using input Device Name: " << audioConfiguration.InputDeviceName << endl;

	//Configure Number of Maximum output Channels
	audioConfiguration.InputDeviceChannels = InputDeviceInfo.inputChannels;
	cout << "-> Number of maximum input Channels supported from this Device: " << audioConfiguration.InputDeviceChannels << endl;

	//Todo: configure Sample Rate

}


int main(int argc, char** argv)
{
    ////
    // Configuration
    ////

    //1. network connection
    configureNetwork();
    
    //2. audio devices
	configureAudioDevices();
    //2.1 audio input
    //2.2 audio output
    //2.3 audio configuration (bit rate, ...)
    
    //3. processors
    //3.1 filters
    //3.2 codecs
    //3.3 compressors
    
    ////
    // Initialize
    ////
    
    //1. RTP
    //2. AudioProcessors
    //3. RTAudio
    
    ////
    // Running
    ////
    
    //start loop
    
    return 0;
}

