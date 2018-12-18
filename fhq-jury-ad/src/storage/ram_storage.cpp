#include <ram_storage.h>
#include <cstdlib>
#include <ctime>
#include <utils_logger.h>
#include <utils_parse_config.h>

RamStorage::RamStorage(ModelScoreboard *pScoreboard, int nGameStartUTCInSec, int nGameEndUTCInSec) {
    m_pScoreboard = pScoreboard;
    TAG = "RamStorage";
    m_nGameStartUTCInSec = nGameStartUTCInSec;
    m_nGameEndUTCInSec = nGameEndUTCInSec;

    // init variables
    std::srand(unsigned(std::time(0)));
}

// ----------------------------------------------------------------------

std::string RamStorage::type() {
    return "ram";
}

// ----------------------------------------------------------------------

bool RamStorage::applyConfigFromFile(const std::string &sConfigFile, 
            std::vector<ModelTeamConf> &vTeamsConf,
            std::vector<ModelServiceConf> &vServicesConf) {
    
    Log::info(TAG, "Reading config: " + sConfigFile);
    
    if (!Log::fileExists(sConfigFile)) {
        Log::err(TAG, "File " + sConfigFile + " does not exists ");
        return false;
    }

    // game.conf - will be override configs from conf.ini
    UtilsParseConfig mysqlStorageConf = UtilsParseConfig(sConfigFile);
    if (!mysqlStorageConf.parseConfig()) {
        Log::err(TAG, "Could not parse " + sConfigFile);
        return false;
    }

    m_sDatabasePath = mysqlStorageConf.getStringValueFromConfig("ram_storage.dbpath", m_sDatabasePath);
    Log::info(TAG, "ram_storage.dbpath: " + m_sDatabasePath);

    return true;
}

// ----------------------------------------------------------------------

void RamStorage::clean() {
    
}

// ----------------------------------------------------------------------

void RamStorage::addLiveFlag(const ModelTeamConf &teamConf, const ModelServiceConf &serviceConf, const ModelFlag &flag){
    std::lock_guard<std::mutex> lock(m_mutexFlags);

    ModelFlag *pModelFlag = new ModelFlag();
    pModelFlag->copyFrom(flag);
    m_vFlagLives.push_back(pModelFlag);
    // cache by flag value
    m_mapFlagLive[flag.value()] = pModelFlag;
}

// ----------------------------------------------------------------------

std::vector<ModelFlag> RamStorage::endedFlags(const ModelTeamConf &teamConf, const ModelServiceConf &serviceConf){
    // TODO
    return std::vector<ModelFlag>();
}

// ----------------------------------------------------------------------

void RamStorage::updateFlag(const ModelTeamConf &teamConf, const ModelServiceConf &serviceConf, const ModelFlag &sFlag){
    // TODO
}

// ----------------------------------------------------------------------

void RamStorage::updateScoreboard(const ModelTeamConf &teamConf, const ModelServiceConf &serviceConf) {
    // TODO
}

// ----------------------------------------------------------------------

bool RamStorage::findFlagByValue(const std::string &sFlag, ModelFlag &resultFlag) {
    std::map<std::string, ModelFlag *>::iterator it;
    it = m_mapFlagLive.find(sFlag);
    if (it != m_mapFlagLive.end()) {
        resultFlag.copyFrom(*(it->second));
        return true;    
    }
    return false;
}

// ----------------------------------------------------------------------

bool RamStorage::updateTeamStole(const std::string &sFlag, const std::string &sTeamId) {
     // TODO
    return false;
}

// ----------------------------------------------------------------------

void RamStorage::removeFlag(ModelFlag &flag) {
    // TODO
}

// ----------------------------------------------------------------------

void RamStorage::moveToArchive(ModelFlag &flag) {
    std::lock_guard<std::mutex> lock(m_mutexFlags);

    // TODO
}