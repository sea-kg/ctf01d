#include "file_storage.h"
#include <cstdlib>
#include <ctime>
#include <utils_logger.h>
#include <conf_file_parser.h>

REGISTRY_STORAGE(FileStorage)

FileStorage::FileStorage(Scoreboard *pScoreboard, int nGameStartUTCInSec, int nGameEndUTCInSec) {
    m_pScoreboard = pScoreboard;
    TAG = "FileStorage";
    m_nGameStartUTCInSec = nGameStartUTCInSec;
    m_nGameEndUTCInSec = nGameEndUTCInSec;

    // init variables
    std::srand(unsigned(std::time(0)));
}

// ----------------------------------------------------------------------

bool FileStorage::applyConfigFromFile(const std::string &sConfigFile, 
            std::vector<Team> &vTeamsConf,
            std::vector<Service> &vServicesConf) {
    
    Log::info(TAG, "Reading config: " + sConfigFile);
    
    if (!Log::fileExists(sConfigFile)) {
        Log::err(TAG, "File " + sConfigFile + " does not exists ");
        return false;
    }

    // game.conf - will be override configs from conf.ini
    ConfFileParser mysqlStorageConf = ConfFileParser(sConfigFile);
    if (!mysqlStorageConf.parseConfig()) {
        Log::err(TAG, "Could not parse " + sConfigFile);
        return false;
    }

    m_sDatabasePath = mysqlStorageConf.getStringValueFromConfig("ram_storage.dbpath", m_sDatabasePath);
    Log::info(TAG, "ram_storage.dbpath: " + m_sDatabasePath);

    return true;
}

// ----------------------------------------------------------------------

void FileStorage::clean() {
    
}

// ----------------------------------------------------------------------

void FileStorage::addLiveFlag(const Team &teamConf, const Service &serviceConf, const Flag &flag){
    std::lock_guard<std::mutex> lock(m_mutexFlags);

    Flag *pFlag = new Flag();
    pFlag->copyFrom(flag);
    m_vFlagLives.push_back(pFlag);
    // cache by flag value
    m_mapFlagLive[flag.value()] = pFlag;
}

// ----------------------------------------------------------------------

std::vector<Flag> FileStorage::endedFlags(const Team &teamConf, const Service &serviceConf){
    // TODO
    return std::vector<Flag>();
}

// ----------------------------------------------------------------------

void FileStorage::updateFlag(const Team &teamConf, const Service &serviceConf, const Flag &sFlag){
    // TODO
}

// ----------------------------------------------------------------------

void FileStorage::updateScoreboard(const Team &teamConf, const Service &serviceConf) {
    // TODO
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
    return false;
}

// ----------------------------------------------------------------------

void FileStorage::removeFlag(Flag &flag) {
    // TODO
}

// ----------------------------------------------------------------------

void FileStorage::moveToArchive(Flag &flag) {
    std::lock_guard<std::mutex> lock(m_mutexFlags);

    // TODO
}