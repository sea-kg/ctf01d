#include "config.h"
#include <sstream>
#include <storages.h>
#include <ctime>
#include <locale>
#include <date.h>
#include <iostream>
#include <sstream>
#include <conf_file_parser.h>
#include <read_teams_conf.h>
#include <wsjcpp_core.h>
#include <wsjcpp_yaml.h>

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
    m_bHasCoffeeBreak = false;
    m_sGameCoffeeBreakStart = "";
    m_sGameCoffeeBreakEnd = "";
    m_nGameCoffeeBreakStartUTCInSec = 0;
    m_nGameCoffeeBreakEndUTCInSec = 0;
    m_nBacisCostsStolenFlagInPoints = 10;
}

// ---------------------------------------------------------------------

bool Config::applyGameConf(WsjcppYaml &yamlConfig) {

    m_sGameId = yamlConfig["game"]["id"].getValue();
    WsjcppLog::info(TAG, "game.id: " + m_sGameId);
    m_sGameName = yamlConfig["game"]["name"].getValue();
    WsjcppLog::info(TAG, "game.name: " + m_sGameName);
    
    m_sGameStart = yamlConfig["game"]["start"].getValue();
    WsjcppLog::info(TAG, "game.start: " + m_sGameStart);
    {
        std::istringstream in{m_sGameStart.c_str()};
        date::sys_seconds tp;
        in >> date::parse("%Y-%m-%d %T", tp);
        m_nGameStartUTCInSec = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
    }
    
    WsjcppLog::info(TAG, "Game start (UNIX timestamp): " + std::to_string(m_nGameStartUTCInSec));
    
    m_sGameEnd = yamlConfig["game"]["end"].getValue();
    WsjcppLog::info(TAG, "game.end: " + m_sGameEnd);
    {
        std::istringstream in{m_sGameEnd.c_str()};
        date::sys_seconds tp;
        in >> date::parse("%Y-%m-%d %T", tp);
        m_nGameEndUTCInSec = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
    }
    WsjcppLog::info(TAG, "Game end (UNIX timestamp): " + std::to_string(m_nGameEndUTCInSec));

    m_nFlagTimeliveInMin = std::atoi(yamlConfig["game"]["flag_timelive_in_min"].getValue().c_str());
    WsjcppLog::info(TAG, "game.flag_timelive_in_min: " + std::to_string(m_nFlagTimeliveInMin));

    m_nBacisCostsStolenFlagInPoints = std::atoi(yamlConfig["game"]["basic_costs_stolen_flag_in_points"].getValue().c_str());
    WsjcppLog::info(TAG, "game.basic_costs_stolen_flag_in_points: " + std::to_string(m_nBacisCostsStolenFlagInPoints));

    if (m_nGameStartUTCInSec == 0) {
        WsjcppLog::err(TAG, "game.start - not found");
        return false;
    }

    if (m_nGameEndUTCInSec == 0) {
        WsjcppLog::err(TAG, "game.end - not found");
        return false;
    }

    if (m_nGameEndUTCInSec < m_nGameStartUTCInSec) {
        WsjcppLog::err(TAG, "game.end must be gather then game.start");
        return false;
    }

    if (m_nFlagTimeliveInMin <= 0) {
        WsjcppLog::err(TAG, "game.flag_timelive_in_min could not be less than 0");
        return false;
    }

    if (m_nFlagTimeliveInMin > 25) {
        WsjcppLog::err(TAG, "game.flag_timelive_in_min could not be gather than 25");
        return false;
    }

    m_sGameCoffeeBreakStart = yamlConfig["game"]["coffee_break_start"].getValue();
    WsjcppLog::info(TAG, "game.coffee_break_start: " + m_sGameCoffeeBreakStart);
    {
        std::istringstream in{m_sGameCoffeeBreakStart.c_str()};
        date::sys_seconds tp;
        in >> date::parse("%Y-%m-%d %T", tp);
        m_nGameCoffeeBreakStartUTCInSec = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
    }
    WsjcppLog::info(TAG, "Game coffee break start (UNIX timestamp): " + std::to_string(m_nGameCoffeeBreakStartUTCInSec));

    m_sGameCoffeeBreakEnd = yamlConfig["game"]["coffee_break_end"].getValue();
    WsjcppLog::info(TAG, "game.coffee_break_end: " + m_sGameCoffeeBreakEnd);
    {
        std::istringstream in{m_sGameCoffeeBreakEnd.c_str()};
        date::sys_seconds tp;
        in >> date::parse("%Y-%m-%d %T", tp);
        m_nGameCoffeeBreakEndUTCInSec = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
    }
    WsjcppLog::info(TAG, "Game coffee break start (UNIX timestamp): " + std::to_string(m_nGameCoffeeBreakEndUTCInSec));

    if (m_nGameStartUTCInSec < m_nGameCoffeeBreakStartUTCInSec
        && m_nGameCoffeeBreakStartUTCInSec < m_nGameEndUTCInSec
        && m_nGameStartUTCInSec < m_nGameCoffeeBreakEndUTCInSec
        && m_nGameCoffeeBreakEndUTCInSec < m_nGameEndUTCInSec
    ) {
        WsjcppLog::info(TAG, "Game has coffee break ");
        m_bHasCoffeeBreak = true;
    }

    if (m_nBacisCostsStolenFlagInPoints <= 0) {
        WsjcppLog::err(TAG, "game.basic_costs_stolen_flag_in_points could not be less than 0");
        return false;
    }

    if (m_nBacisCostsStolenFlagInPoints > 500) {
        WsjcppLog::err(TAG, "game.basic_costs_stolen_flag_in_points could not be gather than 500");
        return false;
    }

    return true;
}

// ---------------------------------------------------------------------

bool Config::applyServerConf(WsjcppYaml &yamlConfig) {
    
    m_sUseStorage = yamlConfig["server"]["use_storage"].getValue();

    if (!Storages::support(m_sUseStorage)) {
        std::vector<std::string> vStoragesListType = Storages::list();
        std::string sSupportedStorgaes = "";
        for (int i = 0; i < vStoragesListType.size(); i++) {
            if (sSupportedStorgaes.length() > 0) {
                sSupportedStorgaes += ",";
            }
            sSupportedStorgaes += " '" + vStoragesListType[i] + "'";
        }
        WsjcppLog::err(TAG, "server.use_storage defined like " + m_sUseStorage + " but supported: " + sSupportedStorgaes);
        return false;
    }
    WsjcppLog::info(TAG, "server.use_storage: " + m_sUseStorage);

    return true;
}

// ---------------------------------------------------------------------

bool Config::applyScoreboardConf() {
    std::string sConfigFile = m_sWorkspaceDir + "/scoreboard.conf";
    WsjcppLog::info(TAG, "Reading config: " + sConfigFile);

    if (!WsjcppCore::fileExists(sConfigFile)) {
        WsjcppLog::err(TAG, "File " + sConfigFile + " does not exists ");
        return false;
    }

    ConfFileParser scoreboardConf = ConfFileParser(sConfigFile);
    if (!scoreboardConf.parseConfig()) {
        WsjcppLog::err(TAG, "Could not parse " + sConfigFile);
        return false;
    }
    
    m_nScoreboardPort = scoreboardConf.getIntValueFromConfig("scoreboard.port", m_nScoreboardPort);
    if (m_nScoreboardPort <= 10 || m_nScoreboardPort > 65536) {
        WsjcppLog::err(TAG, sConfigFile + ": wrong scoreboard.port (expected value od 11..65535)");
        return false;
    }
    WsjcppLog::info(TAG, "scoreboard.port: " + std::to_string(m_nScoreboardPort));

    m_bScoreboardRandom = scoreboardConf.getBoolValueFromConfig("scoreboard.random", m_bScoreboardRandom);
    WsjcppLog::info(TAG, "scoreboard.random: " + std::string(m_bScoreboardRandom == true ? "yes" : "no"));

    m_sScoreboardHtmlFolder = scoreboardConf.getStringValueFromConfig("scoreboard.htmlfolder", m_sScoreboardHtmlFolder);
    if (m_sScoreboardHtmlFolder.length() > 0) {
        if (m_sScoreboardHtmlFolder[0] != '/') {
            m_sScoreboardHtmlFolder = m_sWorkspaceDir + '/' + m_sScoreboardHtmlFolder;
        }
    } else {
        m_sScoreboardHtmlFolder = m_sWorkspaceDir + "/html";
    }
    m_sScoreboardHtmlFolder = WsjcppCore::doNormalizePath(m_sScoreboardHtmlFolder);

    WsjcppLog::info(TAG, "scoreboard.htmlfolder: " + m_sScoreboardHtmlFolder);

    if (!WsjcppCore::dirExists(m_sScoreboardHtmlFolder)) {
        WsjcppLog::err(TAG, "Directory '" + m_sScoreboardHtmlFolder + "' with scorebord does not exists");
        return false;
    }

    return true;
}

// ---------------------------------------------------------------------

bool Config::applyCheckersConf() {
    std::string sRootCheckersDir = m_sWorkspaceDir + "/checkers/";
    if (!WsjcppCore::dirExists(sRootCheckersDir)) {
        WsjcppLog::err(TAG, "Directory " + sRootCheckersDir + " not exists");
        return false;
    }
    WsjcppLog::info(TAG, "Search service.conf");

    std::vector<std::string> vListOfCheckers = WsjcppCore::listOfDirs(sRootCheckersDir);
    if (vListOfCheckers.size() == 0) {
        WsjcppLog::err(TAG, "Folders with services does not found in " + sRootCheckersDir);
        return false;
    }

    for (int i = 0; i < vListOfCheckers.size(); i++) {
        std::string sServiceId = vListOfCheckers[i];
        std::string sServiceConfPath =  sRootCheckersDir + sServiceId + "/service.conf";
        WsjcppLog::info(TAG, "Reading " + sServiceConfPath);
        if (!WsjcppCore::fileExists(sServiceConfPath)) {
            WsjcppLog::err(TAG, "File " + sServiceConfPath + " not exists");
            return false;
        }
        ConfFileParser serviceConf = ConfFileParser(sServiceConfPath);
        if (!serviceConf.parseConfig()) {
            WsjcppLog::err(TAG, "Could not parse " + sServiceConfPath);
            return false;
        }

        std::string sPrefix = "services." + sServiceId + ".";
        std::string sServiceName 
            = serviceConf.getStringValueFromConfig(sPrefix + "name", "");
        WsjcppLog::info(TAG, sPrefix + "name = " + sServiceName);

        bool bServiceEnable 
            = serviceConf.getBoolValueFromConfig(sPrefix + "enabled", true);
        WsjcppLog::info(TAG, sPrefix + "enabled = " + std::string(bServiceEnable ? "yes" : "no"));

        std::string sServiceScriptPath 
            = serviceConf.getStringValueFromConfig(sPrefix + "script_path", "");
        WsjcppLog::info(TAG, sPrefix + "script_path = " + sServiceScriptPath);
        std::string sServiceScriptDir = m_sWorkspaceDir + "/checkers/" + sServiceId + "/";
        WsjcppLog::info(TAG, "sServiceScriptDir: " + sServiceScriptDir);
        if (!WsjcppCore::fileExists(sServiceScriptDir + sServiceScriptPath)) {
            WsjcppLog::err(TAG, "File " + sServiceScriptPath + " did not exists");
            return false;
        }

        int nServiceScritpWait
            = serviceConf.getIntValueFromConfig(sPrefix + "script_wait_in_sec", 5);
        WsjcppLog::info(TAG, sPrefix + "script_wait_in_sec = " + std::to_string(nServiceScritpWait));

        if (nServiceScritpWait < 5) {
            WsjcppLog::err(TAG, "Could not parse " + sPrefix + "script_wait_in_sec - must be more than 4 sec ");
            return false;
        }

        int nServiceSleepBetweenRun
            = serviceConf.getIntValueFromConfig(sPrefix + "time_sleep_between_run_scripts_in_sec", 15);
        WsjcppLog::info(TAG, sPrefix + "time_sleep_between_run_scripts_in_sec = " + std::to_string(nServiceSleepBetweenRun));

        if (nServiceSleepBetweenRun < nServiceScritpWait*3) {
            WsjcppLog::err(TAG, "Could not parse " + sPrefix + "time_sleep_between_run_scripts_in_sec - must be more than " + std::to_string(nServiceScritpWait*3-1) + " sec ");
            return false;
        }

        if (!bServiceEnable) {
            WsjcppLog::warn(TAG, "Checker for service " + sServiceId + " - disabled ");
            continue;
        }
        
        for (unsigned int i = 0; i < m_vServicesConf.size(); i++) {
            if (m_vServicesConf[i].id() == sServiceId) {
                WsjcppLog::err(TAG, "Already registered checker for service " + sServiceId);
                return false;
            }
        }

        // default values of service config
        Service _serviceConf;
        _serviceConf.setId(sServiceId);
        _serviceConf.setName(sServiceName);
        _serviceConf.setScriptPath(sServiceScriptPath);
        _serviceConf.setScriptDir(sServiceScriptDir);
        _serviceConf.setEnabled(bServiceEnable);
        _serviceConf.setScriptWaitInSec(nServiceScritpWait);
        _serviceConf.setTimeSleepBetweenRunScriptsInSec(nServiceSleepBetweenRun);
        m_vServicesConf.push_back(_serviceConf);

        WsjcppLog::ok(TAG, "Registered checker for service " + sServiceId);
    }

    if (m_vServicesConf.size() == 0) {
        WsjcppLog::err(TAG, "No one defined checker for service in " + sRootCheckersDir);
        return false;
    }

    return true;
}

// ---------------------------------------------------------------------

bool Config::applyConfig(){
    bool bResult = true;
    WsjcppLog::info(TAG, "Loading configuration... ");

    std::string sConfigFile = m_sWorkspaceDir + "/config.yml";
    WsjcppLog::info(TAG, "Reading config: " + sConfigFile);

    if (!WsjcppCore::fileExists(sConfigFile)) {
        WsjcppLog::err(TAG, "File " + sConfigFile + " does not exists ");
        return false;
    }
    
    WsjcppYaml yamlConfig;
    if (!yamlConfig.loadFromFile(sConfigFile)) {
        WsjcppLog::err(TAG, "Could not parse " + sConfigFile);
        return false;
    }

    // apply the game config
    if (!this->applyGameConf(yamlConfig)) {
        return false;
    }

    // apply the server config
    if (!this->applyServerConf(yamlConfig)) {
        return false;
    }

    if (!this->applyCheckersConf()) {
        return false;
    }

    ReadTeamsConf teamsConf(m_sWorkspaceDir);
    if (!teamsConf.read(m_vTeamsConf)) {
        return false;
    }

    // apply the scoreboard config
    if (!this->applyScoreboardConf()) {
        return false;
    }

    // storage
    WsjcppLog::info(TAG, "Storage: " + m_sUseStorage);
    m_pStorage = Storages::create(m_sUseStorage, m_nGameStartUTCInSec, m_nGameEndUTCInSec);
    WsjcppLog::info(TAG, "Init storage: " + m_sUseStorage);

    if (m_pStorage == NULL) {
        WsjcppLog::err(TAG, "server/use_storage: '" + m_sUseStorage + "' is unknown type of storage");
        return false;
    }
    
    // scoreboard
    m_pScoreboard = new Scoreboard(m_bScoreboardRandom, 
        m_nGameStartUTCInSec,
        m_nGameEndUTCInSec, 
        m_nGameCoffeeBreakStartUTCInSec,
        m_nGameCoffeeBreakEndUTCInSec,
        m_nFlagTimeliveInMin*60,
        m_nBacisCostsStolenFlagInPoints,
        m_vTeamsConf, 
        m_vServicesConf, 
        m_pStorage);

    // configure storage
    if (!m_pStorage->applyConfigFromFile(m_sWorkspaceDir + "/" + m_sUseStorage + "_storage.conf", m_vTeamsConf, m_vServicesConf)) {
        WsjcppLog::err(TAG, "Could not init configuration storage");
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

Scoreboard *Config::scoreboard(){
    return m_pScoreboard;
}

// ---------------------------------------------------------------------

std::vector<Team> &Config::teamsConf() {
    return m_vTeamsConf;
}

// ---------------------------------------------------------------------

std::vector<Service> &Config::servicesConf() {
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

int Config::bacisCostsStolenFlagInPoints() {
    return m_nBacisCostsStolenFlagInPoints;
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

bool Config::gameHasCoffeeBreak() {
    return m_bHasCoffeeBreak;
}

// ---------------------------------------------------------------------

int Config::gameCoffeeBreakStartUTCInSec() {
    return m_nGameCoffeeBreakStartUTCInSec;
}

// ---------------------------------------------------------------------

int Config::gameCoffeeBreakEndUTCInSec() {
    return m_nGameCoffeeBreakEndUTCInSec;
}

// ---------------------------------------------------------------------
