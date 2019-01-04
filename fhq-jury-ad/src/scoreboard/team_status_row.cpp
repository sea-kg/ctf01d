#include "team_status_row.h"

TeamStatusRow::TeamStatusRow(const std::string &sTeamId, const std::vector<Service> &vServicesConf, int nGameStartInSec, int nGameEndInSec) {
    m_sTeamId = sTeamId;
    m_nScore = 0.0;
    m_nPlace = 0;

    for (unsigned int i = 0; i < vServicesConf.size(); i++) {
        Service serviceConf = vServicesConf[i];
        std::string sServiceId = serviceConf.id();
        m_mapServicesStatus[sServiceId] = new ServiceStatusCell(serviceConf, nGameStartInSec, nGameEndInSec);
    }
}

// ----------------------------------------------------------------------

void TeamStatusRow::setPlace(int nPlace) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_nPlace = nPlace;
}

// ----------------------------------------------------------------------

int TeamStatusRow::place() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_nPlace;
}

// ----------------------------------------------------------------------

const std::string &TeamStatusRow::teamId() {
    // std::lock_guard<std::mutex> lock(m_mutex);
    return m_sTeamId;
}

// ----------------------------------------------------------------------

void TeamStatusRow::setScore(double nScore) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_nScore = nScore;
}

// ----------------------------------------------------------------------

double TeamStatusRow::score(){
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_nScore;
}

// ----------------------------------------------------------------------

void TeamStatusRow::setServiceStatus(const std::string &sServiceId, std::string sStatus){
    std::lock_guard<std::mutex> lock(m_mutex);
    m_mapServicesStatus[sServiceId]->setStatus(sStatus);
}

// ----------------------------------------------------------------------

void TeamStatusRow::setTries(int nTries) {
    m_nTries = nTries;
}

// ----------------------------------------------------------------------

int TeamStatusRow::tries() {
    return m_nTries;
}

// ----------------------------------------------------------------------

std::string TeamStatusRow::serviceStatus(const std::string &sServiceId){
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_mapServicesStatus[sServiceId]->status();
}

// ----------------------------------------------------------------------

std::string TeamStatusRow::servicesToString() {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::string sResult = "";
    /*std::map<int,std::string>::iterator it;
    for (it = m_mapServicesStatus.begin(); it != m_mapServicesStatus.end(); ++it){
        sResult += "\tservice" + std::to_string(it->first) + ": " + it->second + "\n";
    }*/
    return sResult;
}

// ----------------------------------------------------------------------

int TeamStatusRow::incrementDefence(const std::string &sServiceId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    int nDefence = m_mapServicesStatus[sServiceId]->defence();
    nDefence++;
    m_mapServicesStatus[sServiceId]->setDefence(nDefence);
    recalculateScore();
    return nDefence;
}

// ----------------------------------------------------------------------

void TeamStatusRow::setServiceDefence(const std::string &sServiceId, int nDefence) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_mapServicesStatus[sServiceId]->setDefence(nDefence);
    recalculateScore();
}

// ----------------------------------------------------------------------

int TeamStatusRow::incrementAttack(const std::string &sServiceId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    int nAttack = m_mapServicesStatus[sServiceId]->attack();
    nAttack++;
    m_mapServicesStatus[sServiceId]->setAttack(nAttack);
    recalculateScore();
    return nAttack;
}

// ----------------------------------------------------------------------

void TeamStatusRow::setServiceAttack(const std::string &sServiceId, int nAttack) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_mapServicesStatus[sServiceId]->setAttack(nAttack);
    recalculateScore();
}

// ----------------------------------------------------------------------

int TeamStatusRow::incrementFlagsPutted(const std::string &sServiceId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    int nFlagsPutted = m_mapServicesStatus[sServiceId]->flagsPutted();
    nFlagsPutted++;
    m_mapServicesStatus[sServiceId]->setFlagsPutted(nFlagsPutted);
    recalculateScore();
    return nFlagsPutted;
}

// ----------------------------------------------------------------------

void TeamStatusRow::setServiceFlagsPutted(const std::string &sServiceId, int nFlagSuccess) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_mapServicesStatus[sServiceId]->setFlagsPutted(nFlagSuccess);
    recalculateScore();
}

// ----------------------------------------------------------------------

double TeamStatusRow::serviceUptime(const std::string &sServiceId) {
    return m_mapServicesStatus[sServiceId]->uptime();
}

// ----------------------------------------------------------------------

void TeamStatusRow::recalculateScore() {
    double nNewScore = 0.0;
    std::map<std::string,ServiceStatusCell*>::iterator it;
    for (it = m_mapServicesStatus.begin(); it != m_mapServicesStatus.end(); ++it){
        ServiceStatusCell *pServiceStatus = it->second;
        nNewScore += pServiceStatus->calculateScore();
    }
    m_nScore = nNewScore;
}

// ----------------------------------------------------------------------
