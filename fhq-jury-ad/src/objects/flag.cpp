#include "flag.h"
#include <iostream>
#include <chrono>
#include <cstring>

Flag::Flag() {
    
}

// ---------------------------------------------------------------------

void Flag::generateRandomFlag(int nTimeFlagLifeInMin) {
    generateId();
    generateValue();

    // __int64
    long nTimeStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    // std::cout << "nTimeStart: " << nTimeStart << "\n";
    long nTimeEnd = nTimeStart + nTimeFlagLifeInMin*60*1000;
    setTimeStart(nTimeStart);
    setTimeEnd(nTimeEnd);
    m_sTeamStole = "";
}

// ---------------------------------------------------------------------

void Flag::setId(const std::string &sId) {
    m_sId = sId;
}

// ---------------------------------------------------------------------

void Flag::generateId() {
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

void Flag::generateValue() {
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

std::string Flag::id() const {
    return m_sId;
}

// ---------------------------------------------------------------------

void Flag::setValue(const std::string &sValue) {
    m_sValue = sValue;
}

// ---------------------------------------------------------------------

std::string Flag::value() const {
    return m_sValue;
}

// ---------------------------------------------------------------------

void Flag::setTeamId(const std::string &sTeamId) {
    m_sTeamId = sTeamId;
}

// ---------------------------------------------------------------------

const std::string &Flag::teamId() const {
    return m_sTeamId;
}

// ---------------------------------------------------------------------

void Flag::setServiceId(const std::string &sServiceId) {
    m_sServiceId = sServiceId;
}

// ---------------------------------------------------------------------

const std::string &Flag::serviceId() const {
    return m_sServiceId;
}

// ---------------------------------------------------------------------

void Flag::setTimeStart(long nTimeStart) {
    m_nTimeStart = nTimeStart;
}

// ---------------------------------------------------------------------

long Flag::timeStart() const {
    return m_nTimeStart;
}

// ---------------------------------------------------------------------

void Flag::setTimeEnd(long nTimeEnd) {
    m_nTimeEnd = nTimeEnd;
}

// ---------------------------------------------------------------------

long Flag::timeEnd() const {
    return m_nTimeEnd;
}

// ---------------------------------------------------------------------

void Flag::setTeamStole(const std::string &sTeamStole) {
    m_sTeamStole = sTeamStole;
}

// ---------------------------------------------------------------------

const std::string & Flag::teamStole() const {
    return m_sTeamStole;
}

// ---------------------------------------------------------------------

void Flag::copyFrom(const Flag &flag) {
    this->setId(flag.id());
    this->setValue(flag.value());
    this->setServiceId(flag.serviceId());
    this->setTeamId(flag.teamId());
    this->setTeamStole(flag.teamStole());
    this->setTimeStart(flag.timeStart());
    this->setTimeEnd(flag.timeEnd());
}

// ---------------------------------------------------------------------
