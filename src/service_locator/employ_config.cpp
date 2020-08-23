
#include "employ_config.h"
#include <wsjcpp_core.h>

// ----------------------------------------------------------------------
// Ctf01dServiceDef

Ctf01dServiceDef::Ctf01dServiceDef(){
    m_nScriptWaitInSec = 10;
    m_bEnabled = true;
    m_nTimeSleepBetweenRunScriptsInSec = 10;
}

// ----------------------------------------------------------------------

void Ctf01dServiceDef::setId(const std::string &sServiceID){
    m_sID = sServiceID;
}

// ----------------------------------------------------------------------

std::string Ctf01dServiceDef::id() const {
    return m_sID;
}

// ----------------------------------------------------------------------

void Ctf01dServiceDef::setName(const std::string &sName){
    m_sName = sName;
}

// ----------------------------------------------------------------------

std::string Ctf01dServiceDef::name() const {
    return m_sName;
}

// ----------------------------------------------------------------------

void Ctf01dServiceDef::setScriptPath(const std::string &sScriptPath){
    m_sScriptPath = sScriptPath;
}

// ----------------------------------------------------------------------

std::string Ctf01dServiceDef::scriptPath() const {
    return m_sScriptPath;
}

// ----------------------------------------------------------------------

void Ctf01dServiceDef::setScriptDir(const std::string &sScriptDir) {
    m_sScriptDir = sScriptDir;
}

// ----------------------------------------------------------------------

std::string Ctf01dServiceDef::scriptDir() const {
    return m_sScriptDir;
}

// ----------------------------------------------------------------------

void Ctf01dServiceDef::setEnabled(bool bEnabled){
    m_bEnabled = bEnabled;
}

// ----------------------------------------------------------------------

bool Ctf01dServiceDef::isEnabled() const {
    return m_bEnabled;
}

// ----------------------------------------------------------------------

void Ctf01dServiceDef::setScriptWaitInSec(int nSec){
    m_nScriptWaitInSec = nSec;
    if(m_nScriptWaitInSec < 1){
        m_nScriptWaitInSec = 10;
    }
}

// ----------------------------------------------------------------------

int Ctf01dServiceDef::scriptWaitInSec() const {
    return m_nScriptWaitInSec;
}

// ----------------------------------------------------------------------

void Ctf01dServiceDef::setTimeSleepBetweenRunScriptsInSec(int nSec){
    m_nTimeSleepBetweenRunScriptsInSec = nSec;
    if(m_nTimeSleepBetweenRunScriptsInSec < 1){
        m_nTimeSleepBetweenRunScriptsInSec = 10;
    }
}

// ----------------------------------------------------------------------

int Ctf01dServiceDef::timeSleepBetweenRunScriptsInSec() const {
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