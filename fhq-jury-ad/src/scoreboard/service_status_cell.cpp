#include "service_status_cell.h"

std::string ServiceStatusCell::SERVICE_UP = "up";
std::string ServiceStatusCell::SERVICE_DOWN = "down";
std::string ServiceStatusCell::SERVICE_MUMBLE = "mumble";
std::string ServiceStatusCell::SERVICE_CORRUPT = "corrupt";
std::string ServiceStatusCell::SERVICE_SHIT = "shit";
std::string ServiceStatusCell::SERVICE_WAIT = "wait";

// ----------------------------------------------------------------------

ServiceStatusCell::ServiceStatusCell(const std::string &sServiceId) {
    m_sServiceId = sServiceId;
    m_sStatus = ServiceStatusCell::SERVICE_DOWN;
    m_nUptime = 100.0;
    m_nDefence = 0;
    m_nAttack = 0;
}

// ----------------------------------------------------------------------

const std::string &ServiceStatusCell::serviceId() {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    return m_sServiceId;
}

// ----------------------------------------------------------------------

void ServiceStatusCell::setDefence(int nDefence) {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    m_nDefence = nDefence;
}

// ----------------------------------------------------------------------

int ServiceStatusCell::defence() {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    return m_nDefence;
}

// ----------------------------------------------------------------------

void ServiceStatusCell::setAttack(int nAttack) {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    m_nAttack = nAttack;
}

// ----------------------------------------------------------------------

int ServiceStatusCell::attack() {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    return m_nAttack;
}

// ----------------------------------------------------------------------

void ServiceStatusCell::setSLA(double nUptime) {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    m_nUptime = nUptime;
}

// ----------------------------------------------------------------------

double ServiceStatusCell::sla() {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    return m_nUptime;
}

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