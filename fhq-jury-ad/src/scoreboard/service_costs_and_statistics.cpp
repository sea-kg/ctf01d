#include "service_costs_and_statistics.h"
#include <string>

ServiceCostsAndStatistics::ServiceCostsAndStatistics(const std::string &sServiceId) {
    m_sServiceId = sServiceId;
    m_nAllStolenFlagsForService = 0;
    m_nReverseProportionalStolenFlags = 0.0;
    m_nCostDefenceFlag = 0.0;
    m_nAllDefenceFlagsForService = 0;
    m_nReverseProportionalDefenceFlags = 0.0;
    m_nCostStolenFlag = 0.0;
    m_sFirstBlood = "?";
}

// ----------------------------------------------------------------------

int ServiceCostsAndStatistics::allStolenFlagsForService() {
    return m_nAllStolenFlagsForService;
}

// ----------------------------------------------------------------------

void ServiceCostsAndStatistics::incrementStolenFlagsForService() {
    m_nAllStolenFlagsForService++;
}

// ----------------------------------------------------------------------

void ServiceCostsAndStatistics::setStolenFlagsForService(int nStolenFlags) {
    m_nAllStolenFlagsForService = nStolenFlags;
}

// ----------------------------------------------------------------------

double ServiceCostsAndStatistics::updateProportionalStolenFlagsForService(int m_nAllStolenFlags) {
    if (m_nAllStolenFlagsForService > 0) {
        m_nReverseProportionalStolenFlags = double(m_nAllStolenFlags) / double(m_nAllStolenFlagsForService);
    } else {
        m_nReverseProportionalStolenFlags = 1.0;
    }
    return m_nReverseProportionalStolenFlags;
}

// ----------------------------------------------------------------------

double ServiceCostsAndStatistics::updateCostStolenFlagForService(int nStolenPoints, double nSumOfReverseProportionalStolenFlags) {
    m_nCostStolenFlag = double(nStolenPoints) * (m_nReverseProportionalStolenFlags / nSumOfReverseProportionalStolenFlags);
    return m_nCostStolenFlag;
}

// ----------------------------------------------------------------------

double ServiceCostsAndStatistics::costStolenFlag() {
    return m_nCostStolenFlag;
}

// ----------------------------------------------------------------------

int ServiceCostsAndStatistics::allDefenceFlagsForService() {
    return m_nAllDefenceFlagsForService;
}

// ----------------------------------------------------------------------

void ServiceCostsAndStatistics::incrementDefenceFlagsForService() {
    m_nAllDefenceFlagsForService++;
}

// ----------------------------------------------------------------------

void ServiceCostsAndStatistics::setDefenceFlagsForService(int nDdefenceFlags) {
    m_nAllDefenceFlagsForService = nDdefenceFlags;
}

// ----------------------------------------------------------------------

double ServiceCostsAndStatistics::updateProportionalDefenceFlagsForService(int m_nAllDefenceFlags) {
    if (m_nAllDefenceFlagsForService > 0) {
        m_nReverseProportionalDefenceFlags = double(m_nAllDefenceFlags) / double(m_nAllDefenceFlagsForService);
    } else {
        m_nReverseProportionalDefenceFlags = 1.0;
    }
    return m_nReverseProportionalDefenceFlags;
}

// ----------------------------------------------------------------------

double ServiceCostsAndStatistics::updateCostDefenceFlagForService(int nDefencePoints, double nSumOfReverseProportionalDefenceFlags) {
    m_nCostDefenceFlag = double(nDefencePoints) * (m_nReverseProportionalDefenceFlags / nSumOfReverseProportionalDefenceFlags);
    return m_nCostDefenceFlag;
}

// ----------------------------------------------------------------------

double ServiceCostsAndStatistics::costDefenceFlag() {
    return m_nCostDefenceFlag;
}

// ----------------------------------------------------------------------

std::string ServiceCostsAndStatistics::getFirstBloodTeamId() {
    return m_sFirstBlood;
}

// ----------------------------------------------------------------------

void ServiceCostsAndStatistics::setFirstBloodTeamId(const std::string &sFirstBlood) {
    m_sFirstBlood = sFirstBlood;
}