#include <ram_storage.h>
#include <cstdlib>
#include <ctime>
#include <utils_logger.h>
#include <ini.h>

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

void RamStorage::setDatabasePath(const std::string &sDatabasePath) {
    m_sDatabasePath = sDatabasePath;
}

// ----------------------------------------------------------------------

static int handler_parse_ram_storage_config(void* p, const char* section, const char* name, const char* value) {
    RamStorage* pStorage = (RamStorage*)p;
    std::string sSection(section);
    std::string sName(name);
    std::string sValue(value);

    if (sSection == "ram_storage") {
        if (sName == "dbpath") {
            pStorage->setDatabasePath(sValue);
        } else {
            Log::warn("RamStorage", "Unknown props ram_storage/" + sName);
        }
    }
    return 1;
}

// ----------------------------------------------------------------------

bool RamStorage::applyConfigFromFile(const std::string &sFilePath, 
            std::vector<ModelTeamConf> &vTeamsConf,
            std::vector<ModelServiceConf> &vServicesConf) {
    if (ini_parse(sFilePath.c_str(), handler_parse_ram_storage_config, this) < 0) {
        Log::err(TAG, "Could not load config file");
        return false;
    }

    return true;
}

// ----------------------------------------------------------------------

void RamStorage::clean() {
    
}

// ----------------------------------------------------------------------

void RamStorage::addLiveFlag(const ModelTeamConf &teamConf, const ModelServiceConf &serviceConf, const ModelFlag &flag){
    ModelFlag *pModelFlag = new ModelFlag();
    pModelFlag->copyFrom(flag);
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

bool RamStorage::updateTeamStole(const std::string &sFlag, int nTeamNum) {
     // TODO
    return false;
}

// ----------------------------------------------------------------------

void RamStorage::removeFlag(ModelFlag &flag) {
    // TODO
}

// ----------------------------------------------------------------------

void RamStorage::moveToArchive(ModelFlag &flag) {
    // TODO
}