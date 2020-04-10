#include "read_teams_conf.h"
#include <utils_logger.h>
#include <sstream>
#include <storages.h>
#include <ctime>
#include <locale>
#include <date.h>
#include <iostream>
#include <sstream>
#include <conf_file_parser.h>
#include <wsjcpp_core.h>

ReadTeamsConf::ReadTeamsConf(const std::string &sWorkspaceDir) {
    TAG = "ReadTeamsConf";
    m_sWorkspaceDir = sWorkspaceDir;
}

// ---------------------------------------------------------------------

bool ReadTeamsConf::read(std::vector<Team> &vTeams) {
    std::string sRootTeamsDir = m_sWorkspaceDir + "/teams/";
    
    if (!WsjcppCore::dirExists(sRootTeamsDir)) {
        Log::err(TAG, "Directory " + sRootTeamsDir + " not exists");
        return false;
    }
    Log::info(TAG, "Search team.conf");

    std::vector<std::string> vListOfTeams = WsjcppCore::listOfFiles(sRootTeamsDir);

    if (vListOfTeams.size() == 0) {
        Log::err(TAG, "Teams does not defined");
        return false;
    }
    vTeams.clear();
    for (int i = 0; i < vListOfTeams.size(); i++) {
        std::string sFilename = vListOfTeams[i];
        std::stringstream test(sFilename);
        std::string sTeamId = "";
        if (std::getline(test, sTeamId, '.')) {
        }
        std::string sTeamConfPath =  sRootTeamsDir + sTeamId + ".conf";
        Log::info(TAG, "Reading " + sTeamConfPath);
        if (!WsjcppCore::fileExists(sTeamConfPath)) {
            Log::err(TAG, "File " + sTeamConfPath + " not exists");
            return false;
        }
        ConfFileParser teamConf = ConfFileParser(sTeamConfPath);
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

        for (unsigned int i = 0; i < vTeams.size(); i++) {
            if (vTeams[i].id() == sTeamId) {
                Log::err(TAG, "Already registered team with id " + sTeamId);
                return false;
            }
        }
        // default values of service config
        Team _teamConf;
        _teamConf.setId(sTeamId);
        _teamConf.setName(sTeamName);
        _teamConf.setActive(true);
        _teamConf.setIpAddress(sTeamIpAddress);
        _teamConf.setLogo(sTeamLogo);

        vTeams.push_back(_teamConf);
        Log::ok(TAG, "Registered team " + sTeamId);
    }
    
    if (vTeams.size() == 0) {
        Log::err(TAG, "No one defined team " + sRootTeamsDir);
        return false;
    }

    return true;
}

// ---------------------------------------------------------------------

