<<<<<<< HEAD:src/objects/ctf01d_flag.cpp
#include "ctf01d_flag.h"
#include <iostream>
#include <cstring>
#include <wsjcpp_core.h>

Ctf01dFlag::Ctf01dFlag() {
=======

#include "employ_flags.h"
#include <wsjcpp_core.h>

// ---------------------------------------------------------------------
// Flag
Flag::Flag() {
>>>>>>> Prepare employ flags:fhq-jury-ad/src/employees/employ_flags.cpp
    
}

// ---------------------------------------------------------------------

void Ctf01dFlag::generateRandomFlag(int nTimeFlagLifeInMin, const std::string &sTeamId, const std::string &sServiceId) {
    generateId();
    generateValue();

    // __int64
    long nTimeStartInMs = WsjcppCore::currentTime_milliseconds();
    // std::cout << "nTimeStart: " << nTimeStart << "\n";
    long nTimeEndInMs = nTimeStartInMs + nTimeFlagLifeInMin*60*1000;
    setTimeStartInMs(nTimeStartInMs);
    setTimeEndInMs(nTimeEndInMs);
    m_sTeamId = sTeamId;
    m_sServiceId = sServiceId;
}

// ---------------------------------------------------------------------

void Ctf01dFlag::generateId() {
    static const std::string sAlphabet =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

    std::string sFlagId = "";
    for (int i = 0; i < 10; ++i) {
        sFlagId += sAlphabet[rand() % sAlphabet.length()];
    }
    setId(sFlagId);
}

// ---------------------------------------------------------------------

void Ctf01dFlag::setId(const std::string &sId) {
    m_sId = sId;
}

// ---------------------------------------------------------------------

std::string Ctf01dFlag::getId() const {
    return m_sId;
}

// ---------------------------------------------------------------------

void Ctf01dFlag::generateValue() {
    static const std::string sAlphabet = "0123456789abcdef";
    char sUuid[37];
    memset(&sUuid, '\0', 37);
    sUuid[8] = '-';
    sUuid[13] = '-';
    sUuid[18] = '-';
    sUuid[23] = '-';

    for(int i = 0; i < 36; i++){
        if (i != 8 && i != 13 && i != 18 && i != 23) {
            sUuid[i] = sAlphabet[rand() % sAlphabet.length()];
        }
    }
    setValue(std::string(sUuid));
}

// ---------------------------------------------------------------------

void Ctf01dFlag::setValue(const std::string &sValue) {
    m_sValue = sValue;
}

// ---------------------------------------------------------------------

std::string Ctf01dFlag::getValue() const {
    return m_sValue;
}

// ---------------------------------------------------------------------

void Ctf01dFlag::setTeamId(const std::string &sTeamId) {
    m_sTeamId = sTeamId;
}

// ---------------------------------------------------------------------

const std::string &Ctf01dFlag::getTeamId() const {
    return m_sTeamId;
}

// ---------------------------------------------------------------------

void Ctf01dFlag::setServiceId(const std::string &sServiceId) {
    m_sServiceId = sServiceId;
}

// ---------------------------------------------------------------------

const std::string &Ctf01dFlag::getServiceId() const {
    return m_sServiceId;
}

// ---------------------------------------------------------------------

void Ctf01dFlag::setTimeStartInMs(long nTimeStartInMs) {
    m_nTimeStartInMs = nTimeStartInMs;
}

// ---------------------------------------------------------------------

long Ctf01dFlag::getTimeStartInMs() const {
    return m_nTimeStartInMs;
}

// ---------------------------------------------------------------------

void Ctf01dFlag::setTimeEndInMs(long nTimeEndInMs) {
    m_nTimeEndInMs = nTimeEndInMs;
}

// ---------------------------------------------------------------------

long Ctf01dFlag::getTimeEndInMs() const {
    return m_nTimeEndInMs;
}

// ---------------------------------------------------------------------

void Ctf01dFlag::copyFrom(const Ctf01dFlag &flag) {
    this->setId(flag.getId());
    this->setValue(flag.getValue());
    this->setServiceId(flag.getServiceId());
    this->setTeamId(flag.getTeamId());
    this->setTimeStartInMs(flag.getTimeStartInMs());
    this->setTimeEndInMs(flag.getTimeEndInMs());
}

// ---------------------------------------------------------------------
// EmployFlags

REGISTRY_WJSCPP_EMPLOY(EmployFlags)

EmployFlags::EmployFlags() 
: WsjcppEmployBase(EmployFlags::name(), {}) {
    TAG = EmployFlags::name();
}

// ---------------------------------------------------------------------

bool EmployFlags::init() {
    if (m_sDirecotry == "") {
        WsjcppLog::info(TAG, "You must setDerectory before init");
        return false;
    }
    std::string sFlagsAttempts = WsjcppCore::doNormalizePath(m_sDirecotry + "/attempts");
    if (!WsjcppCore::dirExists(sFlagsAttempts)) {
        WsjcppCore::makeDir(sFlagsAttempts);
    }
    return true;
}

// ---------------------------------------------------------------------

bool EmployFlags::deinit() {
    WsjcppLog::info(TAG, "deinit");
    return true;
}

// ---------------------------------------------------------------------

void EmployFlags::setDirectory(const std::string &sDirecotry) {
    m_sDirecotry = sDirecotry;
}

// ---------------------------------------------------------------------

// add flag attempt
void EmployFlags::insertFlagAttempt(const std::string &sTeamId, const std::string &sFlag) {
    WsjcppLog::warn(TAG, "Not implemented insertFlagAttempt");
    // TODO must be special writer how will be append to file and rotate if need
}

// ---------------------------------------------------------------------

// count of flag attempts for init scoreboard
int EmployFlags::numberOfFlagAttempts(const std::string &sTeamId) {
    return m_mapNumberFlagAttemps[sTeamId];
}

// ---------------------------------------------------------------------