#include "scoreboard.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <utils_logger.h>
#include <wsjcpp_core.h>

// ---------------------------------------------------------------------

Scoreboard::Scoreboard(
    bool bRandom,
    int nGameStartInSec,
    int nGameEndInSec,
    int nGameCoffeeBreakStartInSec,
    int nGameCoffeeBreakEndInSec,
    int nFlagTimeLiveInSec,
    int nBacisCostsStolenFlagInPoints,
    const std::vector<Team> &vTeamsConf,
    const std::vector<Service> &vServicesConf,
    Storage *pStorage
) {
    TAG = "Scoreboard";
    m_bRandom = bRandom;
    m_pStorage = pStorage;
    std::srand(unsigned(std::time(0)));
    m_nGameStartInSec = nGameStartInSec;
    m_nGameEndInSec = nGameEndInSec;
    m_nGameCoffeeBreakStartInSec = nGameCoffeeBreakStartInSec;
    m_nGameCoffeeBreakEndInSec = nGameCoffeeBreakEndInSec;
    m_nFlagTimeLiveInSec = nFlagTimeLiveInSec;
    m_nAllStolenFlags = 0;
    m_nAllDefenceFlags = 0;
    m_nBacisCostsStolenFlagInPoints = nBacisCostsStolenFlagInPoints;

    m_mapTeamsStatuses.clear(); // possible memory leak
    for (unsigned int iteam = 0; iteam < vTeamsConf.size(); iteam++) {
        Team teamConf = vTeamsConf[iteam];
        std::string sTeamId = teamConf.id();

        m_mapTeamsStatuses[sTeamId] = new TeamStatusRow(sTeamId, vServicesConf, nGameStartInSec, nGameEndInSec);
        m_mapTeamsStatuses[sTeamId]->setPlace(iteam+1);

        // random values of service for testing
        if (m_bRandom) {
            int nPoints = (std::rand() % 10000);
            m_mapTeamsStatuses[sTeamId]->setPoints(nPoints);
        }

        for (unsigned int iservice = 0; iservice < vServicesConf.size(); iservice++) {
            Service service = vServicesConf[iservice];
            m_mapTeamsStatuses[sTeamId]->setServiceStatus(service.id(), ServiceStatusCell::SERVICE_DOWN);

            // random states of service for testing 
            if (m_bRandom) {
                m_mapTeamsStatuses[sTeamId]->setServiceStatus(service.id(), randomServiceStatus()); 
                m_mapTeamsStatuses[sTeamId]->setTries(std::rand() % 1000);
            }
        }
    }

    // keep the list of the services ids
    for (unsigned int i = 0; i < vServicesConf.size(); i++) {
        m_vServices.push_back(vServicesConf[i]);
        std::string sServiceId = vServicesConf[i].id();
        m_mapServiceCostsAndStatistics[sServiceId] = new ServiceCostsAndStatistics(sServiceId);
    }

    // keep the list of the teams ids
    for (unsigned int i = 0; i < vTeamsConf.size(); i++) {
        m_vTeams.push_back(vTeamsConf[i]);
    }

    initJsonScoreboard();
}

// ----------------------------------------------------------------------

void Scoreboard::initJsonScoreboard() {
    std::lock_guard<std::mutex> lock(m_mutexJson);
    m_jsonScoreboard.clear();
    nlohmann::json jsonCosts;
    for (unsigned int iservice = 0; iservice < m_vServices.size(); iservice++) {
        Service serviceConf = m_vServices[iservice];
        nlohmann::json serviceCosts;
        serviceCosts["cost_att"] = m_mapServiceCostsAndStatistics[serviceConf.id()]->costStolenFlag();
        serviceCosts["cost_def"] = m_mapServiceCostsAndStatistics[serviceConf.id()]->costDefenceFlag();
        serviceCosts["af_att"] = m_nAllStolenFlags;
        serviceCosts["af_def"] = m_nAllDefenceFlags;
        serviceCosts["first_blood"] = "?";
        jsonCosts[serviceConf.id()] = serviceCosts;
    }
    m_jsonScoreboard["s_sta"] = jsonCosts;

    nlohmann::json jsonScoreboard;
    for (unsigned int iteam = 0; iteam < m_vTeams.size(); iteam++) {
        Team teamConf = m_vTeams[iteam];
        std::string sTeamId = teamConf.id();
        nlohmann::json teamData;
        teamData["place"] = m_mapTeamsStatuses[sTeamId]->place();
        teamData["points"] = double(m_mapTeamsStatuses[sTeamId]->getPoints()) / 10.0;
        teamData["tries"] = 0;
        nlohmann::json jsonServices;
        for (unsigned int iservice = 0; iservice < m_vServices.size(); iservice++) {
            Service serviceConf = m_vServices[iservice];
            nlohmann::json serviceData;
            serviceData["def"] = 0;
            serviceData["pt_def"] = 0;
            serviceData["att"] = 0;
            serviceData["pt_att"] = 0;
            serviceData["upt"] = 100.0;
            serviceData["status"] = m_mapTeamsStatuses[sTeamId]->serviceStatus(serviceConf.id());
            jsonServices[serviceConf.id()] = serviceData;
        }
        teamData["ts_sta"] = jsonServices;
        jsonScoreboard[teamConf.id()] = teamData;
    }
    m_jsonScoreboard["scoreboard"] = jsonScoreboard;
}

// ----------------------------------------------------------------------

void Scoreboard::updateJsonScoreboard() {
    std::lock_guard<std::mutex> lock(m_mutexJson);

    
    // TODO update score
    // TODO update costs
}

// ----------------------------------------------------------------------

std::string Scoreboard::randomServiceStatus() {
    std::string sResult = ServiceStatusCell::SERVICE_DOWN;
    int nState = std::rand() % 5;
    switch (nState) {
        case 0: sResult = ServiceStatusCell::SERVICE_UP; break;
        case 1: sResult = ServiceStatusCell::SERVICE_DOWN; break;
        case 2: sResult = ServiceStatusCell::SERVICE_MUMBLE; break;
        case 3: sResult = ServiceStatusCell::SERVICE_CORRUPT; break;
        case 4: sResult = ServiceStatusCell::SERVICE_SHIT; break;
    }
    return sResult;
}

// ----------------------------------------------------------------------

void Scoreboard::setServiceStatus(const std::string &sTeamId, const std::string &sServiceId, const std::string &sStatus) {
    std::lock_guard<std::mutex> lock(m_mutexJson);
    std::string sNewStatus = m_bRandom ? randomServiceStatus() : sStatus;

    std::map<std::string,TeamStatusRow *>::iterator it;
    it = m_mapTeamsStatuses.find(sTeamId);
    if (it != m_mapTeamsStatuses.end()) {
        if (it->second->serviceStatus(sServiceId) != sNewStatus) {
            it->second->setServiceStatus(sServiceId, sNewStatus);
            m_jsonScoreboard["scoreboard"][sTeamId]["ts_sta"][sServiceId]["status"] = sNewStatus;
        }
    }
}

// ----------------------------------------------------------------------

void Scoreboard::incrementTries(const std::string &sTeamId) {
    std::lock_guard<std::mutex> lock(m_mutexJson);
    std::map<std::string,TeamStatusRow *>::iterator it;
    it = m_mapTeamsStatuses.find(sTeamId);
    if (it != m_mapTeamsStatuses.end()) {
        it->second->setTries(it->second->tries() + 1);
        m_jsonScoreboard["scoreboard"][sTeamId]["tries"] = it->second->tries();
    }
}

// ----------------------------------------------------------------------

void Scoreboard::initStateFromStorage() {
    // load flag lives
    std::vector<Flag> vFlagLives = m_pStorage->listOfLiveFlags();
    for (unsigned int i = 0; i < vFlagLives.size(); i++) {
        Flag flag = vFlagLives[i];
        m_mapFlagsLive[flag.value()] = flag;
    }

    // load services statistics
    m_nAllStolenFlags = 0;
    m_nAllDefenceFlags = 0;
    for (unsigned int i = 0; i < m_vServices.size(); i++) {
        std::string sServiceID = m_vServices[i].id();
        
        int nStolenFlags = m_pStorage->numberOfStolenFlagsForService(sServiceID);
        m_mapServiceCostsAndStatistics[sServiceID]->setStolenFlagsForService(nStolenFlags);
        m_nAllStolenFlags += nStolenFlags;

        int nDefenceFlags = m_pStorage->numberOfDefenceFlagForService(sServiceID);
        m_mapServiceCostsAndStatistics[sServiceID]->setDefenceFlagsForService(nDefenceFlags);
        m_nAllDefenceFlags += nDefenceFlags;
    }

    std::map<std::string, TeamStatusRow *>::iterator it;
    for (it = m_mapTeamsStatuses.begin(); it != m_mapTeamsStatuses.end(); it++) {
        TeamStatusRow *pRow = it->second;

        int nTries = m_pStorage->numberOfFlagAttempts(pRow->teamId());
        pRow->setTries(nTries);
        m_jsonScoreboard["scoreboard"][pRow->teamId()]["tries"] = nTries;

        for (unsigned int i = 0; i < m_vServices.size(); i++) {
            std::string sServiceID = m_vServices[i].id();
            
            // calculate defence
            int nDefenceFlags = m_pStorage->getDefenceFlags(pRow->teamId(), sServiceID);
            int nDefencePoints = m_pStorage->getDefencePoints(pRow->teamId(), sServiceID);
            pRow->setServiceDefenceFlagsAndPoints(sServiceID, nDefenceFlags, nDefencePoints);
            m_jsonScoreboard["scoreboard"][pRow->teamId()]["ts_sta"][sServiceID]["def"] = nDefenceFlags;
            m_jsonScoreboard["scoreboard"][pRow->teamId()]["ts_sta"][sServiceID]["pt_def"] = double(nDefencePoints) / 10.0;

            // calculate attack
            int nAttackFlags = m_pStorage->getStollenFlags(pRow->teamId(), sServiceID);
            int nAttackPoints = m_pStorage->getStollenPoints(pRow->teamId(), sServiceID);
            pRow->setServiceAttackFlagsAndPoints(sServiceID, nAttackFlags, nAttackPoints);
            m_jsonScoreboard["scoreboard"][pRow->teamId()]["ts_sta"][sServiceID]["att"] = nAttackFlags;
            m_jsonScoreboard["scoreboard"][pRow->teamId()]["ts_sta"][sServiceID]["pt_att"] = double(nAttackPoints) / 10.0;

            // calculate uptime
            int nFlagsSuccess = m_pStorage->numberOfFlagSuccessPutted(pRow->teamId(), sServiceID);
            pRow->setServiceFlagsPutted(sServiceID, nFlagsSuccess);

            // TODO
            // int nUpPointTimeInSec = m_pStorage->lastUpPointTime(pRow->teamId(), sServiceID);
            // pRow->setUpPointTime(sServiceID, nUpPointTimeInSec);
            // setUpPointTime
        }
    }

    {
        std::lock_guard<std::mutex> lock(m_mutexJson);
        sortPlaces();
        updateCosts();
    }
    
}

// ----------------------------------------------------------------------

int Scoreboard::incrementAttackScore(const Flag &flag, const std::string &sTeamId) {
    std::lock_guard<std::mutex> lock(m_mutexJson);
    std::string sServiceId = flag.serviceId();

    int nFlagPoints = m_mapServiceCostsAndStatistics[flag.serviceId()]->costStolenFlag()*10; // one number after dot
    m_pStorage->insertToFlagsStolen(flag, sTeamId, nFlagPoints);

    std::map<std::string,TeamStatusRow *>::iterator it;
    it = m_mapTeamsStatuses.find(sTeamId);
    if (it != m_mapTeamsStatuses.end()) {
        TeamStatusRow *pRow = it->second; 
        pRow->incrementAttack(sServiceId, nFlagPoints);
        m_jsonScoreboard["scoreboard"][sTeamId]["ts_sta"][sServiceId]["att"] = pRow->getAttackFlags(sServiceId);
        m_jsonScoreboard["scoreboard"][sTeamId]["ts_sta"][sServiceId]["pt_att"] = double(pRow->getAttackPoints(sServiceId)) / 10.0;
        m_jsonScoreboard["scoreboard"][sTeamId]["points"] = double(pRow->getPoints()) / 10.0;
        sortPlaces();
    }

    std::map<std::string,ServiceCostsAndStatistics *>::iterator it2;
    it2 = m_mapServiceCostsAndStatistics.find(sServiceId);
    if (it2 != m_mapServiceCostsAndStatistics.end()) {
        it2->second->incrementStolenFlagsForService();
        m_nAllStolenFlags++;
        updateCosts(); // TODO update only stolen costs
    }
    return nFlagPoints;
}

// ----------------------------------------------------------------------

void Scoreboard::incrementDefenceScore(const Flag &flag) {
    std::lock_guard<std::mutex> lock(m_mutexJson);

    std::string sTeamId = flag.teamId();
    std::string sServiceId = flag.serviceId();
    int nFlagPoints = m_mapServiceCostsAndStatistics[flag.serviceId()]->costDefenceFlag()*10; // one number after dot
    m_pStorage->insertToFlagsDefence(flag, nFlagPoints);

    std::map<std::string,TeamStatusRow *>::iterator it;
    it = m_mapTeamsStatuses.find(sTeamId);
    if (it != m_mapTeamsStatuses.end()) {
        TeamStatusRow *pRow = it->second;
        pRow->incrementDefence(sServiceId, nFlagPoints);
        m_jsonScoreboard["scoreboard"][sTeamId]["ts_sta"][sServiceId]["def"] = pRow->getDefenceFlags(sServiceId);
        m_jsonScoreboard["scoreboard"][sTeamId]["ts_sta"][sServiceId]["pt_def"] = double(pRow->getDefencePoints(sServiceId)) / 10.0;
        m_jsonScoreboard["scoreboard"][sTeamId]["points"] = double(pRow->getPoints()) / 10.0;
        sortPlaces();
    }

    std::map<std::string,ServiceCostsAndStatistics *>::iterator it2;
    it2 = m_mapServiceCostsAndStatistics.find(sServiceId);
    if (it2 != m_mapServiceCostsAndStatistics.end()) {
        it2->second->incrementDefenceFlagsForService();
        m_nAllDefenceFlags++;
        updateCosts();  // TODO update only defence costs
    }
    Log::err(TAG, "CostDefenceFlagForService " + sServiceId + " m_nAllDefenceFlags = " + std::to_string(m_nAllDefenceFlags));
}

// ----------------------------------------------------------------------

void Scoreboard::incrementFlagsPuttedAndServiceUp(const Flag &flag) {
    std::string sServiceId = flag.serviceId();
    std::string sTeamId = flag.teamId();
    std::string sNewStatus = m_bRandom ? randomServiceStatus() : ServiceStatusCell::SERVICE_UP;

    // insert flag lives
    {
        std::lock_guard<std::mutex> lock(m_mutexFlagsLive);
        std::map<std::string,Flag>::iterator it;
        it = m_mapFlagsLive.find(flag.value());
        if (it != m_mapFlagsLive.end()) {
            Log::warn(TAG, flag.value() + " - flag already exists");
        } else {
            m_mapFlagsLive[flag.value()] = flag;
            m_pStorage->insertFlagLive(flag);
        }
    }

    // success putted
    std::lock_guard<std::mutex> lock(m_mutexJson);
    std::map<std::string,TeamStatusRow *>::iterator it;
    it = m_mapTeamsStatuses.find(sTeamId);
    if (it != m_mapTeamsStatuses.end()) {
        TeamStatusRow *pRow = it->second;
        if (pRow->serviceStatus(sServiceId) != sNewStatus) {
            pRow->setServiceStatus(sServiceId, sNewStatus);
        }
	    m_jsonScoreboard["scoreboard"][sTeamId]["ts_sta"][sServiceId]["status"] = sNewStatus;
        m_jsonScoreboard["scoreboard"][sTeamId]["ts_sta"][sServiceId]["upt"] = pRow->serviceUptime(sServiceId);
        m_jsonScoreboard["scoreboard"][sTeamId]["points"] = double(pRow->getPoints()) / 10.0;
        sortPlaces();
        updateCosts();
    }
    // TODO update Costs
    // updateScore(flag.teamId(), flag.serviceId());
}

// ----------------------------------------------------------------------

void Scoreboard::insertFlagPutFail(const Flag &flag, const std::string &sServiceStatus, const std::string &sDescrStatus) {
    std::string sServiceId = flag.serviceId();
    std::string sTeamId = flag.teamId();
    std::string sNewStatus = m_bRandom ? randomServiceStatus() : sServiceStatus;
    m_pStorage->insertFlagPutFail(flag, sDescrStatus);

    std::lock_guard<std::mutex> lock(m_mutexJson);
    std::map<std::string,TeamStatusRow *>::iterator it;
    it = m_mapTeamsStatuses.find(flag.teamId());
    if (it != m_mapTeamsStatuses.end()) {
        TeamStatusRow *pRow = it->second;
        if (pRow->serviceStatus(sServiceId) != sNewStatus) {
            pRow->setServiceStatus(sServiceId, sNewStatus);
        }
        m_jsonScoreboard["scoreboard"][sTeamId]["ts_sta"][sServiceId]["status"] = sNewStatus;
    }
    // TODO update score
    // updateScore(flag.teamId(), flag.serviceId());
}

// ----------------------------------------------------------------------

void Scoreboard::updateScore(const std::string &sTeamId, const std::string &sServiceId) {
    std::lock_guard<std::mutex> lock(m_mutexJson);
    std::map<std::string,TeamStatusRow *>::iterator it;
    it = m_mapTeamsStatuses.find(sTeamId);
    if (it != m_mapTeamsStatuses.end()) {
        TeamStatusRow *pRow = it->second;
        pRow->updateScore(sServiceId);
        m_jsonScoreboard["scoreboard"][sTeamId]["ts_sta"][sServiceId]["upt"] = pRow->serviceUptime(sServiceId);
        m_jsonScoreboard["scoreboard"][sTeamId]["points"] = double(pRow->getPoints()) / 10.0;
        sortPlaces();
    }
}

// ----------------------------------------------------------------------

std::string Scoreboard::serviceStatus(const std::string &sTeamId, const std::string &sServiceId) {
    std::map<std::string,TeamStatusRow *>::iterator it;
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

void Scoreboard::sortPlaces() {
    // std::lock_guard<std::mutex> lock(m_mutexJson);
    // sort places
    {
        std::vector<int> vScores;
        std::map<std::string,TeamStatusRow *>::iterator it1;
        for (it1 = m_mapTeamsStatuses.begin(); it1 != m_mapTeamsStatuses.end(); it1++) {
            if(std::find(vScores.begin(), vScores.end(), it1->second->getPoints()) == vScores.end()) {
                vScores.push_back(it1->second->getPoints());
            }
        }
        std::sort(vScores.begin(), vScores.end(), sort_using_greater_than);
        for (it1 = m_mapTeamsStatuses.begin(); it1 != m_mapTeamsStatuses.end(); it1++) {
            int nPoints = it1->second->getPoints();
            ptrdiff_t pos = std::find(vScores.begin(), vScores.end(), nPoints) - vScores.begin();
            it1->second->setPlace(pos + 1); // TODO fix: same scores will be same place
        }
    }

    // update json
    {
        std::map<std::string,TeamStatusRow *>::iterator it1;
        for (it1 = m_mapTeamsStatuses.begin(); it1 != m_mapTeamsStatuses.end(); it1++) {
            TeamStatusRow *pTeamStatus = it1->second;
            std::string sTeamId_ = pTeamStatus->teamId();

            // std::cout << sTeamNum << ": result: score: " << pTeamStatus->score() << ", place: " << pTeamStatus->place() << "\n";
            m_jsonScoreboard["scoreboard"][sTeamId_]["points"] = double(pTeamStatus->getPoints()) / 10.0;
            m_jsonScoreboard["scoreboard"][sTeamId_]["place"] = pTeamStatus->place();
            m_jsonScoreboard["scoreboard"][sTeamId_]["tries"] = pTeamStatus->tries();
        }
    }
}

// ----------------------------------------------------------------------

void Scoreboard::updateCosts() {
    // std::lock_guard<std::mutex> lock(m_mutexJson);
    // TODO update costs
    std::map<std::string,ServiceCostsAndStatistics *>::iterator it1;
    double nSumOfReverseProportionalStolenFlags = 0.0;
    double nSumOfReverseProportionalDefenceFlags = 0.0;
    double sf = m_vServices.size() * m_nBacisCostsStolenFlagInPoints;
    double df = m_vServices.size() * (m_mapTeamsStatuses.size() - 1) * m_nBacisCostsStolenFlagInPoints;

    // calculate 
    for (it1 = m_mapServiceCostsAndStatistics.begin(); it1 != m_mapServiceCostsAndStatistics.end(); it1++) {
        nSumOfReverseProportionalStolenFlags += it1->second->updateProportionalStolenFlagsForService(m_nAllStolenFlags);
        nSumOfReverseProportionalDefenceFlags += it1->second->updateProportionalDefenceFlagsForService(m_nAllDefenceFlags);
    }

    Log::err(TAG, "CostDefenceFlagForService nSumOfReverseProportionalDefenceFlags = " + std::to_string(nSumOfReverseProportionalDefenceFlags));

    for (it1 = m_mapServiceCostsAndStatistics.begin(); it1 != m_mapServiceCostsAndStatistics.end(); it1++) {
        it1->second->updateCostStolenFlagForService(sf, nSumOfReverseProportionalStolenFlags);

        double r = it1->second->updateCostDefenceFlagForService(df, nSumOfReverseProportionalDefenceFlags);
        Log::err(TAG, "CostDefenceFlagForService " + it1->first + " " + std::to_string(r));
    }

    nlohmann::json jsonCosts;
    for (unsigned int iservice = 0; iservice < m_vServices.size(); iservice++) {
        Service serviceConf = m_vServices[iservice];
        std::string sId = serviceConf.id();
        m_jsonScoreboard["s_sta"][sId]["cost_att"] = m_mapServiceCostsAndStatistics[sId]->costStolenFlag();
        m_jsonScoreboard["s_sta"][sId]["cost_def"] = m_mapServiceCostsAndStatistics[sId]->costDefenceFlag();
        m_jsonScoreboard["s_sta"][sId]["af_att"] = m_mapServiceCostsAndStatistics[sId]->allStolenFlagsForService();
        m_jsonScoreboard["s_sta"][sId]["af_def"] = m_mapServiceCostsAndStatistics[sId]->allDefenceFlagsForService();
        m_jsonScoreboard["s_sta"][sId]["first_blood"] = m_mapServiceCostsAndStatistics[sId]->getFirstBloodTeamId();
    }
}

// ----------------------------------------------------------------------

/*void Scoreboard::addFlagLive(const Flag &flag) {
    std::lock_guard<std::mutex> lock(m_mutexFlagsLive);
    std::map<std::string,Flag>::iterator it;
    it = m_mapFlagsLive.find(flag.value());
    if (it != m_mapFlagsLive.end()) {
        Log::warn(TAG, flag.value() + " - flag already exists");
    } else {
        m_mapFlagsLive[flag.value()] = flag;
        m_pStorage->insertFlagLive(flag);
    }
}*/

// ----------------------------------------------------------------------

std::vector<Flag> Scoreboard::outdatedFlagsLive(const std::string &sTeamId, const std::string &sServiceId) {
    std::lock_guard<std::mutex> lock(m_mutexFlagsLive);
    std::vector<Flag> vResult;
    long nCurrentTime = WsjcppCore::currentTime_milliseconds() - m_nFlagTimeLiveInSec*1000;
    std::map<std::string,Flag>::iterator it;
    for (it = m_mapFlagsLive.begin(); it != m_mapFlagsLive.end(); it++) {
        Flag flag = it->second;
        if (flag.teamId() == sTeamId 
            && flag.serviceId() == sServiceId
            && flag.timeEnd() < nCurrentTime
        ) {
            vResult.push_back(flag);
        }
    }
    return vResult;
}

// ----------------------------------------------------------------------

bool Scoreboard::findFlagLive(const std::string &sFlagValue, Flag &flag) {
    std::lock_guard<std::mutex> lock(m_mutexFlagsLive);
    std::map<std::string,Flag>::iterator it = m_mapFlagsLive.find(sFlagValue);
    if (it != m_mapFlagsLive.end()) {
        flag.copyFrom(it->second);
        return true;
    }
    return false;
}

// ----------------------------------------------------------------------

void Scoreboard::removeFlagLive(const Flag &flag) {
    std::lock_guard<std::mutex> lock(m_mutexFlagsLive);
    std::map<std::string,Flag>::iterator it;
    it = m_mapFlagsLive.find(flag.value());
    if (it != m_mapFlagsLive.end()) {
        m_mapFlagsLive.erase(it);
        m_pStorage->deleteFlagLive(flag);
    } else {
        Log::warn(TAG, flag.value() + " - flag did not exists");
    }
}

// ----------------------------------------------------------------------

std::string Scoreboard::toString(){
    // TODO mutex
    std::string sResult = ""; 
    std::map<std::string,TeamStatusRow *>::iterator it;
    for (it = m_mapTeamsStatuses.begin(); it != m_mapTeamsStatuses.end(); ++it){
        sResult += it->first + ": \n"
            "\tpoints: " + std::to_string(it->second->getPoints()) + "\n"
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
