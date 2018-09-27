#include "model_service_status.h"

std::string ModelServiceStatus::SERVICE_UP = "up";
std::string ModelServiceStatus::SERVICE_DOWN = "down";
std::string ModelServiceStatus::SERVICE_MUMBLE = "mumble";
std::string ModelServiceStatus::SERVICE_CORRUPT = "corrupt";
std::string ModelServiceStatus::SERVICE_SHIT = "shit";
std::string ModelServiceStatus::SERVICE_WAIT = "wait";

// ----------------------------------------------------------------------

ModelServiceStatus::ModelServiceStatus(int nServiceNum) {
    m_nServiceNum = nServiceNum;
    m_sStatus = ModelServiceStatus::SERVICE_DOWN;
    m_nSLA = 100.0;
    m_nDefence = 0;
    m_nAttack = 0;
}

// ----------------------------------------------------------------------

int ModelServiceStatus::serviceNum() {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    return m_nServiceNum;
}

// ----------------------------------------------------------------------

void ModelServiceStatus::setDefence(int nDefence) {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    m_nDefence = nDefence;
}

// ----------------------------------------------------------------------

int ModelServiceStatus::defence() {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    return m_nDefence;
}

// ----------------------------------------------------------------------

void ModelServiceStatus::setAttack(int nAttack) {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    m_nAttack = nAttack;
}

// ----------------------------------------------------------------------

int ModelServiceStatus::attack() {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    return m_nAttack;
}

// ----------------------------------------------------------------------

void ModelServiceStatus::setSLA(double nSLA) {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    m_nSLA = nSLA;
}

// ----------------------------------------------------------------------

double ModelServiceStatus::sla() {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    return m_nSLA;
}

// ----------------------------------------------------------------------

void ModelServiceStatus::setStatus(const std::string &sStatus) {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    m_sStatus = sStatus;
}

// ----------------------------------------------------------------------

std::string ModelServiceStatus::status() {
    return m_sStatus;
}

// ----------------------------------------------------------------------