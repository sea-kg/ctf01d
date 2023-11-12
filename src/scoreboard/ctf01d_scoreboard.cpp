/**********************************************************************************
 *          Project
 *  _______ _________ _______  _______  __    ______
 * (  ____ \\__   __/(  ____ \(  __   )/  \  (  __  \
 * | (    \/   ) (   | (    \/| (  )  |\/) ) | (  \  )
 * | |         | |   | (__    | | /   |  | | | |   ) |
 * | |         | |   |  __)   | (/ /) |  | | | |   | |
 * | |         | |   | (      |   / | |  | | | |   ) |
 * | (____/\   | |   | )      |  (__) |__) (_| (__/  )
 * (_______/   )_(   |/       (_______)\____/(______/
 *
 * MIT License
 * Copyright (c) 2018-2023 Evgenii Sopov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ***********************************************************************************/

#include "ctf01d_scoreboard.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <wsjcpp_core.h>
#include <employ_config.h>
#include <employ_flags.h>

// ---------------------------------------------------------------------
// Scoreboard

Ctf01dScoreboard::Ctf01dScoreboard(
    bool bRandom,
    int nGameStartInSec,
    int nGameEndInSec,
    int nGameCoffeeBreakStartInSec,
    int nGameCoffeeBreakEndInSec,
    int nFlagTimeLiveInSec,
    Storage *pStorage
) {
    EmployConfig *pEmployConfig = findWsjcppEmploy<EmployConfig>();
    m_pDatabase = findWsjcppEmploy<EmployDatabase>();
    const std::vector<Ctf01dTeamDef> &vTeamsConf = pEmployConfig->teamsConf();
    const std::vector<Ctf01dServiceDef> &vServicesConf = pEmployConfig->servicesConf();

    TAG = "Ctf01dScoreboard";
    m_bRandom = bRandom;
    std::string sScroreboardRandom = "Scoreboard random: ";
    sScroreboardRandom = sScroreboardRandom + (m_bRandom ? "yes" : "no");
    WsjcppLog::warn(TAG, sScroreboardRandom);
    m_pStorage = pStorage;
    std::srand(unsigned(std::time(0)));
    m_nGameStartInSec = nGameStartInSec;
    m_nGameEndInSec = nGameEndInSec;
    m_nGameCoffeeBreakStartInSec = nGameCoffeeBreakStartInSec;
    m_nGameCoffeeBreakEndInSec = nGameCoffeeBreakEndInSec;
    m_nFlagTimeLiveInSec = nFlagTimeLiveInSec;
    m_nAllStolenFlags = 0;
    m_nAllDefenceFlags = 0;
    m_pEmployFlags = findWsjcppEmploy<EmployFlags>();

    m_mapTeamsStatuses.clear(); // possible memory leak
    for (unsigned int iteam = 0; iteam < vTeamsConf.size(); iteam++) {
        Ctf01dTeamDef teamConf = vTeamsConf[iteam];
        std::string sTeamId = teamConf.getId();

        m_mapTeamsStatuses[sTeamId] = new TeamStatusRow(sTeamId, nGameStartInSec, nGameEndInSec);
        m_mapTeamsStatuses[sTeamId]->setPlace(iteam+1);

        // random values of service for testing
        if (m_bRandom) {
            int nPoints = (std::rand() % 10000);
            m_mapTeamsStatuses[sTeamId]->setPoints(nPoints);
        }

        for (unsigned int iservice = 0; iservice < vServicesConf.size(); iservice++) {
            Ctf01dServiceDef service = vServicesConf[iservice];
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
        std::string sServiceId = vServicesConf[i].id();
        m_mapServiceCostsAndStatistics[sServiceId] = new Ctf01dServiceCostsAndStatistics(
            sServiceId,
            pEmployConfig->getBasicCostsStolenFlagInPoints(),
            vServicesConf.size(),
            vTeamsConf.size()
        );
    }

    initJsonScoreboard();
}

void Ctf01dScoreboard::initJsonScoreboard() {
    std::lock_guard<std::mutex> lock(m_mutexJson);
    m_jsonScoreboard.clear();
    EmployConfig *pConfig = findWsjcppEmploy<EmployConfig>();
    const std::vector<Ctf01dTeamDef> &vTeamsConf = pConfig->teamsConf();
    const std::vector<Ctf01dServiceDef> &vServices = pConfig->servicesConf();

    nlohmann::json jsonCosts;
    for (unsigned int iservice = 0; iservice < vServices.size(); iservice++) {
        Ctf01dServiceDef serviceConf = vServices[iservice];
        nlohmann::json serviceCosts;
        m_mapServiceCostsAndStatistics[serviceConf.id()]->updateJsonCosts(serviceCosts);
        jsonCosts[serviceConf.id()] = serviceCosts;
    }
    m_jsonScoreboard["s_sta"] = jsonCosts;

    nlohmann::json jsonScoreboard;
    for (unsigned int iteam = 0; iteam < vTeamsConf.size(); iteam++) {
        Ctf01dTeamDef teamConf = vTeamsConf[iteam];
        std::string sTeamId = teamConf.getId();
        nlohmann::json teamData;
        teamData["place"] = m_mapTeamsStatuses[sTeamId]->place();
        teamData["points"] = double(m_mapTeamsStatuses[sTeamId]->getPoints()) / 10.0;
        teamData["tries"] = 0;
        nlohmann::json jsonServices;
        for (unsigned int iservice = 0; iservice < vServices.size(); iservice++) {
            Ctf01dServiceDef serviceConf = vServices[iservice];
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
        jsonScoreboard[teamConf.getId()] = teamData;
    }
    m_jsonScoreboard["scoreboard"] = jsonScoreboard;
    nlohmann::json jsonGame;
    jsonGame["t0"] = pConfig->gameStartUTCInSec();
    jsonGame["t1"] = pConfig->gameCoffeeBreakStartUTCInSec();
    jsonGame["t2"] = pConfig->gameCoffeeBreakEndUTCInSec();
    jsonGame["t3"] = pConfig->gameEndUTCInSec();
    jsonGame["tc"] = WsjcppCore::getCurrentTimeInSeconds();
    m_jsonScoreboard["game"] = jsonGame;
}

void Ctf01dScoreboard::updateJsonScoreboard() {
    std::lock_guard<std::mutex> lock(m_mutexJson);

    // TODO update score
    // TODO update costs
}

std::string Ctf01dScoreboard::randomServiceStatus() {
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

void Ctf01dScoreboard::setServiceStatus(const std::string &sTeamId, const std::string &sServiceId, const std::string &sStatus) {
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

void Ctf01dScoreboard::incrementTries(const std::string &sTeamId) {
    std::lock_guard<std::mutex> lock(m_mutexJson);
    std::map<std::string,TeamStatusRow *>::iterator it;
    it = m_mapTeamsStatuses.find(sTeamId);
    if (it != m_mapTeamsStatuses.end()) {
        it->second->setTries(it->second->tries() + 1);
        m_jsonScoreboard["scoreboard"][sTeamId]["tries"] = it->second->tries();
    }
}

void Ctf01dScoreboard::initStateFromStorage() {
    EmployConfig *pConfig = findWsjcppEmploy<EmployConfig>();
    const std::vector<Ctf01dServiceDef> &vServices = pConfig->servicesConf();

    // load flag lives
    std::vector<Ctf01dFlag> vFlagLives = m_pStorage->listOfLiveFlags();
    for (unsigned int i = 0; i < vFlagLives.size(); i++) {
        Ctf01dFlag flag = vFlagLives[i];
        m_mapFlagsLive[flag.getValue()] = flag;
    }

    // load services statistics
    m_nAllStolenFlags = 0;
    m_nAllDefenceFlags = 0;
    struct FlagsForService {
        std::string sServiceID;
        int nStolenFlags;
        int nDefenceFlags;
    };
    std::vector<FlagsForService> vFlags;
    for (unsigned int i = 0; i < vServices.size(); i++) {
        std::string sServiceID = vServices[i].id();
        FlagsForService f;
        f.sServiceID = sServiceID;
        f.nStolenFlags = m_pStorage->numberOfStolenFlagsForService(sServiceID);
        f.nDefenceFlags = m_pStorage->numberOfDefenceFlagForService(sServiceID);

        m_nAllStolenFlags += f.nStolenFlags;
        m_nAllDefenceFlags += f.nDefenceFlags;
        vFlags.push_back(f);
    }

    for (int i = 0; i < vFlags.size(); i++) {
        FlagsForService f = vFlags[i];
        m_mapServiceCostsAndStatistics[f.sServiceID]->updateProportionalStolenFlagsForService(f.nStolenFlags, m_nAllStolenFlags);
        m_mapServiceCostsAndStatistics[f.sServiceID]->updateProportionalDefenceFlagsForService(f.nDefenceFlags, m_nAllDefenceFlags);
    }

    std::map<std::string, TeamStatusRow *>::iterator it;
    for (it = m_mapTeamsStatuses.begin(); it != m_mapTeamsStatuses.end(); it++) {
        TeamStatusRow *pRow = it->second;

        int nTries = m_pStorage->numberOfFlagAttempts(pRow->teamId());
        pRow->setTries(nTries);
        m_jsonScoreboard["scoreboard"][pRow->teamId()]["tries"] = nTries;

        for (unsigned int i = 0; i < vServices.size(); i++) {
            std::string sServiceID = vServices[i].id();

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

int Ctf01dScoreboard::incrementAttackScore(const Ctf01dFlag &flag, const std::string &sTeamId) {
    std::lock_guard<std::mutex> lock(m_mutexJson);
    std::string sServiceId = flag.getServiceId();

    int nFlagPoints = m_mapServiceCostsAndStatistics[sServiceId]->getCostStolenFlag()*10; // one number after dot
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

    std::map<std::string, Ctf01dServiceCostsAndStatistics *>::iterator it2;
    it2 = m_mapServiceCostsAndStatistics.find(sServiceId);
    if (it2 != m_mapServiceCostsAndStatistics.end()) {
        m_nAllStolenFlags++;
        it2->second->doIncrementStolenFlagsForService(m_nAllStolenFlags);
        updateCosts(); // TODO update only stolen costs
    }
    return nFlagPoints;
}

void Ctf01dScoreboard::incrementDefenceScore(const Ctf01dFlag &flag) {
    std::lock_guard<std::mutex> lock(m_mutexJson);

    std::string sTeamId = flag.getTeamId();
    std::string sServiceId = flag.getServiceId();
    int nFlagPoints = m_mapServiceCostsAndStatistics[sServiceId]->getCostDefenceFlag()*10; // one number after dot
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

    std::map<std::string, Ctf01dServiceCostsAndStatistics *>::iterator it2;
    it2 = m_mapServiceCostsAndStatistics.find(sServiceId);
    if (it2 != m_mapServiceCostsAndStatistics.end()) {
        m_nAllDefenceFlags++;
        it2->second->doIncrementDefenceFlagsForService(m_nAllDefenceFlags);
        updateCosts();  // TODO update only defence costs
    }
    WsjcppLog::err(TAG, "CostDefenceFlagForService " + sServiceId + " m_nAllDefenceFlags = " + std::to_string(m_nAllDefenceFlags));
}

void Ctf01dScoreboard::incrementFlagsPuttedAndServiceUp(const Ctf01dFlag &flag) {
    std::string sServiceId = flag.getServiceId();
    std::string sTeamId = flag.getTeamId();
    std::string sNewStatus = m_bRandom ? randomServiceStatus() : ServiceStatusCell::SERVICE_UP;

    // insert flag lives
    {
        std::lock_guard<std::mutex> lock(m_mutexFlagsLive);
        std::map<std::string,Ctf01dFlag>::iterator it;
        it = m_mapFlagsLive.find(flag.getValue());
        if (it != m_mapFlagsLive.end()) {
            WsjcppLog::warn(TAG, flag.getValue() + " - flag already exists");
        } else {
            m_mapFlagsLive[flag.getValue()] = flag;
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
        m_jsonScoreboard["scoreboard"][sTeamId]["points"] = double(pRow->getPoints()) / 10.0;
        sortPlaces();
        updateCosts();
    }
    // TODO update Costs
    // updateScore(flag.teamId(), flag.serviceId());
}

void Ctf01dScoreboard::insertFlagPutFail(const Ctf01dFlag &flag, const std::string &sServiceStatus, const std::string &sDescrStatus) {
    m_pDatabase->insertToFlagPutFail(flag, sDescrStatus);

    std::lock_guard<std::mutex> lock(m_mutexJson);

    std::string sServiceId = flag.getServiceId();
    std::string sTeamId = flag.getTeamId();
    std::string sNewStatus = m_bRandom ? randomServiceStatus() : sServiceStatus;
    // WsjcppLog::info(TAG, "Ctf01dScoreboard::insert FlagPutFail 1");
    // WsjcppLog::info(TAG, "Ctf01dScoreboard::insert FlagPutFail sServiceId " + sServiceId);
    // WsjcppLog::info(TAG, "Ctf01dScoreboard::insert FlagPutFail sTeamId " + sTeamId);
    // WsjcppLog::info(TAG, "Ctf01dScoreboard::insert FlagPutFail sNewStatus " + sNewStatus);
    // WsjcppLog::info(TAG, "Ctf01dScoreboard::insert FlagPutFail sDescrStatus " + sDescrStatus);
    // WsjcppLog::info(TAG, "Ctf01dScoreboard::insert FlagPutFail flag.getId() " + flag.getId());
    // WsjcppLog::info(TAG, "Ctf01dScoreboard::insert FlagPutFail flag.getValue() " + flag.getValue());

    std::map<std::string,TeamStatusRow *>::iterator it;
    it = m_mapTeamsStatuses.find(flag.getTeamId());
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

void Ctf01dScoreboard::updateScore(const std::string &sTeamId, const std::string &sServiceId) {
    std::lock_guard<std::mutex> lock(m_mutexJson);
    std::map<std::string,TeamStatusRow *>::iterator it;
    it = m_mapTeamsStatuses.find(sTeamId);
    if (it != m_mapTeamsStatuses.end()) {
        TeamStatusRow *pRow = it->second;
        pRow->updateScore(sServiceId);
        m_jsonScoreboard["scoreboard"][sTeamId]["points"] = double(pRow->getPoints()) / 10.0;
        sortPlaces();
    }
}

std::string Ctf01dScoreboard::serviceStatus(const std::string &sTeamId, const std::string &sServiceId) {
    std::map<std::string,TeamStatusRow *>::iterator it;
    it = m_mapTeamsStatuses.find(sTeamId);
    if (it != m_mapTeamsStatuses.end()) {
        return it->second->serviceStatus(sServiceId);
    }
    return "";
}

static bool sort_using_greater_than(double u, double v) {
   return u > v;
}

void Ctf01dScoreboard::sortPlaces() {
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

void Ctf01dScoreboard::updateCosts() {
    // std::lock_guard<std::mutex> lock(m_mutexJson);
    // TODO update costs
    std::map<std::string, Ctf01dServiceCostsAndStatistics *>::iterator it1;
    double nSumOfReverseProportionalStolenFlags = 0.0;
    double nSumOfReverseProportionalDefenceFlags = 0.0;

    // calculate 
    for (it1 = m_mapServiceCostsAndStatistics.begin(); it1 != m_mapServiceCostsAndStatistics.end(); it1++) {
        nSumOfReverseProportionalStolenFlags += it1->second->updateProportionalStolenFlagsForService(m_nAllStolenFlags);
        nSumOfReverseProportionalDefenceFlags += it1->second->updateProportionalDefenceFlagsForService(m_nAllDefenceFlags);
    }

    WsjcppLog::err(TAG, "CostDefenceFlagForService nSumOfReverseProportionalDefenceFlags = " + std::to_string(nSumOfReverseProportionalDefenceFlags));

    for (it1 = m_mapServiceCostsAndStatistics.begin(); it1 != m_mapServiceCostsAndStatistics.end(); it1++) {
        it1->second->updateCostStolenFlag(nSumOfReverseProportionalStolenFlags);
        it1->second->updateCostDefenceFlag(nSumOfReverseProportionalDefenceFlags);
        // WsjcppLog::err(TAG, "CostDefenceFlagForService " + it1->first + " " + std::to_string(r));
    }

    // nlohmann::json jsonCosts;
    for (it1 = m_mapServiceCostsAndStatistics.begin(); it1 != m_mapServiceCostsAndStatistics.end(); it1++) {
        std::string sId = it1->first;
        it1->second->updateJsonCosts(m_jsonScoreboard["s_sta"][sId]);
    }
}

/*void Ctf01dScoreboard::addFlagLive(const Flag &flag) {
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

std::vector<Ctf01dFlag> Ctf01dScoreboard::outdatedFlagsLive(const std::string &sTeamId, const std::string &sServiceId) {
    std::lock_guard<std::mutex> lock(m_mutexFlagsLive);
    std::vector<Ctf01dFlag> vResult;
    long nCurrentTime = WsjcppCore::getCurrentTimeInMilliseconds() - m_nFlagTimeLiveInSec*1000;
    std::map<std::string,Ctf01dFlag>::iterator it;
    for (it = m_mapFlagsLive.begin(); it != m_mapFlagsLive.end(); it++) {
        Ctf01dFlag flag = it->second;
        if (flag.getTeamId() == sTeamId
            && flag.getServiceId() == sServiceId
            && flag.getTimeEndInMs() < nCurrentTime
        ) {
            vResult.push_back(flag);
        }
    }
    return vResult;
}

bool Ctf01dScoreboard::findFlagLive(const std::string &sFlagValue, Ctf01dFlag &flag) {
    std::lock_guard<std::mutex> lock(m_mutexFlagsLive);
    std::map<std::string,Ctf01dFlag>::iterator it = m_mapFlagsLive.find(sFlagValue);
    if (it != m_mapFlagsLive.end()) {
        flag.copyFrom(it->second);
        return true;
    }
    return false;
}

void Ctf01dScoreboard::removeFlagLive(const Ctf01dFlag &flag) {
    std::lock_guard<std::mutex> lock(m_mutexFlagsLive);
    std::map<std::string,Ctf01dFlag>::iterator it;
    it = m_mapFlagsLive.find(flag.getValue());
    if (it != m_mapFlagsLive.end()) {
        m_mapFlagsLive.erase(it);
        m_pStorage->deleteFlagLive(flag);
    } else {
        WsjcppLog::warn(TAG, flag.getValue() + " - flag did not exists");
    }
}

std::string Ctf01dScoreboard::toString(){
    // TODO mutex
    std::string sResult = "";
    std::map<std::string, TeamStatusRow *>::iterator it;
    for (it = m_mapTeamsStatuses.begin(); it != m_mapTeamsStatuses.end(); ++it){
        sResult += it->first + ": \n"
            "\tpoints: " + std::to_string(it->second->getPoints()) + "\n"
            + it->second->servicesToString() + "\n";
    }
    return sResult;
}

const nlohmann::json &Ctf01dScoreboard::toJson(){
    std::lock_guard<std::mutex> lock(m_mutexJson);
    m_jsonScoreboard["game"]["tc"] = WsjcppCore::getCurrentTimeInSeconds();
    return m_jsonScoreboard;
}
