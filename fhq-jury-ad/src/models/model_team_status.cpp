#include "model_team_status.h"

ModelTeamStatus::ModelTeamStatus(int nTeamNum, const std::vector<ModelServiceConf> &vServicesConf) {
    m_nTeamNum = nTeamNum;
    m_nScore = 0.0;
    m_nPlace = 0;

    for (unsigned int iservice = 0; iservice < vServicesConf.size(); iservice++) {
        ModelServiceConf serviceConf = vServicesConf[iservice];
        int nServiceNum = serviceConf.num();
        m_mapServicesStatus[nServiceNum] = new ModelServiceStatus(nServiceNum);
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

int ModelTeamStatus::teamNum() {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_nTeamNum;
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

void ModelTeamStatus::setServiceStatus(int nServiceNum, std::string sStatus){
    std::lock_guard<std::mutex> lock(m_mutex);
    m_mapServicesStatus[nServiceNum]->setStatus(sStatus);
}

// ----------------------------------------------------------------------

void ModelTeamStatus::setServiceScore(int nServiceNum, int nNewDefence, int nNewAttack, double nNewSLA) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_mapServicesStatus[nServiceNum]->setDefence(nNewDefence);
    m_mapServicesStatus[nServiceNum]->setAttack(nNewAttack);
    m_mapServicesStatus[nServiceNum]->setSLA(nNewSLA);

    // update score
    double nNewScore = 0.0;
    std::map<int,ModelServiceStatus*>::iterator it;
    for (it = m_mapServicesStatus.begin(); it != m_mapServicesStatus.end(); ++it){
        ModelServiceStatus *pServiceStatus = it->second;
        double nSum = pServiceStatus->attack() + pServiceStatus->defence();
        nNewScore += nSum * (pServiceStatus->sla() / 100);
    }
    m_nScore = nNewScore;
}

// ----------------------------------------------------------------------

std::string ModelTeamStatus::serviceStatus(int nServiceNum){
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_mapServicesStatus[nServiceNum]->status();
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