#include "scoreboard.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <utils_logger.h>

// ---------------------------------------------------------------------

Scoreboard::Scoreboard(bool bRandom, int nGameStart, int nGameEnd, const std::vector<Team> &vTeamsConf, const std::vector<Service> &vServicesConf) {
    TAG = "Scoreboard";
    m_bRandom = bRandom;
    std::srand(unsigned(std::time(0)));
    m_nGameStart = nGameStart;
    m_nGameEnd = nGameEnd;

    m_mapTeamsStatuses.clear(); // possible memory leak
    for (unsigned int iteam = 0; iteam < vTeamsConf.size(); iteam++) {
        Team teamConf = vTeamsConf[iteam];
        std::string sTeamId = teamConf.id();

        m_mapTeamsStatuses[sTeamId] = new ModelTeamStatus(sTeamId, vServicesConf);
        m_mapTeamsStatuses[sTeamId]->setPlace(iteam+1);

        // random values of service for testing
        if (m_bRandom) {
            double nScore = (std::rand() % 10000)/10;
            m_mapTeamsStatuses[sTeamId]->setScore(nScore);
        }

        for (unsigned int iservice = 0; iservice < vServicesConf.size(); iservice++) {
            Service service = vServicesConf[iservice];
            m_mapTeamsStatuses[sTeamId]->setServiceStatus(service.id(), ModelServiceStatus::SERVICE_DOWN);

            // random states of service for testing 
            if (m_bRandom) {
                m_mapTeamsStatuses[sTeamId]->setServiceStatus(service.id(), randomServiceStatus()); 
                m_mapTeamsStatuses[sTeamId]->setTries(std::rand() % 1000);
            }
        }
    }
    
    {
        std::lock_guard<std::mutex> lock(m_mutexJson);
        m_jsonScoreboard.clear();
        for (unsigned int iteam = 0; iteam < vTeamsConf.size(); iteam++) {
            Team teamConf = vTeamsConf[iteam];
            std::string sTeamId = teamConf.id();
            // team.num();
            nlohmann::json teamData;
            teamData["place"] = m_mapTeamsStatuses[sTeamId]->place();
            teamData["score"] = m_mapTeamsStatuses[sTeamId]->score();
            nlohmann::json jsonServices;
            for (unsigned int iservice = 0; iservice < vServicesConf.size(); iservice++) {
                Service serviceConf = vServicesConf[iservice];
                nlohmann::json serviceData;
                serviceData["defence"] = 0;
                serviceData["attack"] = 0;
                serviceData["uptime"] = 100.0;
                serviceData["status"] = m_mapTeamsStatuses[sTeamId]->serviceStatus(serviceConf.id());
                jsonServices[serviceConf.id()] = serviceData;
            }
            teamData["services"] = jsonServices;
            m_jsonScoreboard[teamConf.id()] = teamData;
        }
    }

}

// ----------------------------------------------------------------------

std::string Scoreboard::randomServiceStatus() {
    std::string sResult = ModelServiceStatus::SERVICE_DOWN;
    int nState = std::rand() % 5;
    switch (nState) {
        case 0: sResult = ModelServiceStatus::SERVICE_UP; break;
        case 1: sResult = ModelServiceStatus::SERVICE_DOWN; break;
        case 2: sResult = ModelServiceStatus::SERVICE_MUMBLE; break;
        case 3: sResult = ModelServiceStatus::SERVICE_CORRUPT; break;
        case 4: sResult = ModelServiceStatus::SERVICE_SHIT; break;
    }
    return sResult;
}

// ----------------------------------------------------------------------

void Scoreboard::setServiceStatus(const std::string &sTeamId, const std::string &sServiceId, const std::string &sStatus) {
    std::lock_guard<std::mutex> lock(m_mutexJson);
    std::string sNewStatus = m_bRandom ? randomServiceStatus() : sStatus;

    std::map<std::string,ModelTeamStatus *>::iterator it;
    it = m_mapTeamsStatuses.find(sTeamId);
    if (it != m_mapTeamsStatuses.end()) {
        if (it->second->serviceStatus(sServiceId) != sNewStatus) {
            it->second->setServiceStatus(sServiceId, sNewStatus);
            m_jsonScoreboard[sTeamId][sServiceId]["status"] = sNewStatus;
        }
    }
}

// ----------------------------------------------------------------------

void Scoreboard::incrementTries(const std::string &sTeamId) {
    std::lock_guard<std::mutex> lock(m_mutexJson);
    std::map<std::string,ModelTeamStatus *>::iterator it;
    it = m_mapTeamsStatuses.find(sTeamId);
    if (it != m_mapTeamsStatuses.end()) {
        it->second->setTries(it->second->tries() + 1);
        m_jsonScoreboard[sTeamId]["tries"] = it->second->tries();
    }
}

// ----------------------------------------------------------------------

void Scoreboard::setTries(const std::string &sTeamId, int nTries) {
    std::lock_guard<std::mutex> lock(m_mutexJson);
    std::map<std::string,ModelTeamStatus *>::iterator it;
    it = m_mapTeamsStatuses.find(sTeamId);
    if (it != m_mapTeamsStatuses.end()) {
        it->second->setTries(nTries);
        m_jsonScoreboard[sTeamId]["tries"] = nTries;
    }
}

// ----------------------------------------------------------------------

void Scoreboard::incrementAttackScore(const std::string &sTeamId, const std::string &sServiceId) {
    // TODO
    Log::warn(TAG, "TODO: incrementDefenceScore");
}

// ----------------------------------------------------------------------

void Scoreboard::incrementDefenceScore(const std::string &sTeamId, const std::string &sServiceId) {
    // TODO
    Log::warn(TAG, "TODO: incrementDefenceScore");
}

// ----------------------------------------------------------------------

std::string Scoreboard::serviceStatus(const std::string &sTeamId, const std::string &sServiceId) {
    std::map<std::string,ModelTeamStatus *>::iterator it;
    it = m_mapTeamsStatuses.find(sTeamId);
    if (it != m_mapTeamsStatuses.end()) {
        return it->second->serviceStatus(sServiceId);
    }
    return "";
}

// ----------------------------------------------------------------------

static bool sort_using_greater_than(double u, double v) {
   return u > v;
}

// ----------------------------------------------------------------------

void Scoreboard::setServiceScore(const std::string &sTeamId, const std::string &sServiceId, int nDefence, int nAttack, double nSLA) {
    std::lock_guard<std::mutex> lock(m_mutexJson);
    int nNewDefence = nDefence;
    int nNewAttack = nAttack;
    int nNewSLA = nSLA;

    if (m_bRandom) {
        nNewDefence = std::rand() % 10000;
        nNewAttack = std::rand() % 1000;
        nNewSLA = double(std::rand() % 1000) / 100;
    }

    {
        std::map<std::string,ModelTeamStatus *>::iterator it;
        it = m_mapTeamsStatuses.find(sTeamId);
        if (it != m_mapTeamsStatuses.end()) {
            it->second->setServiceScore(sServiceId, nNewDefence, nNewAttack, nNewSLA);
        }
    }

    // sort places
    {
        std::vector<double> vScores;
        std::map<std::string,ModelTeamStatus *>::iterator it1;
        for (it1 = m_mapTeamsStatuses.begin(); it1 != m_mapTeamsStatuses.end(); it1++) {
            if(std::find(vScores.begin(), vScores.end(), it1->second->score()) == vScores.end()) {
                vScores.push_back(it1->second->score());
            }
        }
        std::sort(vScores.begin(), vScores.end(), sort_using_greater_than);
        for (it1 = m_mapTeamsStatuses.begin(); it1 != m_mapTeamsStatuses.end(); it1++) {
            double nScore = it1->second->score();
            ptrdiff_t pos = std::find(vScores.begin(), vScores.end(), nScore) - vScores.begin();
            it1->second->setPlace(pos + 1); // TODO fix: same scores will be same place
        }
    }

    // update json
    {
        m_jsonScoreboard[sTeamId]["services"][sServiceId]["attack"] = nNewAttack;
        m_jsonScoreboard[sTeamId]["services"][sServiceId]["defence"] = nNewDefence;
        m_jsonScoreboard[sTeamId]["services"][sServiceId]["uptime"] = nNewSLA;

        std::map<std::string,ModelTeamStatus *>::iterator it1;
        for (it1 = m_mapTeamsStatuses.begin(); it1 != m_mapTeamsStatuses.end(); it1++) {
            ModelTeamStatus *pTeamStatus = it1->second;
            std::string sTeamId_ = pTeamStatus->teamId();

            // std::cout << sTeamNum << ": result: score: " << pTeamStatus->score() << ", place: " << pTeamStatus->place() << "\n";
            m_jsonScoreboard[sTeamId_]["score"] = pTeamStatus->score();
            m_jsonScoreboard[sTeamId_]["place"] = pTeamStatus->place();
            m_jsonScoreboard[sTeamId_]["tries"] = pTeamStatus->tries();
        }
    }
}

// ----------------------------------------------------------------------

double Scoreboard::calculateSLA(int nFlagsSuccess, const Service &serviceConf) {
    // TODO start game time
    double nTimeSuccess = nFlagsSuccess * serviceConf.timeSleepBetweenRunScriptsInSec();

    auto now = std::chrono::system_clock::now().time_since_epoch();
    int nLastTime = std::chrono::duration_cast<std::chrono::seconds>(now).count();
    nLastTime = std::min(nLastTime, m_nGameEnd);
    
    double nTimeAll = (double)nLastTime - (double)m_nGameStart;
    double nTimeAll_ = nTimeAll;

    // correction
    nTimeAll = nTimeAll - std::fmod(nTimeAll, serviceConf.timeSleepBetweenRunScriptsInSec());
    nTimeAll += serviceConf.timeSleepBetweenRunScriptsInSec();

    if (nTimeAll == 0) {
        return 100.0; // Normal
    }
    
    // nTimeAll
    double nResult = (nTimeSuccess*100) / nTimeAll;
    if (nResult > 100.0) {
        // Log::err(TAG, "calculateSLA nFlagsSuccess = " + std::to_string(nFlagsSuccess) + "");
        // Log::err(TAG, "calculateSLA nTimeAll_ = " + std::to_string(nTimeAll_) + "");
        // Log::err(TAG, "calculateSLA serviceConf.timeSleepBetweenRunScriptsInSec() = " + std::to_string(serviceConf.timeSleepBetweenRunScriptsInSec()) + "");
        Log::err(TAG, "calculateSLA nResult = " + std::to_string(nResult) + "% - wrong");
        nResult = 100.0;
    }
    return nResult;
}

// ----------------------------------------------------------------------

void Scoreboard::addFlagLive(const Flag &flag) {
    std::lock_guard<std::mutex> lock(m_mutexFlagsLive);
    std::map<std::string,Flag>::iterator it;
    it = m_mapFlagsLive.find(flag.value());
    if (it != m_mapFlagsLive.end()) {
        Log::warn(TAG, flag.value() + " - flag already exists");
    } else {
        m_mapFlagsLive[flag.value()] = flag;
    }
}

// ----------------------------------------------------------------------

void Scoreboard::removeFlagLive(const Flag &flag) {
    std::lock_guard<std::mutex> lock(m_mutexFlagsLive);

    std::map<std::string,Flag>::iterator it;
    it = m_mapFlagsLive.find(flag.value());
    if (it != m_mapFlagsLive.end()) {
        m_mapFlagsLive.erase(flag.value());
    } else {
        Log::warn(TAG, flag.value() + " - flag did not exists");
    }
}

// ----------------------------------------------------------------------

std::string Scoreboard::toString(){
    // TODO mutex
    std::string sResult = ""; 
    std::map<std::string,ModelTeamStatus *>::iterator it;
    for (it = m_mapTeamsStatuses.begin(); it != m_mapTeamsStatuses.end(); ++it){
        sResult += it->first + ": \n"
            "\tscore: " + std::to_string(it->second->score()) + "\n"
            + it->second->servicesToString() + "\n";
    }

    return sResult;
}

// ----------------------------------------------------------------------

const nlohmann::json &Scoreboard::toJson(){
    std::lock_guard<std::mutex> lock(m_mutexJson);
    return m_jsonScoreboard;
}

// ----------------------------------------------------------------------
