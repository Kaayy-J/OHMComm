# OHMComm-Light
OHMComm-Light is fork of OHMComm, which it an IT-Project at the university Georg Simon Ohm in Nuremberg/Germany. The goal of the project is to creating platform-independent communication framework in C++. Fore more information about this project visit the [offical site](https://github.com/doe300/OHMComm).

The target of OHMComm-Light is to remove some functions and improve the architecture in order to reduce the complexity. 


-----------------------------

This program is currently developed under linux (Fedora 22) and Windows (7/8) and tested under Mac OS X.

OHMComm is based upon [RtAudio](http://www.music.mcgill.ca/~gary/rtaudio/) and therefore supports all audio-libraries supported by RtAudio (see [RtAudio API-Notes](http://www.music.mcgill.ca/~gary/rtaudio/apinotes.html)). The audio-data are (optionally but highly recommended) encoded with the included [opus-codec](http://www.opus-codec.org/).

## Build it

#### Under Linux/Mac OS
You will need a compiler with full C++11 support and cmake in version 2.6 or higher.
Additionally you will require the development-header for the supported audio-library of your choice,
for *ALSA* this would be `libasound-dev` on Debian-based systems and `alsa-lib-devel` on Fedora.

	$ cd <project-directory>
	$ cmake -G "Unix Makefiles" ./CMakeLists.txt
	$ make OHMCommLib	# To build the library
	$ make OHMComm		# To build the executable

This will build the library/executable into `<project-directory>/build/` by default.
## Run it

#### Under Linux/Mac OS/Windows

	$ cd <executable-directory>	
	$ ./OHMComm		# To run in interactive mode
	$ ./OHMComm	-h	# Prints all command-line arguments
	$ ./OHMComm path/to/config-file	# Runs in file-configuration mode
