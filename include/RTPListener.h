/* 
 * File:   ReceiveThread.h
 * Author: daniel
 *
 * Created on May 16, 2015, 12:49 PM
 */

#ifndef RTPLISTENER_H
#define	RTPLISTENER_H

#include <thread>
#include <malloc.h>

#ifdef _WIN32
#include <winsock2.h>
//#include <cstdint>
#else
#include <sys/socket.h> // socket(), connect()
#include <unistd.h> //socklen_t
#endif

#include "RTPBuffer.h"

#include "NetworkWrapper.h"
#include "configuration.h"

class RTPListener
{
public:
    RTPListener(const sockaddr *receiveAddress, RTPBuffer *buffer, unsigned int receiveBufferSize);
    RTPListener(const RTPListener& orig);
    virtual ~RTPListener();
    
    /*!
     * Shuts down the receive-thread
     */
    void shutdown();
    
    /*!
     * Starts the receive-thread
     */
    void startUp();
private:
    /*!
     * The socket to listen on
     */
    int receiveSocket;
    RTPBuffer *buffer;
    RTPPackage *receivedPackage;
    const sockaddr *receiveAddress;
    std::thread receiveThread;
    bool threadRunning = false;
    
    /*!
     * Method called in the parallel thread, receiving packages and writing them into RTPBuffer
     */
    void runThread();
};

#endif	/* RTPLISTENER_H */
