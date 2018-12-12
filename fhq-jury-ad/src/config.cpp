#include "config.h"
#include <utils_logger.h>
#include <sstream>
#include <mysql_storage.h>
#include <ram_storage.h>
#include <ctime>
#include <locale>
#include <ini.h>
#include <date.h>
#include <iostream>
#include <sstream>
#include <utils_search_lazy_conf.h>
#include <utils_parse_config.h>

JuryConfiguration::JuryConfiguration(const std::string &sWorkspaceDir) {
    TAG = "JuryConfiguration";
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

static int handler_parse_jury_config(void* p, const char* section, const char* name, const char* value) {
    JuryConfiguration* pJuryConfiguration = (JuryConfiguration*)p;
    std::string sSection(section);
    std::string sName(name);
    std::string sValue(value);

    if (sSection == "game") {
        if (sName == "flag_timelive_in_min") {
            int nValue = std::stoi(sValue); // TODO try catch
            pJuryConfiguration->setFlagTimeliveInMin(nValue);
        } else if (sName == "name") {
            pJuryConfiguration->setGameName(sValue);
        } else if (sName == "start") {
            pJuryConfiguration->setGameStart(sValue);
        } else if (sName == "end") {
            pJuryConfiguration->setGameEnd(sValue);
        } else {
            Log::warn("JuryConfiguration", "Unknown property " + sSection + "/" + sName);
        }
    } else if (sSection == "server") {
        if (sName == "scoreboard_port") {
            int nValue = std::stoi(sValue); // TODO try catch
            pJuryConfiguration->setScoreboardPort(nValue);
        } else if (sName == "scoreboard_html_folder") {
            pJuryConfiguration->setScoreboardHtmlFolder(sValue);
        } else if (sName == "use_storage") {
            pJuryConfiguration->setUseStorage(sValue);
        } else if (sName == "scoreboard_random") {
            if (sValue == "yes") {
                pJuryConfiguration->setScoreboardRandom(true);
            } else if (sValue == "no") {
                pJuryConfiguration->setScoreboardRandom(false);
            } else {
                pJuryConfiguration->setScoreboardRandom(false);
                Log::warn("JuryConfiguration", "Unknown value of server/scoreboard_random '" + sValue + "' (expected: 'yes' or 'no')");
            }
        } else {
            Log::warn("JuryConfiguration", "Unknown property " + sSection + "/" + sName);
        }
    } else if (sSection.find("service") == 0) {
        // parse num service
        int nPos = std::string("service").length();
        int nServiceNum = std::stoi(sSection.substr(nPos));
        if (sName == "name") {
            pJuryConfiguration->setServiceName(nServiceNum, sValue);
        } else if (sName == "script_path") {
            pJuryConfiguration->setServiceScriptPath(nServiceNum, sValue);
        } else if (sName == "script_wait_in_sec") {
            int nValue = std::stoi(sValue); // TODO try catch
            pJuryConfiguration->setServiceScriptWait(nServiceNum, nValue);
        } else if (sName == "time_sleep_between_run_scripts_in_sec") {
            int nValue = std::stoi(sValue); // TODO try catch
            pJuryConfiguration->setServiceTimeSleep(nServiceNum, nValue);
        } else if (sName == "enabled") {
            if (sValue == "yes") {
                pJuryConfiguration->setServiceEnabled(nServiceNum, true);
            } else if (sValue == "no") {
                pJuryConfiguration->setServiceEnabled(nServiceNum, false);
            } else {
                pJuryConfiguration->setServiceEnabled(nServiceNum, false);
                Log::warn("JuryConfiguration", "Unknown value of " + sSection+ "/enabled '" + sValue + "' (expected: 'yes' or 'no')");
            }
        } else {
            Log::warn("JuryConfiguration", "Unknown property " + sSection + "/" + sName);
        }
    } else if (sSection.find("team") == 0) {
        // parse num team
        int nPos = std::string("team").length();
        int nTeamNum = std::stoi(sSection.substr(nPos));
        if (sName == "name") {
            pJuryConfiguration->setTeamName(nTeamNum, sValue);
        } else if (sName == "logo") {
            pJuryConfiguration->setTeamLogo(nTeamNum, sValue);
        } else if (sName == "ip_address") {
            pJuryConfiguration->setTeamIpAddress(nTeamNum, sValue);
        } else if (sName == "active") {
            if (sValue == "yes") {
                pJuryConfiguration->setTeamActive(nTeamNum, true);
            } else if (sValue == "no") {
                pJuryConfiguration->setTeamActive(nTeamNum, false);
            } else {
                pJuryConfiguration->setTeamActive(nTeamNum, false);
                Log::warn("JuryConfiguration", "Unknown value of " + sSection+ "/active '" + sValue + "' (expected: 'yes' or 'no')");
            }
        } else {
            Log::warn("JuryConfiguration", "Unknown property " + sSection + "/" + sName);
        }
    } else {
        if (sSection == "mysql_storage" || sSection == "ram_storage") // will be parsing in another places
            return 1;
        Log::warn("JuryConfiguration", "Unknown section " + sSection + "/" + sName);
    }
    return 1;
}

// ---------------------------------------------------------------------

bool JuryConfiguration::applyConfig(bool bLazyStart){
    bool bResult = true;

    std::string sConfigFile = m_sWorkspaceDir + "/conf.d/conf.ini";

    if (!Log::fileExists(sConfigFile)) {
        Log::err(TAG, "File " + sConfigFile + " does not exists ");
        return false;
    }

    Log::info(TAG, "Loading configuration... ");
    Log::info(TAG, "Config File: " + sConfigFile);

    if (ini_parse(sConfigFile.c_str(), handler_parse_jury_config, this) < 0) {
        Log::err(TAG, "Could not load config file");
        return false;
    }

    if (m_nFlagTimeliveInMin <= 0) {
         Log::err(TAG, "game/flag_timelive_in_min could not be less than 0");
        return false;
    }

    if (m_nFlagTimeliveInMin > 25) {
         Log::err(TAG, "game/flag_timelive_in_min could not be gather than 25");
        return false;
    }

    if (m_nScoreboardPort <= 10 || m_nScoreboardPort > 65536) {
        Log::err(TAG, "Wrong server/scoreboard_port (expected value od 11..65535)");
        return false;
    }

    if (!Log::dirExists(m_sScoreboardHtmlFolder)) {
        Log::err(TAG, "Directory '" + m_sScoreboardHtmlFolder + "' with scorebord does not exists");
        return false;
    }

    // services

    std::vector<int> vServicesConfErase;
    for (unsigned int i = 0; i < m_vServicesConf.size(); i++) {
        ModelServiceConf *pServiceConf = &m_vServicesConf[i];
         std::string sScriptPath = m_sWorkspaceDir + "/conf.d/" + pServiceConf->scriptPath();
        if (!Log::fileExists(sScriptPath)) {
            Log::err(TAG, "Script file not exists " + sScriptPath);
            return false;
        }

        if (pServiceConf->scriptWaitInSec() > pServiceConf->timeSleepBetweenRunScriptsInSec()) {
            Log::err(TAG, pServiceConf->id() + ": scriptWaitInSec must be less than timeSleepBetweenRunScriptsInSec");
            return false;
        }

        pServiceConf->setScriptPath(sScriptPath);
        if (pServiceConf->isEnabled()) {
            Log::info(TAG, "Registred " + pServiceConf->id() + " : " + pServiceConf->name());
        } else {
            // TODO erase
            Log::warn(TAG, "Disbaled " + pServiceConf->id() + " : " + pServiceConf->name());
            vServicesConfErase.push_back(i);
        }
    }

    // remove disabled services
    for (int i = vServicesConfErase.size()-1; i >= 0; i--) {
        int nPos = vServicesConfErase[i];
        m_vServicesConf.erase(m_vServicesConf.begin() + nPos);
    }

    if(m_vServicesConf.size() == 0){
        Log::err(TAG, "Services does not defined");
        return false;
    }


    if (bLazyStart) {
       
        // teams by scanning
        SearchLazyConf searchLazyConf(this->scoreboardPort());
        searchLazyConf.scan();
        m_vTeamsConf.clear();
        m_vTeamsConf = searchLazyConf.getFoundTeams();

    } else {
        
        // teams from config

        std::vector<int> vTeamsConfErase;
        for (unsigned int i = 0; i < m_vTeamsConf.size(); i++) {
            ModelTeamConf *pTeamConf = &m_vTeamsConf[i];

            // TODO check logo

            if(pTeamConf->isActive()){
                Log::info(TAG, "Registred " + pTeamConf->id()
                    + " : " + pTeamConf->name()
                    + " (ip address: " + pTeamConf->ipAddress() + ")");
            }else{
                vTeamsConfErase.push_back(i);
                Log::warn(TAG, "Inactive " + pTeamConf->id()
                    + " : " + pTeamConf->name()
                    + " (ip address: " + pTeamConf->ipAddress() + ")");
            }
        }

        // remove inactive teams
        for (int i = vTeamsConfErase.size()-1; i >= 0; i--) {
            int nPos = vTeamsConfErase[i];
            m_vTeamsConf.erase(m_vTeamsConf.begin() + nPos);
        }
    }

    if (m_vTeamsConf.size() == 0) {
        Log::err(TAG, "Teams does not defined");
        return false;
    }

    // check the game config

    if (bLazyStart) {
        m_nGameStartUTCInSec = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        m_nGameEndUTCInSec = m_nGameStartUTCInSec + 4*60*60; // 4 hours
    }

    if (m_nGameStartUTCInSec == 0) {
        Log::err(TAG, "'game/start' - not found");
        return false;
    }

    if (m_nGameEndUTCInSec == 0) {
        Log::err(TAG, "'game/end' - not found");
        return false;
    }

    if (m_nGameEndUTCInSec < m_nGameStartUTCInSec) {
        Log::err(TAG, "'game/end' must be gather then 'game/start'");
        return false;
    }

    // game.conf - will be override configs from conf.ini
    UtilsParseConfig gameConf = UtilsParseConfig(m_sWorkspaceDir + "/game.conf");
    if (gameConf.parseConfig()) {
        m_sGameName = gameConf.getStringValueFromConfig("game.name", m_sGameName);
        Log::info(TAG, "game.name: " + m_sGameName);
    }
    
    // scoreboard
    m_pScoreboard = new ModelScoreboard(m_bScoreboardRandom, m_nGameStartUTCInSec, m_nGameEndUTCInSec, m_vTeamsConf, m_vServicesConf);

    // storage
    Log::info(TAG, "Storage: " + m_sUseStorage);
    m_pStorage = NULL;
    if (m_sUseStorage == "mysql") {
        this->setStorage(new MySqlStorage(m_pScoreboard, m_nGameStartUTCInSec, m_nGameEndUTCInSec));
        Log::info(TAG, "Init storage: " + m_sUseStorage);
    }else if(m_sUseStorage == "ram") {
        this->setStorage(new RamStorage(m_pScoreboard, m_nGameStartUTCInSec, m_nGameEndUTCInSec));
        Log::info(TAG, "Init storage: " + m_sUseStorage);
    }
    if (m_pStorage == NULL) {
        Log::err(TAG, "server/use_storage: '" + m_sUseStorage + "' is unknown type of storage");
		return false;
    }

    // configure storage
    if (!m_pStorage->applyConfigFromFile(sConfigFile, m_vTeamsConf, m_vServicesConf)) {
        Log::err(TAG, "Could not init configuration storage");
        return false;
    }

    return bResult;
}

// ---------------------------------------------------------------------

void JuryConfiguration::setUseStorage(const std::string &sUseStorage) {
    m_sUseStorage = sUseStorage;
}

// ---------------------------------------------------------------------

IStorage *JuryConfiguration::storage(){
    return m_pStorage;
}

// ---------------------------------------------------------------------

void JuryConfiguration::setStorage(IStorage *pStorage){
    m_pStorage = pStorage;
}

// ---------------------------------------------------------------------

ModelScoreboard *JuryConfiguration::scoreboard(){
    return m_pScoreboard;
}

// ---------------------------------------------------------------------

void JuryConfiguration::setFlagTimeliveInMin(int nMinutes) {
    m_nFlagTimeliveInMin = nMinutes;
}

// ---------------------------------------------------------------------

int JuryConfiguration::flagTimeliveInMin(){
    return m_nFlagTimeliveInMin;
}

// ---------------------------------------------------------------------

std::vector<ModelTeamConf> &JuryConfiguration::teamsConf() {
    return m_vTeamsConf;
}

// ---------------------------------------------------------------------

std::vector<ModelServiceConf> &JuryConfiguration::servicesConf() {
    return m_vServicesConf;
}

// ---------------------------------------------------------------------

ModelServiceConf *JuryConfiguration::findOrCreateServiceConf(int nServiceNum) {
    for (unsigned int i = 0; i < m_vServicesConf.size(); i++) {
        if (m_vServicesConf[i].num() == nServiceNum) {
            return &m_vServicesConf[i];
        }
    }
    // default values of service config
    ModelServiceConf serviceConf;
    serviceConf.setId("service" + std::to_string(nServiceNum));
    serviceConf.setNum(nServiceNum);
    serviceConf.setEnabled(true);
    serviceConf.setScriptWaitInSec(10);
    serviceConf.setTimeSleepBetweenRunScriptsInSec(10);
    m_vServicesConf.push_back(serviceConf);
    return &m_vServicesConf[m_vServicesConf.size()-1];
}

// ---------------------------------------------------------------------

void JuryConfiguration::setServiceName(int nServiceNum, const std::string &sServiceName) {
    ModelServiceConf *pServiceConf = findOrCreateServiceConf(nServiceNum);
    pServiceConf->setName(sServiceName);
}

// ---------------------------------------------------------------------

void JuryConfiguration::setServiceScriptPath(int nServiceNum, const std::string &sScriptPath) {
    ModelServiceConf *pServiceConf = findOrCreateServiceConf(nServiceNum);
    pServiceConf->setScriptPath(sScriptPath);
}

// ---------------------------------------------------------------------

void JuryConfiguration::setServiceEnabled(int nServiceNum, bool bEnabled) {
    ModelServiceConf *pServiceConf = findOrCreateServiceConf(nServiceNum);
    pServiceConf->setEnabled(bEnabled);
}

// ---------------------------------------------------------------------

void JuryConfiguration::setServiceScriptWait(int nServiceNum, int nScriptWait) {
    ModelServiceConf *pServiceConf = findOrCreateServiceConf(nServiceNum);
    pServiceConf->setScriptWaitInSec(nScriptWait);
}

// ---------------------------------------------------------------------

void JuryConfiguration::setServiceTimeSleep(int nServiceNum, int nTimeSleep) {
    ModelServiceConf *pServiceConf = findOrCreateServiceConf(nServiceNum);
    pServiceConf->setTimeSleepBetweenRunScriptsInSec(nTimeSleep);
}

// ---------------------------------------------------------------------

ModelTeamConf *JuryConfiguration::findOrCreateTeamConf(int nTeamNum) {
    for (unsigned int i = 0; i < m_vTeamsConf.size(); i++) {
        if (m_vTeamsConf[i].num() == nTeamNum) {
            return &m_vTeamsConf[i];
        }
    }
    // default values of service config
    ModelTeamConf teamConf;
    teamConf.setId("team" + std::to_string(nTeamNum));
    teamConf.setNum(nTeamNum);
    teamConf.setActive(true);
    m_vTeamsConf.push_back(teamConf);
    return &m_vTeamsConf[m_vTeamsConf.size()-1];
}

// ---------------------------------------------------------------------

void JuryConfiguration::setTeamName(int nTeamNum, const std::string &sTeamName) {
    ModelTeamConf *pTeamConf = findOrCreateTeamConf(nTeamNum);
    pTeamConf->setName(sTeamName);
}

// ---------------------------------------------------------------------

void JuryConfiguration::setTeamLogo(int nTeamNum, const std::string &sTeamLogo) {
    ModelTeamConf *pTeamConf = findOrCreateTeamConf(nTeamNum);
    pTeamConf->setLogo(sTeamLogo);
}

// ---------------------------------------------------------------------

void JuryConfiguration::setTeamIpAddress(int nTeamNum, const std::string &sTeamIpAddress) {
    ModelTeamConf *pTeamConf = findOrCreateTeamConf(nTeamNum);
    pTeamConf->setIpAddress(sTeamIpAddress);
}

// ---------------------------------------------------------------------

void JuryConfiguration::setTeamActive(int nTeamNum, bool bActive) {
    ModelTeamConf *pTeamConf = findOrCreateTeamConf(nTeamNum);
    pTeamConf->setActive(bActive);
}

// ---------------------------------------------------------------------

void JuryConfiguration::setScoreboardPort(int nPort) {
    m_nScoreboardPort = nPort;
}

// ---------------------------------------------------------------------

int JuryConfiguration::scoreboardPort(){
    return m_nScoreboardPort;
}

// ---------------------------------------------------------------------

void JuryConfiguration::setScoreboardHtmlFolder(const std::string &sHtmlFolder) {
    if (sHtmlFolder.length() > 0) {
        if (sHtmlFolder[0] != '/') {
            m_sScoreboardHtmlFolder = m_sWorkspaceDir + '/' + sHtmlFolder;
        } else {
            m_sScoreboardHtmlFolder = sHtmlFolder;
        }
    } else {
        m_sScoreboardHtmlFolder = m_sWorkspaceDir;
    }
}

// ---------------------------------------------------------------------

std::string JuryConfiguration::scoreboardHtmlFolder() {
    return m_sScoreboardHtmlFolder;
}

// ---------------------------------------------------------------------

void JuryConfiguration::setScoreboardRandom(bool bRandom) {
    m_bScoreboardRandom = bRandom;
}

// ---------------------------------------------------------------------

bool JuryConfiguration::scoreboardRandom() {
    return m_bScoreboardRandom;
}

// ---------------------------------------------------------------------

void JuryConfiguration::setGameName(const std::string &sGameName) {
    m_sGameName = sGameName;
}

// ---------------------------------------------------------------------

std::string JuryConfiguration::gameName(){
    return m_sGameName;
}

// ---------------------------------------------------------------------

void JuryConfiguration::setGameStart(const std::string &sGameStart) {
    m_sGameStart = sGameStart;

    std::istringstream in{m_sGameStart.c_str()};
    date::sys_seconds tp;
    in >> date::parse("%Y-%m-%d %T", tp);
    m_nGameStartUTCInSec = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
    Log::info(TAG, "Game start: " + std::to_string(m_nGameStartUTCInSec));
}

// ---------------------------------------------------------------------

int JuryConfiguration::gameStartUTCInSec() {
    return m_nGameStartUTCInSec;
}

// ---------------------------------------------------------------------

void JuryConfiguration::setGameEnd(const std::string &sGameEnd) {
    m_sGameEnd = sGameEnd;

    std::istringstream in{m_sGameEnd.c_str()};
    date::sys_seconds tp;
    in >> date::parse("%Y-%m-%d %T", tp);
    m_nGameEndUTCInSec = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
    Log::info(TAG, "Game end: " + std::to_string(m_nGameEndUTCInSec));
}

// ---------------------------------------------------------------------

int JuryConfiguration::gameEndUTCInSec() {
    return m_nGameEndUTCInSec;
}

// ---------------------------------------------------------------------