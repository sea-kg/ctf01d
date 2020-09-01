
#include "employ_scoreboard.h"
#include <wsjcpp_core.h>

// ---------------------------------------------------------------------
// Ctf01dServiceCostsAndStatistics

Ctf01dServiceCostsAndStatistics::Ctf01dServiceCostsAndStatistics(const std::string &sServiceId) {
    m_sServiceId = sServiceId;
    m_nAllStolenFlagsForService = 0;
    m_nReverseProportionalStolenFlags = 0.0;
    m_nCostDefenceFlag = 0.0;
    m_nAllDefenceFlagsForService = 0;
    m_nReverseProportionalDefenceFlags = 0.0;
    m_nCostStolenFlag = 0.0;
    m_sFirstBlood = "?";
    TAG = "Ctf01dServiceCostsAndStatistics-" + sServiceId;
}

// ----------------------------------------------------------------------

int Ctf01dServiceCostsAndStatistics::allStolenFlagsForService() {
    return m_nAllStolenFlagsForService;
}

// ----------------------------------------------------------------------

void Ctf01dServiceCostsAndStatistics::incrementStolenFlagsForService() {
    m_nAllStolenFlagsForService++;
}

// ----------------------------------------------------------------------

void Ctf01dServiceCostsAndStatistics::setStolenFlagsForService(int nStolenFlags) {
    m_nAllStolenFlagsForService = nStolenFlags;
}

// ----------------------------------------------------------------------

double Ctf01dServiceCostsAndStatistics::updateProportionalStolenFlagsForService(int m_nAllStolenFlags) {
    if (m_nAllStolenFlagsForService > 0) {
        m_nReverseProportionalStolenFlags = double(m_nAllStolenFlags) / double(m_nAllStolenFlagsForService);
    } else {
        m_nReverseProportionalStolenFlags = double(m_nAllStolenFlags) / 1.0;
    }
    return m_nReverseProportionalStolenFlags;
}

// ----------------------------------------------------------------------

double Ctf01dServiceCostsAndStatistics::updateCostStolenFlagForService(int nStolenPoints, double nSumOfReverseProportionalStolenFlags) {
    if (nSumOfReverseProportionalStolenFlags == 0.0) {
        m_nCostStolenFlag = nStolenPoints;
        return m_nCostStolenFlag;
    }
    double k = m_nReverseProportionalStolenFlags / nSumOfReverseProportionalStolenFlags;
    m_nCostStolenFlag = double(nStolenPoints) * k;
    return m_nCostStolenFlag;
}

// ----------------------------------------------------------------------

double Ctf01dServiceCostsAndStatistics::costStolenFlag() {
    return m_nCostStolenFlag;
}

// ----------------------------------------------------------------------

int Ctf01dServiceCostsAndStatistics::allDefenceFlagsForService() {
    return m_nAllDefenceFlagsForService;
}

// ----------------------------------------------------------------------

void Ctf01dServiceCostsAndStatistics::incrementDefenceFlagsForService() {
    m_nAllDefenceFlagsForService++;
}

// ----------------------------------------------------------------------

void Ctf01dServiceCostsAndStatistics::setDefenceFlagsForService(int nDdefenceFlags) {
    m_nAllDefenceFlagsForService = nDdefenceFlags;
}

// ----------------------------------------------------------------------

double Ctf01dServiceCostsAndStatistics::updateProportionalDefenceFlagsForService(int m_nAllDefenceFlags) {
    if (m_nAllDefenceFlagsForService > 0) {
        m_nReverseProportionalDefenceFlags = double(m_nAllDefenceFlags) / double(m_nAllDefenceFlagsForService);
    } else {
        m_nReverseProportionalDefenceFlags = double(m_nAllDefenceFlags) / 1.0;
    }
    return m_nReverseProportionalDefenceFlags;
}

// ----------------------------------------------------------------------

double Ctf01dServiceCostsAndStatistics::updateCostDefenceFlagForService(int nDefencePoints, double nSumOfReverseProportionalDefenceFlags) {
    if (nSumOfReverseProportionalDefenceFlags == 0.0) {
        m_nCostDefenceFlag = nDefencePoints;
        return m_nCostDefenceFlag;
    }
    double k = m_nReverseProportionalDefenceFlags / nSumOfReverseProportionalDefenceFlags;
    m_nCostDefenceFlag = double(nDefencePoints) * k;
    return m_nCostDefenceFlag;
}

// ----------------------------------------------------------------------

double Ctf01dServiceCostsAndStatistics::costDefenceFlag() {
    return m_nCostDefenceFlag;
}

// ----------------------------------------------------------------------

std::string Ctf01dServiceCostsAndStatistics::getFirstBloodTeamId() {
    return m_sFirstBlood;
}

// ----------------------------------------------------------------------

void Ctf01dServiceCostsAndStatistics::setFirstBloodTeamId(const std::string &sFirstBlood) {
    m_sFirstBlood = sFirstBlood;
}

// ---------------------------------------------------------------------
// EmployScoreboard

REGISTRY_WJSCPP_SERVICE_LOCATOR(EmployScoreboard)

EmployScoreboard::EmployScoreboard() 
: WsjcppEmployBase(EmployScoreboard::name(), {}) {
    TAG = EmployScoreboard::name();
}

// ---------------------------------------------------------------------

bool EmployScoreboard::init() {
    WsjcppLog::info(TAG, "init");
    return true;
}

// ---------------------------------------------------------------------

bool EmployScoreboard::deinit() {
    WsjcppLog::info(TAG, "deinit");
    return true;
}


