#include "team_status_row.h"

TeamStatusRow::TeamStatusRow(
    const std::string &sTeamId,
    const std::vector<Ctf01dServiceDef> &vServicesConf,
    int nGameStartInSec,
    int nGameEndInSec
) {
    m_sTeamId = sTeamId;
    m_nPlace = 0;
    m_nPoints = 0;

    for (unsigned int i = 0; i < vServicesConf.size(); i++) {
        Ctf01dServiceDef serviceConf = vServicesConf[i];
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

void TeamStatusRow::setPoints(int nPoints) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_nPoints = nPoints;
}

// ----------------------------------------------------------------------

int TeamStatusRow::getPoints() {
    return m_nPoints;
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

void TeamStatusRow::incrementDefence(const std::string &sServiceId, int nFlagPoints) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_mapServicesStatus[sServiceId]->incrementDefenceFlags();
    m_mapServicesStatus[sServiceId]->addDefencePoints(nFlagPoints);
    m_nPoints += nFlagPoints;
}

// ----------------------------------------------------------------------

int TeamStatusRow::getDefenceFlags(const std::string &sServiceId) {
    return m_mapServicesStatus[sServiceId]->getDefenceFlags();
}

// ----------------------------------------------------------------------

int TeamStatusRow::getDefencePoints(const std::string &sServiceId) {
    return m_mapServicesStatus[sServiceId]->getDefencePoints();
}

// ----------------------------------------------------------------------

void TeamStatusRow::setServiceDefenceFlagsAndPoints(const std::string &sServiceId, int nDefenceFlags, int nDefencePoints) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_mapServicesStatus[sServiceId]->setDefenceFlags(nDefenceFlags);
    m_mapServicesStatus[sServiceId]->setDefencePoints(nDefencePoints);
    m_nPoints += nDefencePoints;
}

// ----------------------------------------------------------------------

void TeamStatusRow::incrementAttack(const std::string &sServiceId, int nFlagPoints) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_mapServicesStatus[sServiceId]->incrementAttackFlags();
    m_mapServicesStatus[sServiceId]->addAttackPoints(nFlagPoints);
    m_nPoints += nFlagPoints;
}

// ----------------------------------------------------------------------

void TeamStatusRow::setServiceAttackFlagsAndPoints(const std::string &sServiceId, int nAttackFlags, int nAttackPoints) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_mapServicesStatus[sServiceId]->setAttackFlags(nAttackFlags);
    m_mapServicesStatus[sServiceId]->setAttackPoints(nAttackPoints);
    m_nPoints += nAttackPoints;
}

// ----------------------------------------------------------------------

int TeamStatusRow::getAttackFlags(const std::string &sServiceId) {
    return m_mapServicesStatus[sServiceId]->getAttackFlags();
}

// ----------------------------------------------------------------------

int TeamStatusRow::getAttackPoints(const std::string &sServiceId) {
    return m_mapServicesStatus[sServiceId]->getAttackPoints();
}

// ----------------------------------------------------------------------

void TeamStatusRow::updateScore(const std::string &sServiceId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    int nFlagsPutted = m_mapServicesStatus[sServiceId]->flagsPutted();
    m_mapServicesStatus[sServiceId]->setFlagsPutted(nFlagsPutted);
}

// ----------------------------------------------------------------------

void TeamStatusRow::setServiceFlagsPutted(const std::string &sServiceId, int nFlagSuccess) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_mapServicesStatus[sServiceId]->setFlagsPutted(nFlagSuccess);
}

// ----------------------------------------------------------------------

double TeamStatusRow::serviceUptime(const std::string &sServiceId) {
    return 100.0; // m_mapServicesStatus[sServiceId]->uptime();
}

// ----------------------------------------------------------------------
