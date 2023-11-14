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

#include "employ_scoreboard.h"
#include <wsjcpp_core.h>
#include <employ_config.h>
#include <cmath>
#include <stdio.h>
#include <string>
#include <map>
#include <mutex>
#include <vector>

// ---------------------------------------------------------------------
// Ctf01dServiceCostsAndStatistics

Ctf01dServiceCostsAndStatistics::Ctf01dServiceCostsAndStatistics(
    const std::string &sServiceId,
    int nBasicCostOfStolenFlag,
    int nServicesSize,
    int nTeamsSize
) {
    TAG = "Ctf01dServiceCostsAndStatistics-" + sServiceId;
    m_nBasicCostOfStolenFlag = nBasicCostOfStolenFlag;
    m_nServicesSize = nServicesSize;
    m_nTeamsSize = nTeamsSize;
    m_nBasicCostOfDefenceFlag = m_nServicesSize * (m_nTeamsSize - 1) * m_nBasicCostOfStolenFlag;
    m_sServiceId = sServiceId;
    m_nAllStolenFlagsForService = 0;
    m_nReverseProportionalStolenFlags = 0.0;
    m_nAllDefenceFlagsForService = 0;
    m_nReverseProportionalDefenceFlags = 0.0;
    m_nCostStolenFlag = m_nBasicCostOfStolenFlag;
    m_nCostDefenceFlag = m_nBasicCostOfDefenceFlag;
    m_sFirstBloodTeamId = "?";
}

// ----------------------------------------------------------------------

int Ctf01dServiceCostsAndStatistics::getAllStolenFlagsForService() {
    return m_nAllStolenFlagsForService;
}

// ----------------------------------------------------------------------

void Ctf01dServiceCostsAndStatistics::doIncrementStolenFlagsForService(int nAllStolenFlags) {
    m_nAllStolenFlagsForService++;
    m_nReverseProportionalStolenFlags = double(nAllStolenFlags) / double(m_nAllStolenFlagsForService);
}

// ----------------------------------------------------------------------

double Ctf01dServiceCostsAndStatistics::updateProportionalStolenFlagsForService(int nStolenFlags, int nAllStolenFlags) {
    m_nAllStolenFlagsForService = nStolenFlags;
    return updateProportionalStolenFlagsForService(nAllStolenFlags);
}

// ----------------------------------------------------------------------


double Ctf01dServiceCostsAndStatistics::updateProportionalStolenFlagsForService(int nAllStolenFlags) {
    if (m_nAllStolenFlagsForService > 0) {
        m_nReverseProportionalStolenFlags = double(nAllStolenFlags) / double(m_nAllStolenFlagsForService);
    } else {
        m_nReverseProportionalStolenFlags = double(nAllStolenFlags) / 1.0;
    }
    return m_nReverseProportionalStolenFlags;
}

// ----------------------------------------------------------------------

double Ctf01dServiceCostsAndStatistics::updateCostStolenFlag(double nSumOfReverseProportionalStolenFlags) {
    if (nSumOfReverseProportionalStolenFlags == 0.0) {
        m_nCostStolenFlag = m_nBasicCostOfStolenFlag;
        return m_nCostStolenFlag;
    }
    double k = m_nReverseProportionalStolenFlags / nSumOfReverseProportionalStolenFlags;
    m_nCostStolenFlag = double(m_nBasicCostOfStolenFlag) * k;
    return m_nCostStolenFlag;
}

// ----------------------------------------------------------------------

double Ctf01dServiceCostsAndStatistics::getCostStolenFlag() {
    return m_nCostStolenFlag;
}

// ----------------------------------------------------------------------

int Ctf01dServiceCostsAndStatistics::getAllDefenceFlagsForService() {
    return m_nAllDefenceFlagsForService;
}

// ----------------------------------------------------------------------

void Ctf01dServiceCostsAndStatistics::doIncrementDefenceFlagsForService(int nAllDefenceFlags) {
    m_nAllDefenceFlagsForService++;
    m_nReverseProportionalDefenceFlags = double(nAllDefenceFlags) / double(m_nAllDefenceFlagsForService);
}

// ----------------------------------------------------------------------

double Ctf01dServiceCostsAndStatistics::updateProportionalDefenceFlagsForService(int nDefenceFlags, int nAllDefenceFlags) {
    m_nAllDefenceFlagsForService = nDefenceFlags;
    return updateProportionalDefenceFlagsForService(nAllDefenceFlags);
}

// ----------------------------------------------------------------------

double Ctf01dServiceCostsAndStatistics::updateProportionalDefenceFlagsForService(int nAllDefenceFlags) {
    if (m_nAllDefenceFlagsForService > 0) {
        m_nReverseProportionalDefenceFlags = double(nAllDefenceFlags) / double(m_nAllDefenceFlagsForService);
    } else {
        m_nReverseProportionalDefenceFlags = double(nAllDefenceFlags) / 1.0;
    }
    return m_nReverseProportionalDefenceFlags;
}

// ----------------------------------------------------------------------

double Ctf01dServiceCostsAndStatistics::updateCostDefenceFlag(double nSumOfReverseProportionalDefenceFlags) {
    if (nSumOfReverseProportionalDefenceFlags == 0.0) {
        m_nCostDefenceFlag = m_nBasicCostOfDefenceFlag;
        return m_nCostDefenceFlag;
    }
    // 2 / 4  = 0.5
    double k = m_nReverseProportionalDefenceFlags / nSumOfReverseProportionalDefenceFlags;
    m_nCostDefenceFlag = double(m_nBasicCostOfDefenceFlag) * k;
    return m_nCostDefenceFlag;
}

// ----------------------------------------------------------------------

double Ctf01dServiceCostsAndStatistics::getCostDefenceFlag() {
    return 1.0;
    // return m_nCostDefenceFlag;
}

// ----------------------------------------------------------------------

std::string Ctf01dServiceCostsAndStatistics::getFirstBloodTeamId() {
    return m_sFirstBloodTeamId;
}

// ----------------------------------------------------------------------

void Ctf01dServiceCostsAndStatistics::updateJsonCosts(nlohmann::json &jsonCosts) {
    jsonCosts["cost_att"] = m_nCostStolenFlag;
    jsonCosts["cost_def"] = m_nCostDefenceFlag;
    jsonCosts["af_att"] = m_nAllStolenFlagsForService;
    jsonCosts["af_def"] = m_nAllDefenceFlagsForService;
    jsonCosts["first_blood"] = m_sFirstBloodTeamId;
}

// ----------------------------------------------------------------------

void Ctf01dServiceCostsAndStatistics::setFirstBloodTeamId(const std::string &sFirstBlood) {
    m_sFirstBloodTeamId = sFirstBlood;
}

// ---------------------------------------------------------------------
// ServiceStatusCell


std::string ServiceStatusCell::SERVICE_UP = "up";
std::string ServiceStatusCell::SERVICE_DOWN = "down";
std::string ServiceStatusCell::SERVICE_MUMBLE = "mumble";
std::string ServiceStatusCell::SERVICE_CORRUPT = "corrupt";
std::string ServiceStatusCell::SERVICE_SHIT = "shit";
std::string ServiceStatusCell::SERVICE_WAIT = "wait";
std::string ServiceStatusCell::SERVICE_COFFEEBREAK = "coffeebreak";


// ----------------------------------------------------------------------

ServiceStatusCell::ServiceStatusCell(const std::string &sServiceId) {
    m_nUpPointTimeInSec = WsjcppCore::getCurrentTimeInSeconds();
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

void ServiceStatusCell::setUpPointTime(int nUpPointTimeInSec) {
    m_nUpPointTimeInSec = nUpPointTimeInSec;
}

// ----------------------------------------------------------------------

void ServiceStatusCell::setStatus(const std::string &sStatus) {
    std::lock_guard<std::mutex> lock(m_mutexServiceStatus);
    m_sStatus = sStatus;
    if (sStatus != ServiceStatusCell::SERVICE_UP) {
        m_nUpPointTimeInSec = WsjcppCore::getCurrentTimeInSeconds();
    }
}

// ----------------------------------------------------------------------

std::string ServiceStatusCell::status() {
    return m_sStatus;
}

// ----------------------------------------------------------------------
// TeamStatusRow

TeamStatusRow::TeamStatusRow(
    const std::string &sTeamId,
    int nGameStartInSec,
    int nGameEndInSec
) {
    EmployConfig *pConfig = findWsjcppEmploy<EmployConfig>();
    const std::vector<Ctf01dServiceDef> &vServicesConf = pConfig->servicesConf();

    m_sTeamId = sTeamId;
    m_nPlace = 0;
    m_nPoints = 0;

    for (unsigned int i = 0; i < vServicesConf.size(); i++) {
        Ctf01dServiceDef serviceConf = vServicesConf[i];
        std::string sServiceId = serviceConf.id();
        m_mapServicesStatus[sServiceId] = new ServiceStatusCell(serviceConf.id());
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

// ---------------------------------------------------------------------
// EmployScoreboard

REGISTRY_WJSCPP_SERVICE_LOCATOR(EmployScoreboard)

EmployScoreboard::EmployScoreboard() 
: WsjcppEmployBase(EmployScoreboard::name(), {}) {
    TAG = EmployScoreboard::name();
    m_nAllStolenFlags = 0;
    m_nAllDefenceFlags = 0;
}

// ---------------------------------------------------------------------

bool EmployScoreboard::init() {
    EmployConfig *pEmployConfig = findWsjcppEmploy<EmployConfig>();
    const std::vector<Ctf01dTeamDef> &vTeamsConf = pEmployConfig->teamsConf();
    const std::vector<Ctf01dServiceDef> &vServicesConf = pEmployConfig->servicesConf();
    
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

    return true;
}

// ---------------------------------------------------------------------

bool EmployScoreboard::deinit() {
    WsjcppLog::info(TAG, "deinit");
    return true;
}


