#include "service_status_cell.h"
#include <wsjcpp_core.h>
#include <stdio.h>
#include <cmath>

std::string ServiceStatusCell::SERVICE_UP = "up";
std::string ServiceStatusCell::SERVICE_DOWN = "down";
std::string ServiceStatusCell::SERVICE_MUMBLE = "mumble";
std::string ServiceStatusCell::SERVICE_CORRUPT = "corrupt";
std::string ServiceStatusCell::SERVICE_SHIT = "shit";
std::string ServiceStatusCell::SERVICE_WAIT = "wait";
std::string ServiceStatusCell::SERVICE_COFFEEBREAK = "coffeebreak";


// ----------------------------------------------------------------------

ServiceStatusCell::ServiceStatusCell(const std::string &sServiceId) {
    m_nUpPointTimeInSec = WsjcppCore::currentTime_seconds();
    TAG = "ServiceStatusCell-" + sServiceId;
    m_sServiceId = sServiceId;
    m_sStatus = ServiceStatusCell::SERVICE_DOWN;
    m_nDefenceFlags = 0;
    m_nAttackFlags = 0;
    m_nAttackPoints = 0;
    m_nDefencePoints = 0;

}

// ----------------------------------------------------------------------

const std::string &ServiceStatusCell::serviceId() {
    return m_sServiceId;
}

// ----------------------------------------------------------------------

void ServiceStatusCell::setDefenceFlags(int nDefenceFlags) {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    m_nDefenceFlags = nDefenceFlags;
}

// ----------------------------------------------------------------------

int ServiceStatusCell::getDefenceFlags() {
    return m_nDefenceFlags;
}

// ----------------------------------------------------------------------

void ServiceStatusCell::incrementDefenceFlags() {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    m_nDefenceFlags++;
}

// ----------------------------------------------------------------------

void ServiceStatusCell::setDefencePoints(int nDefencePoints) {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    m_nDefencePoints = nDefencePoints;
}

// ----------------------------------------------------------------------

int ServiceStatusCell::getDefencePoints() {
    return m_nDefencePoints;
}

// ----------------------------------------------------------------------

void ServiceStatusCell::addDefencePoints(int nDefencePoints) {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    m_nDefencePoints += nDefencePoints;
}

// ----------------------------------------------------------------------

void ServiceStatusCell::setAttackFlags(int nAttackFlags) {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    m_nAttackFlags = nAttackFlags;
}

// ----------------------------------------------------------------------

int ServiceStatusCell::getAttackFlags() {
    return m_nAttackFlags;
}

// ----------------------------------------------------------------------

void ServiceStatusCell::incrementAttackFlags() {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    m_nAttackFlags++;
}

// ----------------------------------------------------------------------

void ServiceStatusCell::setAttackPoints(int nAttackPoints) {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    m_nAttackPoints = nAttackPoints;
}

// ----------------------------------------------------------------------

int ServiceStatusCell::getAttackPoints() {
    return m_nAttackPoints;
}

// ----------------------------------------------------------------------

void ServiceStatusCell::addAttackPoints(int nAttackPoints) {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    m_nAttackPoints += nAttackPoints;
}

// ----------------------------------------------------------------------

void ServiceStatusCell::setFlagsPutted(int nFlagsPutted) {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    m_nFlagsPutted = nFlagsPutted;
}

// ----------------------------------------------------------------------

int ServiceStatusCell::flagsPutted() {
    return m_nFlagsPutted;
}

// ----------------------------------------------------------------------

/*
double ServiceStatusCell::uptime() {
    return m_nUptime;
}*/

// ----------------------------------------------------------------------

int ServiceStatusCell::getUptimeInSec() {
     return WsjcppCore::currentTime_seconds() - m_nUpPointTimeInSec;
}

// ----------------------------------------------------------------------

void ServiceStatusCell::setUpPointTime(int nUpPointTimeInSec) {
    m_nUpPointTimeInSec = nUpPointTimeInSec;
}

// ----------------------------------------------------------------------

void ServiceStatusCell::setStatus(const std::string &sStatus) {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    m_sStatus = sStatus;
    if (sStatus != ServiceStatusCell::SERVICE_UP) {
        m_nUpPointTimeInSec = WsjcppCore::currentTime_seconds();
    }
}

// ----------------------------------------------------------------------

std::string ServiceStatusCell::status() {
    return m_sStatus;
}

// ----------------------------------------------------------------------

double ServiceStatusCell::calculateScore() {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    return  m_nAttackPoints + m_nDefenceFlags;
}

// ----------------------------------------------------------------------
