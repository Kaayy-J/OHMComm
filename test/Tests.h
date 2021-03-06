#ifndef OHMCOMMTESTS
#define	OHMCOMMTESTS

#include "cpptest.h"
#include "AudioHandlerFactory.h"
#include "TestAudioProcessors.h"
#include "TestUserInput.h"
#include "TestParameters.h"
#include "TestConfigurationModes.h"
#include "TestNetworkWrappers.h"
#include "rtp/TestRTP.h"
#include "rtp/TestRTCP.h"
#include "rtp/TestRTPBuffer.h"

#include <iostream>
#include <fstream>

class TestAudioIO : public Test::Suite {
    void testAudioHandlerInstances(); // getNewAudioIO(..)
    void testAudioProcessorInterface(); // add, remove, reset
public:
    TestAudioIO();
};

#endif