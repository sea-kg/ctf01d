
#include "employ_team_logos.h"
#include <wsjcpp_core.h>

// ---------------------------------------------------------------------
// EmployTeamLogos

REGISTRY_WJSCPP_EMPLOY(EmployTeamLogos)

EmployTeamLogos::EmployTeamLogos() 
: WsjcppEmployBase(EmployTeamLogos::name(), {}) {
    TAG = EmployTeamLogos::name();
}

// ---------------------------------------------------------------------

bool EmployTeamLogos::init() {
    WsjcppLog::info(TAG, "init");
    return true;
}

// ---------------------------------------------------------------------

bool EmployTeamLogos::deinit() {
    WsjcppLog::info(TAG, "deinit");
    return true;
}

// ---------------------------------------------------------------------

bool EmployTeamLogos::loadTeamLogo(const std::string &sTeamId, const std::string &sTeamLogoPath) {
    if (!WsjcppCore::fileExists(sTeamLogoPath)) {
        WsjcppLog::err(TAG, "File '" + sTeamLogoPath + "' did not found");
        return false;
    }
    TeamLogo *pTeamLogo = new TeamLogo();
    m_mapTeamLogos[sTeamId] = pTeamLogo;
    pTeamLogo->sTeamId = sTeamId;
    pTeamLogo->pBuffer = nullptr;
    pTeamLogo->nBufferSize = 0;
    pTeamLogo->sFilename = WsjcppCore::extractFilename(sTeamLogoPath);
    pTeamLogo->sFilepath = sTeamLogoPath;
    if (!WsjcppCore::readFileToBuffer(sTeamLogoPath, &(pTeamLogo->pBuffer), (pTeamLogo->nBufferSize))) {
        WsjcppLog::err(TAG, "Could not read file '" + sTeamLogoPath + "'");
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------

TeamLogo *EmployTeamLogos::findTeamLogo(const std::string &sTeamId) {
    std::map<std::string, TeamLogo *>::iterator it = m_mapTeamLogos.find(sTeamId);
    if (it != m_mapTeamLogos.end()) {
        return it->second;
    }
    return nullptr;
}

// ---------------------------------------------------------------------

