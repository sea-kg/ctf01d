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

#include "file_storage.h"
#include <cstdlib>
#include <ctime>
#include <utils_logger.h>
#include <conf_file_parser.h>
#include <fs.h>

REGISTRY_STORAGE(FileStorage)

FileStorage::FileStorage(int nGameStartUTCInSec, int nGameEndUTCInSec) {
    TAG = "FileStorage";
    m_nGameStartUTCInSec = nGameStartUTCInSec;
    m_nGameEndUTCInSec = nGameEndUTCInSec;

    // init variables
    std::srand(unsigned(std::time(0)));
}

// ----------------------------------------------------------------------

bool FileStorage::applyConfigFromYaml(
    WsjcppYaml &yamlConfig,
    std::vector<Team> &vTeamsConf,
    std::vector<Ctf01dServiceDef> &vServicesConf
) {
    
    Log::info(TAG, "Reading config: " + sConfigFile);

    m_sDatabasePath = yamlConfig["ram_storage"]["dbpath"].getValue();
    Log::info(TAG, "ram_storage.dbpath: " + m_sDatabasePath);

    return true;
}

// ----------------------------------------------------------------------

void FileStorage::clean() {
    Log::warn(TAG, "TODO clean");
}

// ----------------------------------------------------------------------

void FileStorage::insertFlagLive(const Flag &flag){
    std::lock_guard<std::mutex> lock(m_mutexFlags);

    Flag *pFlag = new Flag();
    pFlag->copyFrom(flag);
    m_vFlagLives.push_back(pFlag);
    // cache by flag value
    m_mapFlagLive[flag.value()] = pFlag;
}

// ----------------------------------------------------------------------

std::vector<Flag> FileStorage::listOfLiveFlags() {
    std::vector<Flag> vResult;
    Log::warn(TAG, "TODO listOfLiveFlags");
    return vResult;
}

void FileStorage::insertToArchive(Flag &flag) {
    std::lock_guard<std::mutex> lock(m_mutexFlags);
    Log::warn(TAG, "TODO insertToArchive");
    // TODO
}

int FileStorage::numberOfFlagSuccessPutted(const std::string &sTeamId, const std::string &sServiceId) {
    Log::warn(TAG, "TODO numberOfFlagSuccessPutted");
    return 0;
}

std::vector<Flag> FileStorage::outdatedFlags(const std::string &sTeamId, const std::string &sServiceId){
    // TODO
    Log::warn(TAG, "TODO outdatedFlags");
    return std::vector<Flag>();
}

// ----------------------------------------------------------------------

int FileStorage::defenceValue(const std::string &sTeamId, const std::string &sServiceId) {
    Log::warn(TAG, "TODO defenceValue");
    return 0;
}

// ----------------------------------------------------------------------

int FileStorage::attackValue(const std::string &sTeamId, const std::string &sServiceId) {
    Log::warn(TAG, "TODO attackValue");
    return 0;
}

// ----------------------------------------------------------------------

bool FileStorage::findFlagByValue(const std::string &sFlag, Flag &resultFlag) {
    std::map<std::string, Flag *>::iterator it;
    it = m_mapFlagLive.find(sFlag);
    if (it != m_mapFlagLive.end()) {
        resultFlag.copyFrom(*(it->second));
        return true;    
    }
    return false;
}

// ----------------------------------------------------------------------

void FileStorage::deleteFlagLive(const Flag &flag) {
    // TODO
    Log::warn(TAG, "TODO deleteFlagLive");
}

// ----------------------------------------------------------------------
