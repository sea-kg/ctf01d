#include "model_service_conf.h"

ModelServiceConf::ModelServiceConf(){
    m_nScriptWaitInSec = 10;
    m_bEnabled = true;
    m_nTimeSleepBetweenRunScriptsInSec = 10;
}

// ----------------------------------------------------------------------

void ModelServiceConf::setId(const std::string &sServiceID){
    m_sID = sServiceID;
}

// ----------------------------------------------------------------------

std::string ModelServiceConf::id() const {
    return m_sID;
}

// ----------------------------------------------------------------------

void ModelServiceConf::setName(const std::string &sName){
    m_sName = sName;
}

// ----------------------------------------------------------------------

std::string ModelServiceConf::name() const {
    return m_sName;
}

// ----------------------------------------------------------------------

void ModelServiceConf::setScriptPath(const std::string &sScriptPath){
    m_sScriptPath = sScriptPath;
}

// ----------------------------------------------------------------------

std::string ModelServiceConf::scriptPath() const {
    return m_sScriptPath;
}

// ----------------------------------------------------------------------

void ModelServiceConf::setScriptDir(const std::string &sScriptDir) {
    m_sScriptDir = sScriptDir;
}

// ----------------------------------------------------------------------

std::string ModelServiceConf::scriptDir() const {
    return m_sScriptDir;
}

// ----------------------------------------------------------------------

void ModelServiceConf::setEnabled(bool bEnabled){
    m_bEnabled = bEnabled;
}

// ----------------------------------------------------------------------

bool ModelServiceConf::isEnabled() const {
    return m_bEnabled;
}

// ----------------------------------------------------------------------

void ModelServiceConf::setScriptWaitInSec(int nSec){
    m_nScriptWaitInSec = nSec;
    if(m_nScriptWaitInSec < 1){
        m_nScriptWaitInSec = 10;
    }
}

// ----------------------------------------------------------------------

int ModelServiceConf::scriptWaitInSec() const {
    return m_nScriptWaitInSec;
}

// ----------------------------------------------------------------------

void ModelServiceConf::setTimeSleepBetweenRunScriptsInSec(int nSec){
    m_nTimeSleepBetweenRunScriptsInSec = nSec;
    if(m_nTimeSleepBetweenRunScriptsInSec < 1){
        m_nTimeSleepBetweenRunScriptsInSec = 10;
    }
}

// ----------------------------------------------------------------------

int ModelServiceConf::timeSleepBetweenRunScriptsInSec() const {
    return m_nTimeSleepBetweenRunScriptsInSec;
}

// ----------------------------------------------------------------------