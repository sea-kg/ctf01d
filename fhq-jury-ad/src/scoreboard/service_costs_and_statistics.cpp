#include "service_costs_and_statistics.h"
#include <string>

ServiceCostsAndStatistics::ServiceCostsAndStatistics(const std::string &sServiceId) {
    m_sServiceId = sServiceId;
    m_nAllStolenFlagsForService = 0;
    m_nReverseProportionalStolenFlags = 0.0;
    m_nAllDefenceFlagsForService = 0;
    m_nReverseProportionalDefenceFlags = 0.0;
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

double ServiceCostsAndStatistics::updateProportionalStolenFlagsForService(int m_nAllStolenFlags) {
    if (m_nAllStolenFlagsForService > 0) {
        m_nReverseProportionalStolenFlags = double(m_nAllStolenFlags) / double(m_nAllStolenFlagsForService);
    } else {
        m_nReverseProportionalStolenFlags = 0.0;
    }
    return m_nReverseProportionalStolenFlags;
}

// ----------------------------------------------------------------------

double ServiceCostsAndStatistics::updateCostStolenFlagForService(int nStolenPoints, double nSumOfReverseProportionalStolenFlags) {
    m_nCostStolenFlag = double(nStolenPoints) * (m_nReverseProportionalStolenFlags / nSumOfReverseProportionalStolenFlags);
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

double ServiceCostsAndStatistics::updateProportionalDefenceFlagsForService(int m_nAllDefenceFlags) {
    if (m_nAllDefenceFlagsForService > 0) {
        m_nReverseProportionalDefenceFlags = double(m_nAllDefenceFlags) / double(m_nAllDefenceFlagsForService);
    } else {
        m_nReverseProportionalDefenceFlags = 0.0;
    }
    return m_nReverseProportionalDefenceFlags;
}

// ----------------------------------------------------------------------

double ServiceCostsAndStatistics::updateCostDefenceFlagForService(int nDefencePoints, double nSumOfReverseProportionalDefenceFlags) {
    m_nCostDefenceFlag = double(nDefencePoints) * (m_nReverseProportionalDefenceFlags / nSumOfReverseProportionalDefenceFlags);
    return m_nCostDefenceFlag;
}