
#include "employ_config.h"
#include <wsjcpp_core.h>

// ----------------------------------------------------------------------
// Service

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

// ---------------------------------------------------------------------
// Team

Team::Team() {
    // nothing
}

// ---------------------------------------------------------------------

void Team::setId(const std::string &sTeamId){
    m_sTeamID = sTeamId;
}

// ---------------------------------------------------------------------

std::string Team::id() const {
    return m_sTeamID;
}

// ---------------------------------------------------------------------

void Team::setName(const std::string &sName){
    m_sName = sName;
}

// ---------------------------------------------------------------------

std::string Team::name() const {
    return m_sName;
}

// ---------------------------------------------------------------------

void Team::setIpAddress(const std::string &sIpAddress){
    m_sIpAddress = sIpAddress;
}

// ---------------------------------------------------------------------

std::string Team::ipAddress() const {
    return m_sIpAddress;
}

// ---------------------------------------------------------------------

void Team::setActive(bool bActive){
    m_bActive = bActive;
}

// ---------------------------------------------------------------------

bool Team::isActive() const {
    return m_bActive;
}

// ---------------------------------------------------------------------

void Team::setLogo(const std::string &sLogo){
    m_sLogo = sLogo;
}

// ---------------------------------------------------------------------

std::string Team::logo() const {
    return m_sLogo;
}

// ---------------------------------------------------------------------
// EmployConfig

REGISTRY_WJSCPP_SERVICE_LOCATOR(EmployConfig)

EmployConfig::EmployConfig() 
: WsjcppEmployBase(EmployConfig::name(), {}) {
    TAG = EmployConfig::name();
}

// ---------------------------------------------------------------------

bool EmployConfig::init() {
    if (WsjcppCore::getEnv("CTF01D_WORKDIR", m_sWorkDir)) {
        WsjcppLog::info(TAG, "Work Directory from enviroment: " + m_sWorkDir);
    }
    return true;
}

// ---------------------------------------------------------------------

bool EmployConfig::deinit() {
    WsjcppLog::info(TAG, "deinit");
    return true;
}

// ---------------------------------------------------------------------

void EmployConfig::setWorkDir(const std::string &sWorkDir) {
    m_sWorkDir = sWorkDir;
}

// ---------------------------------------------------------------------

std::string EmployConfig::getWorkDir() {
    return m_sWorkDir;
}