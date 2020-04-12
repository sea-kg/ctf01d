#include "file_storage.h"
#include <cstdlib>
#include <ctime>
#include <wsjcpp_core.h>

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
    
    WsjcppLog::info(TAG, "Reading config");

    m_sDatabasePath = yamlConfig["file_storage"]["dbpath"].getValue();
    WsjcppLog::info(TAG, "ram_storage.dbpath: " + m_sDatabasePath);

    return true;
}

// ----------------------------------------------------------------------

void FileStorage::clean() {
    WsjcppLog::warn(TAG, "TODO clean");
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
    WsjcppLog::warn(TAG, "TODO listOfLiveFlags");
    return vResult;
}

// ----------------------------------------------------------------------

void FileStorage::insertToArchive(Flag &flag) {
    std::lock_guard<std::mutex> lock(m_mutexFlags);
    WsjcppLog::warn(TAG, "TODO insertToArchive");
    // TODO
}

// ----------------------------------------------------------------------

void FileStorage::insertToFlagsDefence(const Flag &flag, int nPoints) {
    std::lock_guard<std::mutex> lock(m_mutexFlags);
    WsjcppLog::warn(TAG, "TODO insertToFlagsDefence");
}

// ----------------------------------------------------------------------

int FileStorage::numberOfFlagSuccessPutted(const std::string &sTeamId, const std::string &sServiceId) {
    WsjcppLog::warn(TAG, "TODO numberOfFlagSuccessPutted");
    return 0;
}

// ----------------------------------------------------------------------

int FileStorage::numberOfDefenceFlagForService(const std::string &sServiceId) {
    WsjcppLog::warn(TAG, "TODO numberOfDefenceFlagForService");
    return 0;
}

// ----------------------------------------------------------------------

int FileStorage::numberOfStolenFlagsForService(const std::string &sServiceId) {
    WsjcppLog::warn(TAG, "TODO numberOfStolenFlagsForService");
    return 0;
}

// ----------------------------------------------------------------------

std::vector<Flag> FileStorage::outdatedFlags(const std::string &sTeamId, const std::string &sServiceId){
    // TODO
    WsjcppLog::warn(TAG, "TODO outdatedFlags");
    return std::vector<Flag>();
}

// ----------------------------------------------------------------------

void FileStorage::updateFlag(const std::string &sTeamId, const std::string &sServiceId, const Flag &sFlag){
    // TODO
    WsjcppLog::warn(TAG, "TODO updateFlag");
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

<<<<<<< HEAD:src/storages/file/file_storage.cpp
=======
bool FileStorage::updateTeamStole(const std::string &sFlag, const std::string &sTeamId) {
    // TODO
    WsjcppLog::warn(TAG, "TODO updateTeamStole");
    return false;
}

// ----------------------------------------------------------------------

bool FileStorage::isAlreadyStole(const Flag &flag, const std::string &sTeamId) {
    // TODO
    WsjcppLog::warn(TAG, "TODO isAlreadyStole");
    return false;
}

bool FileStorage::isSomebodyStole(const Flag &flag) {
    // TODO
    WsjcppLog::warn(TAG, "TODO isAlreadyStole");
    return false;
}

// ----------------------------------------------------------------------

void FileStorage::insertToFlagsStolen(const Flag &flag, const std::string &sTeamId, int nPoints) {
    // TODO
}

// ----------------------------------------------------------------------

>>>>>>> Restored interface for file_Storage:fhq-jury-ad/src/storages/file/file_storage.cpp
void FileStorage::deleteFlagLive(const Flag &flag) {
    // TODO
    WsjcppLog::warn(TAG, "TODO deleteFlagLive");
}

// ----------------------------------------------------------------------

int FileStorage::getDefenceFlags(const std::string &sTeamId, const std::string &sServiceId) {
    // TODO
    return 0;
}

// ----------------------------------------------------------------------

int FileStorage::getDefencePoints(const std::string &sTeamId, const std::string &sServiceId) {
    // TODO
    return 0;
}

// ----------------------------------------------------------------------

int FileStorage::getStollenFlags(const std::string &sTeamId, const std::string &sServiceId) {
    // TODO
    return 0;
}

// ----------------------------------------------------------------------

int FileStorage::getStollenPoints(const std::string &sTeamId, const std::string &sServiceId) {
    return 0;
}

// ----------------------------------------------------------------------