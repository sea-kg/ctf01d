#include "service_status_cell.h"
#include <ts.h>
#include <utils_logger.h>

std::string ServiceStatusCell::SERVICE_UP = "up";
std::string ServiceStatusCell::SERVICE_DOWN = "down";
std::string ServiceStatusCell::SERVICE_MUMBLE = "mumble";
std::string ServiceStatusCell::SERVICE_CORRUPT = "corrupt";
std::string ServiceStatusCell::SERVICE_SHIT = "shit";
std::string ServiceStatusCell::SERVICE_WAIT = "wait";
std::string ServiceStatusCell::SERVICE_COFFEEBREAK = "coffeebreak";


// ----------------------------------------------------------------------

ServiceStatusCell::ServiceStatusCell(const Service &serviceConf, int nGameStartInSec, int nGameEndInSec) {
    m_serviceConf = serviceConf;
    TAG = "ServiceStatusCell-" + serviceConf.id();
    m_sServiceId = serviceConf.id();
    m_nGameStartInSec = nGameStartInSec;
    m_nGameEndInSec = nGameEndInSec;
    m_sStatus = ServiceStatusCell::SERVICE_DOWN;
    m_nUptime = 0.0;
    m_nDefence = 0;
    m_nAttack = 0;
}

// ----------------------------------------------------------------------

const std::string &ServiceStatusCell::serviceId() {
    return m_sServiceId;
}

// ----------------------------------------------------------------------

void ServiceStatusCell::setDefence(int nDefence) {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    m_nDefence = nDefence;
}

// ----------------------------------------------------------------------

int ServiceStatusCell::defence() {
    return m_nDefence;
}

// ----------------------------------------------------------------------

void ServiceStatusCell::setAttack(int nAttack) {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    m_nAttack = nAttack;
}

// ----------------------------------------------------------------------

int ServiceStatusCell::attack() {
    return m_nAttack;
}

// ----------------------------------------------------------------------

void ServiceStatusCell::setFlagsPutted(int nFlagsPutted) {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    m_nFlagsPutted = nFlagsPutted;
    
    double nTimeSuccess = nFlagsPutted * m_serviceConf.timeSleepBetweenRunScriptsInSec();

    int nLastTime = TS::currentTime_seconds();
    nLastTime = std::min(nLastTime, m_nGameEndInSec);
    
    double nTimeAll = (double)nLastTime - (double)m_nGameStartInSec;

    // correction
    nTimeAll = nTimeAll - std::fmod(nTimeAll, m_serviceConf.timeSleepBetweenRunScriptsInSec());
    nTimeAll += m_serviceConf.timeSleepBetweenRunScriptsInSec();

    if (nTimeAll == 0) {
        m_nUptime = 100.0; // Normal
        return;
    }
    
    // nTimeAll
    double nResult = (nTimeSuccess*100) / nTimeAll;
    if (nResult > 100.0) {
        // Log::err(TAG, "calculateSLA nFlagsSuccess = " + std::to_string(nFlagsSuccess) + "");
        // Log::err(TAG, "calculateSLA nTimeAll_ = " + std::to_string(nTimeAll_) + "");
        // Log::err(TAG, "calculateSLA serviceConf.timeSleepBetweenRunScriptsInSec() = " + std::to_string(serviceConf.timeSleepBetweenRunScriptsInSec()) + "");
        Log::err(TAG, "calculateUptime nResult = " + std::to_string(nResult) + "% - wrong");
        m_nUptime = 100.0;
    } else {
        m_nUptime = nResult;
    }
}

// ----------------------------------------------------------------------

int ServiceStatusCell::flagsPutted() {
    return m_nFlagsPutted;
}

// ----------------------------------------------------------------------

/*
double ServiceStatusCell::uptime() {
    return m_nUptime;
}
*/

// ----------------------------------------------------------------------

void ServiceStatusCell::setStatus(const std::string &sStatus) {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    m_sStatus = sStatus;
}

// ----------------------------------------------------------------------

std::string ServiceStatusCell::status() {
    return m_sStatus;
}

// ----------------------------------------------------------------------

double ServiceStatusCell::calculateScore() {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    return  double(m_nAttack + m_nDefence) * (m_nUptime / 100);
}

// ----------------------------------------------------------------------
