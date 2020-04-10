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
    std::vector<Service> &vServicesConf
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

// ----------------------------------------------------------------------

void FileStorage::insertFlagPutFail(const Flag &flag, const std::string &sReason) {
    Log::warn(TAG, "TODO insertFlagPutFail");
}

// ----------------------------------------------------------------------

void FileStorage::insertFlagCheckFail(const Flag &flag, const std::string &sReason) {
    Log::warn(TAG, "TODO insertFlagCheckFail");
}

// ----------------------------------------------------------------------

void FileStorage::insertFlagAttempt(const std::string &sTeamId, const std::string &sFlag) {
    // TODO
    Log::warn(TAG, "TODO insertFlagAttempt");
}

// ----------------------------------------------------------------------

int FileStorage::numberOfFlagAttempts(const std::string &sTeamId) {
    Log::warn(TAG, "TODO numberOfFlagAttempts");
    return 0;
}

// ----------------------------------------------------------------------

void FileStorage::insertToArchive(Flag &flag) {
    std::lock_guard<std::mutex> lock(m_mutexFlags);
    Log::warn(TAG, "TODO insertToArchive");
    // TODO
}

// ----------------------------------------------------------------------

void FileStorage::insertToFlagsDefence(const Flag &flag, int nPoints) {
    std::lock_guard<std::mutex> lock(m_mutexFlags);
    Log::warn(TAG, "TODO insertToFlagsDefence");
}

// ----------------------------------------------------------------------

int FileStorage::numberOfFlagSuccessPutted(const std::string &sTeamId, const std::string &sServiceId) {
    Log::warn(TAG, "TODO numberOfFlagSuccessPutted");
    return 0;
}

// ----------------------------------------------------------------------

int FileStorage::numberOfDefenceFlagForService(const std::string &sServiceId) {
    Log::warn(TAG, "TODO numberOfDefenceFlagForService");
    return 0;
}

// ----------------------------------------------------------------------

int FileStorage::numberOfStolenFlagsForService(const std::string &sServiceId) {
    Log::warn(TAG, "TODO numberOfStolenFlagsForService");
    return 0;
}

// ----------------------------------------------------------------------

std::vector<Flag> FileStorage::outdatedFlags(const Team &teamConf, const Service &serviceConf){
    // TODO
    Log::warn(TAG, "TODO outdatedFlags");
    return std::vector<Flag>();
}

// ----------------------------------------------------------------------

void FileStorage::updateFlag(const Team &teamConf, const Service &serviceConf, const Flag &sFlag){
    // TODO
    Log::warn(TAG, "TODO updateFlag");
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

bool FileStorage::updateTeamStole(const std::string &sFlag, const std::string &sTeamId) {
     // TODO
     Log::warn(TAG, "TODO updateTeamStole");
    return false;
}

// ----------------------------------------------------------------------

void FileStorage::deleteFlagLive(const Flag &flag) {
    // TODO
    Log::warn(TAG, "TODO deleteFlagLive");
}

// ----------------------------------------------------------------------
