#include "model_team_status.h"

ModelTeamStatus::ModelTeamStatus(const std::string &sTeamId, const std::vector<Service> &vServicesConf) {
    m_sTeamId = sTeamId;
    m_nScore = 0.0;
    m_nPlace = 0;

    for (unsigned int iservice = 0; iservice < vServicesConf.size(); iservice++) {
        Service serviceConf = vServicesConf[iservice];
        std::string sServiceId = serviceConf.id();
        m_mapServicesStatus[sServiceId] = new ModelServiceStatus(sServiceId);
    }
}

// ----------------------------------------------------------------------

void ModelTeamStatus::setPlace(int nPlace) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_nPlace = nPlace;
}

// ----------------------------------------------------------------------

int ModelTeamStatus::place() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_nPlace;
}

// ----------------------------------------------------------------------

const std::string &ModelTeamStatus::teamId() {
    // std::lock_guard<std::mutex> lock(m_mutex);
    return m_sTeamId;
}

// ----------------------------------------------------------------------

void ModelTeamStatus::setScore(double nScore) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_nScore = nScore;
}

// ----------------------------------------------------------------------

double ModelTeamStatus::score(){
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_nScore;
}

// ----------------------------------------------------------------------

void ModelTeamStatus::setServiceStatus(const std::string &sServiceId, std::string sStatus){
    std::lock_guard<std::mutex> lock(m_mutex);
    m_mapServicesStatus[sServiceId]->setStatus(sStatus);
}

// ----------------------------------------------------------------------

void ModelTeamStatus::setServiceScore(const std::string &sServiceId, int nNewDefence, int nNewAttack, double nNewSLA) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_mapServicesStatus[sServiceId]->setDefence(nNewDefence);
    m_mapServicesStatus[sServiceId]->setAttack(nNewAttack);
    m_mapServicesStatus[sServiceId]->setSLA(nNewSLA);

    // update score
    double nNewScore = 0.0;
    std::map<std::string,ModelServiceStatus*>::iterator it;
    for (it = m_mapServicesStatus.begin(); it != m_mapServicesStatus.end(); ++it){
        ModelServiceStatus *pServiceStatus = it->second;
        double nSum = pServiceStatus->attack() + pServiceStatus->defence();
        nNewScore += nSum * (pServiceStatus->sla() / 100);
    }
    m_nScore = nNewScore;
}

// ----------------------------------------------------------------------

void ModelTeamStatus::setTries(int nTries) {
    m_nTries = nTries;
}

// ----------------------------------------------------------------------

int ModelTeamStatus::tries() {
    return m_nTries;
}

// ----------------------------------------------------------------------

std::string ModelTeamStatus::serviceStatus(const std::string &sServiceId){
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_mapServicesStatus[sServiceId]->status();
}

// ----------------------------------------------------------------------

std::string ModelTeamStatus::servicesToString() {
    std::lock_guard<std::mutex> lock(m_mutex);
    std::string sResult = "";
    /*std::map<int,std::string>::iterator it;
    for (it = m_mapServicesStatus.begin(); it != m_mapServicesStatus.end(); ++it){
        sResult += "\tservice" + std::to_string(it->first) + ": " + it->second + "\n";
    }*/
    return sResult;
}

// ----------------------------------------------------------------------