#include "config.h"
#include <utils_logger.h>
#include <sstream>
#include <storages.h>
#include <ctime>
#include <locale>
#include <date.h>
#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <utils_search_lazy_conf.h>
#include <utils_parse_config.h>
#include <dirent.h>

Config::Config(const std::string &sWorkspaceDir) {
    TAG = "Config";
    m_nFlagTimeliveInMin = 10;
    m_nScoreboardPort = 8080;
    m_bScoreboardRandom = false;
    m_pStorage = NULL;
    m_pScoreboard = NULL;
    m_sWorkspaceDir = sWorkspaceDir;
    m_sScoreboardHtmlFolder = sWorkspaceDir + "/html";
    m_sUseStorage = ""; // default value
    m_nGameStartUTCInSec = 0;
    m_nGameEndUTCInSec = 0;
}

// ---------------------------------------------------------------------

bool Config::fileExists(const std::string &sFilename) {
    struct stat st;
    bool bExists = (stat(sFilename.c_str(), &st) == 0);
    if (bExists) {
        return (st.st_mode & S_IFDIR) == 0;
    }
	return false;
}

// ---------------------------------------------------------------------

bool Config::dirExists(const std::string &sDirname) {
    struct stat st;
    bool bExists = (stat(sDirname.c_str(), &st) == 0);
    if (bExists) {
        return (st.st_mode & S_IFDIR) != 0;
    }
	return false;
}

// ---------------------------------------------------------------------

std::vector<std::string> Config::listOfDirs(const std::string &sDirname) {
    std::vector<std::string> vDirs;
    if (!this->dirExists(sDirname)) {
        Log::err(TAG, "Directory " + sDirname + " not exists");
        return vDirs;
    }
    DIR *dir = opendir(sDirname.c_str());
    struct dirent *entry = readdir(dir);
    while (entry != NULL) {
        if (entry->d_type == DT_DIR) {
            std::string sDir(entry->d_name);
            if (sDir != "." && sDir != "..") {
                vDirs.push_back(sDir);
            }
        }
        entry = readdir(dir);
    }
    closedir(dir);
    return vDirs;
}

// ---------------------------------------------------------------------

std::vector<std::string> Config::listOfFiles(const std::string &sDirname) {
    std::vector<std::string> vFiles;
    if (!this->dirExists(sDirname)) {
        Log::err(TAG, "Directory " + sDirname + " not exists");
        return vFiles;
    }
    DIR *dir = opendir(sDirname.c_str());
    struct dirent *entry = readdir(dir);
    while (entry != NULL) {
        if (entry->d_type != DT_DIR) {
            std::string sDir(entry->d_name);
            if (sDir != "." && sDir != "..") {
                vFiles.push_back(sDir);
            }
        }
        entry = readdir(dir);
    }
    closedir(dir);
    return vFiles;
}

// ---------------------------------------------------------------------

bool Config::applyGameConf(bool bLazyStart) {
    std::string sConfigFile = m_sWorkspaceDir + "/game.conf";
    Log::info(TAG, "Reading config: " + sConfigFile);

    if (!this->fileExists(sConfigFile)) {
        Log::err(TAG, "File " + sConfigFile + " does not exists ");
        return false;
    }

    // game.conf - will be override configs from conf.ini
    UtilsParseConfig gameConf = UtilsParseConfig(sConfigFile);
    if (!gameConf.parseConfig()) {
        Log::err(TAG, "Could not parse " + sConfigFile);
        return false;
    }

    m_sGameId = gameConf.getStringValueFromConfig("game.id", m_sGameId);
    Log::info(TAG, "game.id: " + m_sGameId);
    m_sGameName = gameConf.getStringValueFromConfig("game.name", m_sGameName);
    Log::info(TAG, "game.name: " + m_sGameName);
    if (bLazyStart) {
        m_nGameStartUTCInSec = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    } else {
        m_sGameStart = gameConf.getStringValueFromConfig("game.start", m_sGameStart);
        Log::info(TAG, "game.start: " + m_sGameStart);
        {
            std::istringstream in{m_sGameStart.c_str()};
            date::sys_seconds tp;
            in >> date::parse("%Y-%m-%d %T", tp);
            m_nGameStartUTCInSec = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
        }
    }
    Log::info(TAG, "Game start (UNIX timestamp): " + std::to_string(m_nGameStartUTCInSec));
    
    if (bLazyStart) {
        m_nGameEndUTCInSec = m_nGameStartUTCInSec + 4*60*60; // 4 hours
    } else {
        m_sGameEnd = gameConf.getStringValueFromConfig("game.end", m_sGameEnd);
        Log::info(TAG, "game.end: " + m_sGameEnd);
        {
            std::istringstream in{m_sGameEnd.c_str()};
            date::sys_seconds tp;
            in >> date::parse("%Y-%m-%d %T", tp);
            m_nGameEndUTCInSec = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
        }
    }
    Log::info(TAG, "Game end (UNIX timestamp): " + std::to_string(m_nGameEndUTCInSec));

    m_nFlagTimeliveInMin = gameConf.getIntValueFromConfig("game.flag_timelive_in_min", m_nFlagTimeliveInMin);
    Log::info(TAG, "game.flag_timelive_in_min: " + std::to_string(m_nFlagTimeliveInMin));

    if (m_nGameStartUTCInSec == 0) {
        Log::err(TAG, sConfigFile + ":game.start - not found");
        return false;
    }

    if (m_nGameEndUTCInSec == 0) {
        Log::err(TAG, sConfigFile + ": game.end - not found");
        return false;
    }

    if (m_nGameEndUTCInSec < m_nGameStartUTCInSec) {
        Log::err(TAG, sConfigFile + ": game.end must be gather then game.start");
        return false;
    }

    if (m_nFlagTimeliveInMin <= 0) {
         Log::err(TAG, sConfigFile + ": game.flag_timelive_in_min could not be less than 0");
        return false;
    }

    if (m_nFlagTimeliveInMin > 25) {
         Log::err(TAG, sConfigFile + ": game.flag_timelive_in_min could not be gather than 25");
        return false;
    }

    return true;
}

// ---------------------------------------------------------------------

bool Config::applyServerConf(bool bLazyStart) {
    std::string sConfigFile = m_sWorkspaceDir + "/server.conf";
    Log::info(TAG, "Reading config: " + sConfigFile);

    if (!this->fileExists(sConfigFile)) {
        Log::err(TAG, "File " + sConfigFile + " does not exists ");
        return false;
    }

    UtilsParseConfig serverConf = UtilsParseConfig(sConfigFile);
    if (!serverConf.parseConfig()) {
        Log::err(TAG, "Could not parse " + sConfigFile);
        return false;
    }
    
    m_sUseStorage = serverConf.getStringValueFromConfig("server.use_storage", m_sUseStorage);

    if (!Storages::support(m_sUseStorage)) {
        std::vector<std::string> vStoragesListType = Storages::list();
        std::string sSupportedStorgaes = "";
        for (int i = 0; i < vStoragesListType.size(); i++) {
            if (sSupportedStorgaes.length() > 0) {
                sSupportedStorgaes += ",";
            }
            sSupportedStorgaes += " '" + vStoragesListType[i] + "'";
        }
        Log::err(TAG, sConfigFile + ": server.use_storage defined like " + m_sUseStorage + " but supported: " + sSupportedStorgaes);
        return false;
    }
    Log::info(TAG, "server.use_storage: " + m_sUseStorage);

    return true;
}

// ---------------------------------------------------------------------

bool Config::applyScoreboardConf(bool bLazyStart) {
    std::string sConfigFile = m_sWorkspaceDir + "/scoreboard.conf";
    Log::info(TAG, "Reading config: " + sConfigFile);

    if (!this->fileExists(sConfigFile)) {
        Log::err(TAG, "File " + sConfigFile + " does not exists ");
        return false;
    }

    UtilsParseConfig scoreboardConf = UtilsParseConfig(sConfigFile);
    if (!scoreboardConf.parseConfig()) {
        Log::err(TAG, "Could not parse " + sConfigFile);
        return false;
    }
    
    m_nScoreboardPort = scoreboardConf.getIntValueFromConfig("scoreboard.port", m_nScoreboardPort);
    if (m_nScoreboardPort <= 10 || m_nScoreboardPort > 65536) {
        Log::err(TAG, sConfigFile + ": wrong scoreboard.port (expected value od 11..65535)");
        return false;
    }
    Log::info(TAG, "scoreboard.port: " + std::to_string(m_nScoreboardPort));

    m_bScoreboardRandom = scoreboardConf.getBoolValueFromConfig("scoreboard.random", m_bScoreboardRandom);
    Log::info(TAG, "scoreboard.random: " + std::string(m_bScoreboardRandom == true ? "yes" : "no"));

    m_sScoreboardHtmlFolder = scoreboardConf.getStringValueFromConfig("scoreboard.htmlfolder", m_sScoreboardHtmlFolder);
    if (m_sScoreboardHtmlFolder.length() > 0) {
        if (m_sScoreboardHtmlFolder[0] != '/') {
            m_sScoreboardHtmlFolder = m_sWorkspaceDir + '/' + m_sScoreboardHtmlFolder;
        } else {
            m_sScoreboardHtmlFolder = m_sScoreboardHtmlFolder;
        }
    } else {
        m_sScoreboardHtmlFolder = m_sWorkspaceDir + "/html";
    }
    Log::info(TAG, "scoreboard.htmlfolder: " + m_sScoreboardHtmlFolder);

    if (!this->dirExists(m_sScoreboardHtmlFolder)) {
        Log::err(TAG, "Directory '" + m_sScoreboardHtmlFolder + "' with scorebord does not exists");
        return false;
    }

    return true;
}

// ---------------------------------------------------------------------

bool Config::applyTeamsConf(bool bLazyStart) {
    std::string sRootTeamsDir = m_sWorkspaceDir + "/teams/";
    if (!this->dirExists(sRootTeamsDir)) {
        Log::err(TAG, "Directory " + sRootTeamsDir + " not exists");
        return false;
    }
    Log::info(TAG, "Search team.conf");

    std::vector<std::string> vListOfTeams = this->listOfFiles(sRootTeamsDir);

    if (vListOfTeams.size() == 0) {
        Log::err(TAG, "Teams does not defined");
        return false;
    }

    for (int i = 0; i < vListOfTeams.size(); i++) {
        std::string sFilename = vListOfTeams[i];
        std::stringstream test(sFilename);
        std::string sTeamId = "";
        if (std::getline(test, sTeamId, '.')) {
        }
        std::string sTeamConfPath =  sRootTeamsDir + sTeamId + ".conf";
        Log::info(TAG, "Reading " + sTeamConfPath);
        if (!this->fileExists(sTeamConfPath)) {
            Log::err(TAG, "File " + sTeamConfPath + " not exists");
            return false;
        }
        UtilsParseConfig teamConf = UtilsParseConfig(sTeamConfPath);
        if (!teamConf.parseConfig()) {
            Log::err(TAG, "Could not parse " + sTeamConfPath);
            return false;
        }
        std::string sPrefix = "teams." + sTeamId + ".";

        Log::info(TAG, sPrefix + "id = " + sTeamId);

        std::string sTeamName 
            = teamConf.getStringValueFromConfig(sPrefix + "name", "");
        Log::info(TAG, sPrefix + "name = " + sTeamName);

         bool bTeamActive
            = teamConf.getBoolValueFromConfig(sPrefix + "active", true);
        Log::info(TAG, sPrefix + "active = " + std::string(bTeamActive ? "yes" : "no"));

        std::string sTeamIpAddress 
            = teamConf.getStringValueFromConfig(sPrefix + "ip_address", "");
        Log::info(TAG, sPrefix + "ip_address = " + sTeamIpAddress);
        // TODO check the ip format

        std::string sTeamLogo
            = teamConf.getStringValueFromConfig(sPrefix + "logo", "");
        Log::info(TAG, sPrefix + "logo = " + sTeamLogo);
        // TODO check logo exists
        
        if (!bTeamActive) {
            Log::warn(TAG, "Team " + sTeamId + " - disactivated ");
            continue;
        }

        for (unsigned int i = 0; i < m_vTeamsConf.size(); i++) {
            if (m_vTeamsConf[i].id() == sTeamId) {
                Log::err(TAG, "Already registered team with id " + sTeamId);
                return false;
            }
        }
        // default values of service config
        ModelTeamConf _teamConf;
        _teamConf.setId(sTeamId);
        _teamConf.setActive(true);
        _teamConf.setIpAddress(sTeamIpAddress);
        _teamConf.setLogo(sTeamLogo);

        m_vTeamsConf.push_back(_teamConf);
        Log::ok(TAG, "Registered team " + sTeamId);
    }
    
    if (m_vTeamsConf.size() == 0) {
        Log::err(TAG, "No one defined team " + sRootTeamsDir);
        return false;
    }

    return true;
}

// ---------------------------------------------------------------------

bool Config::applyCheckersConf(bool bLazyStart) {
    std::string sRootCheckersDir = m_sWorkspaceDir + "/checkers/";
    if (!this->dirExists(sRootCheckersDir)) {
        Log::err(TAG, "Directory " + sRootCheckersDir + " not exists");
        return false;
    }
    Log::info(TAG, "Search service.conf");

    std::vector<std::string> vListOfCheckers = this->listOfDirs(sRootCheckersDir);
    if (vListOfCheckers.size() == 0) {
        Log::err(TAG, "Folders with services does not found in " + sRootCheckersDir);
        return false;
    }

    for (int i = 0; i < vListOfCheckers.size(); i++) {
        std::string sServiceId = vListOfCheckers[i];
        std::string sServiceConfPath =  sRootCheckersDir + sServiceId + "/service.conf";
        Log::info(TAG, "Reading " + sServiceConfPath);
        if (!this->fileExists(sServiceConfPath)) {
            Log::err(TAG, "File " + sServiceConfPath + " not exists");
            return false;
        }
        UtilsParseConfig serviceConf = UtilsParseConfig(sServiceConfPath);
        if (!serviceConf.parseConfig()) {
            Log::err(TAG, "Could not parse " + sServiceConfPath);
            return false;
        }

        std::string sPrefix = "services." + sServiceId + ".";
        std::string sServiceName 
            = serviceConf.getStringValueFromConfig(sPrefix + "name", "");
        Log::info(TAG, sPrefix + "name = " + sServiceName);

        bool bServiceEnable 
            = serviceConf.getBoolValueFromConfig(sPrefix + "enabled", true);
        Log::info(TAG, sPrefix + "enabled = " + std::string(bServiceEnable ? "yes" : "no"));

        std::string sServiceScriptPath 
            = serviceConf.getStringValueFromConfig(sPrefix + "script_path", "");
        Log::info(TAG, sPrefix + "script_path = " + sServiceScriptPath);
        std::string sServiceScriptDir = m_sWorkspaceDir + "/checkers/" + sServiceId + "/";
        Log::info(TAG, "sServiceScriptDir: " + sServiceScriptDir);
        if (!this->fileExists(sServiceScriptDir + sServiceScriptPath)) {
            Log::err(TAG, "File " + sServiceScriptPath + " did not exists");
            return false;
        }

        int nServiceScritpWait
            = serviceConf.getIntValueFromConfig(sPrefix + "script_wait_in_sec", 5);
        Log::info(TAG, sPrefix + "script_wait_in_sec = " + std::to_string(nServiceScritpWait));

        if (nServiceScritpWait < 5) {
            Log::err(TAG, "Could not parse " + sPrefix + "script_wait_in_sec - must be more than 4 sec ");
            return false;
        }

        int nServiceSleepBetweenRun
            = serviceConf.getIntValueFromConfig(sPrefix + "time_sleep_between_run_scripts_in_sec", 15);
        Log::info(TAG, sPrefix + "time_sleep_between_run_scripts_in_sec = " + std::to_string(nServiceSleepBetweenRun));

        if (nServiceSleepBetweenRun < nServiceScritpWait*3) {
            Log::err(TAG, "Could not parse " + sPrefix + "time_sleep_between_run_scripts_in_sec - must be more than " + std::to_string(nServiceScritpWait*3-1) + " sec ");
            return false;
        }

        if (!bServiceEnable) {
            Log::warn(TAG, "Checker for service " + sServiceId + " - disabled ");
            continue;
        }
        
        for (unsigned int i = 0; i < m_vServicesConf.size(); i++) {
            if (m_vServicesConf[i].id() == sServiceId) {
                Log::err(TAG, "Already registered checker for service " + sServiceId);
                return false;
            }
        }

        // default values of service config
        ModelServiceConf _serviceConf;
        _serviceConf.setId(sServiceId);
        _serviceConf.setName(sServiceName);
        _serviceConf.setScriptPath(sServiceScriptPath);
        _serviceConf.setScriptDir(sServiceScriptDir);
        _serviceConf.setEnabled(bServiceEnable);
        _serviceConf.setScriptWaitInSec(nServiceScritpWait);
        _serviceConf.setTimeSleepBetweenRunScriptsInSec(nServiceSleepBetweenRun);
        m_vServicesConf.push_back(_serviceConf);

        Log::ok(TAG, "Registered checker for service " + sServiceId);
    }

    if (m_vServicesConf.size() == 0) {
        Log::err(TAG, "No one defined checker for service in " + sRootCheckersDir);
        return false;
    }

    return true;
}

// ---------------------------------------------------------------------

bool Config::applyConfig(bool bLazyStart){
    bool bResult = true;
    Log::info(TAG, "Loading configuration... ");

    // apply the game config
    if (!this->applyGameConf(bLazyStart)) {
        return false;
    }

    // apply the server config
    if (!this->applyServerConf(bLazyStart)) {
        return false;
    }

    if (!this->applyCheckersConf(bLazyStart)) {
        return false;
    }

    // teams
    if (bLazyStart) {

        // teams by scanning
        SearchLazyConf searchLazyConf(this->scoreboardPort());
        searchLazyConf.scan();
        m_vTeamsConf.clear();
        m_vTeamsConf = searchLazyConf.getFoundTeams();

    } else {
        if (!this->applyTeamsConf(bLazyStart)) {
            return false;
        }
    }

    // apply the scoreboard config
    if (!this->applyScoreboardConf(bLazyStart)) {
        return false;
    }

    // scoreboard
    m_pScoreboard = new ModelScoreboard(m_bScoreboardRandom, m_nGameStartUTCInSec, m_nGameEndUTCInSec, m_vTeamsConf, m_vServicesConf);

    // storage
    Log::info(TAG, "Storage: " + m_sUseStorage);
    m_pStorage = Storages::create(m_sUseStorage, m_pScoreboard, m_nGameStartUTCInSec, m_nGameEndUTCInSec);
    Log::info(TAG, "Init storage: " + m_sUseStorage);
    
    if (m_pStorage == NULL) {
        Log::err(TAG, "server/use_storage: '" + m_sUseStorage + "' is unknown type of storage");
		return false;
    }

    // configure storage
    if (!m_pStorage->applyConfigFromFile(m_sWorkspaceDir + "/" + m_sUseStorage + "_storage.conf", m_vTeamsConf, m_vServicesConf)) {
        Log::err(TAG, "Could not init configuration storage");
        return false;
    }

    return bResult;
}

// ---------------------------------------------------------------------

Storage *Config::storage(){
    return m_pStorage;
}

// ---------------------------------------------------------------------

void Config::setStorage(Storage *pStorage){
    m_pStorage = pStorage;
}

// ---------------------------------------------------------------------

ModelScoreboard *Config::scoreboard(){
    return m_pScoreboard;
}

// ---------------------------------------------------------------------

std::vector<ModelTeamConf> &Config::teamsConf() {
    return m_vTeamsConf;
}

// ---------------------------------------------------------------------

std::vector<ModelServiceConf> &Config::servicesConf() {
    return m_vServicesConf;
}

// ---------------------------------------------------------------------

int Config::scoreboardPort(){
    return m_nScoreboardPort;
}

// ---------------------------------------------------------------------

std::string Config::scoreboardHtmlFolder() {
    return m_sScoreboardHtmlFolder;
}

// ---------------------------------------------------------------------

bool Config::scoreboardRandom() {
    return m_bScoreboardRandom;
}

// ---------------------------------------------------------------------

int Config::flagTimeliveInMin(){
    return m_nFlagTimeliveInMin;
}

// ---------------------------------------------------------------------

std::string Config::gameId(){
    return m_sGameId;
}

// ---------------------------------------------------------------------

std::string Config::gameName(){
    return m_sGameName;
}

// ---------------------------------------------------------------------

int Config::gameStartUTCInSec() {
    return m_nGameStartUTCInSec;
}

// ---------------------------------------------------------------------

int Config::gameEndUTCInSec() {
    return m_nGameEndUTCInSec;
}

// ---------------------------------------------------------------------
