#include "employ_flags.h"
#include <employ_config.h>
#include <wsjcpp_core.h>
#include <fstream>
#include <cstring>

// ---------------------------------------------------------------------
// Ctf01dFlag
Ctf01dFlag::Ctf01dFlag() {
    m_sId = "";
    m_sValue = "";
    m_sTeamId = "";
    m_sServiceId = "";
    m_nTimeStartInMs = 0;
    m_nTimeEndInMs = 0;
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
    // TODO redesign more freeble format

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

REGISTRY_WJSCPP_SERVICE_LOCATOR(EmployFlags)

EmployFlags::EmployFlags() 
: WsjcppEmployBase(EmployFlags::name(), { EmployConfig::name() }) {
    TAG = EmployFlags::name();
}

// ---------------------------------------------------------------------

bool EmployFlags::init() {
    WsjcppLog::info(TAG, "init");
    return true;
}

// ---------------------------------------------------------------------

bool EmployFlags::deinit() {
    WsjcppLog::info(TAG, "deinit");
    return true;
}


