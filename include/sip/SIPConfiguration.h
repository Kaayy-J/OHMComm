/* 
 * File:   SIPConfiguration.h
 * Author: daniel
 *
 * Created on December 2, 2015, 1:06 PM
 */

#ifndef SIPCONFIGURATION_H
#define	SIPCONFIGURATION_H

#include "ConfigurationMode.h"
#include "SIPHandler.h"

class SIPConfiguration : public ConfigurationMode
{
public:
    SIPConfiguration(const NetworkConfiguration& sipConfig, bool profileProcessors = false, const std::string& logFile = "");
    
    ~SIPConfiguration();
    
    bool runConfiguration();
    
    const std::string getCustomConfiguration(const std::string key, const std::string message, const std::string defaultValue) const;
    const int getCustomConfiguration(const std::string key, const std::string message, const int defaultValue) const;
    const bool getCustomConfiguration(const std::string key, const std::string message, const bool defaultValue) const;
    const bool isCustomConfigurationSet(const std::string key, const std::string message) const;

private:
    SIPHandler handler;

    void setAudioConfig(const MediaDescription& media);
    
    friend class SIPHandler;
};

#endif	/* SIPCONFIGURATION_H */

