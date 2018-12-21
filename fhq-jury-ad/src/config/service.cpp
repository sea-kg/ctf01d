#include "service.h"

Service::Service(){
    m_nScriptWaitInSec = 10;
    m_bEnabled = true;
    m_nTimeSleepBetweenRunScriptsInSec = 10;
}

// ----------------------------------------------------------------------

void Service::setId(const std::string &sServiceID){
    m_sID = sServiceID;
}

// ----------------------------------------------------------------------

std::string Service::id() const {
    return m_sID;
}

// ----------------------------------------------------------------------

void Service::setName(const std::string &sName){
    m_sName = sName;
}

// ----------------------------------------------------------------------

std::string Service::name() const {
    return m_sName;
}

// ----------------------------------------------------------------------

void Service::setScriptPath(const std::string &sScriptPath){
    m_sScriptPath = sScriptPath;
}

// ----------------------------------------------------------------------

std::string Service::scriptPath() const {
    return m_sScriptPath;
}

// ----------------------------------------------------------------------

void Service::setScriptDir(const std::string &sScriptDir) {
    m_sScriptDir = sScriptDir;
}

// ----------------------------------------------------------------------

std::string Service::scriptDir() const {
    return m_sScriptDir;
}

// ----------------------------------------------------------------------

void Service::setEnabled(bool bEnabled){
    m_bEnabled = bEnabled;
}

// ----------------------------------------------------------------------

bool Service::isEnabled() const {
    return m_bEnabled;
}

// ----------------------------------------------------------------------

void Service::setScriptWaitInSec(int nSec){
    m_nScriptWaitInSec = nSec;
    if(m_nScriptWaitInSec < 1){
        m_nScriptWaitInSec = 10;
    }
}

// ----------------------------------------------------------------------

int Service::scriptWaitInSec() const {
    return m_nScriptWaitInSec;
}

// ----------------------------------------------------------------------

void Service::setTimeSleepBetweenRunScriptsInSec(int nSec){
    m_nTimeSleepBetweenRunScriptsInSec = nSec;
    if(m_nTimeSleepBetweenRunScriptsInSec < 1){
        m_nTimeSleepBetweenRunScriptsInSec = 10;
    }
}

// ----------------------------------------------------------------------

int Service::timeSleepBetweenRunScriptsInSec() const {
    return m_nTimeSleepBetweenRunScriptsInSec;
}

// ----------------------------------------------------------------------