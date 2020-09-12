
#include "light_web_http_handler_team_logo.h"
#include <wsjcpp_core.h>
#include <employ_team_logos.h>
#include <string_view>
#include <string>

// ---------------------------------------------------------------------
// LightWebHttpHandlerTeamLogo

LightWebHttpHandlerTeamLogo::LightWebHttpHandlerTeamLogo() 
: WsjcppLightWebHttpHandlerBase("team_logo") {
    TAG = "LightWebHttpHandlerTeamLogo";
    m_sPrefix = "/team-logo/";
    m_nPrefixLength = m_sPrefix.length();
}

// ---------------------------------------------------------------------

bool LightWebHttpHandlerTeamLogo::canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
    // std::string _tag = TAG + "-" + sWorkerId;
    std::string sRequestPath = pRequest->getRequestPath();
    if (sRequestPath.rfind(m_sPrefix, 0) == 0) {
        return true;
    }
    return false;
}

// ---------------------------------------------------------------------

bool LightWebHttpHandlerTeamLogo::handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    std::string sRequestPath = pRequest->getRequestPath();
    std::string sTeamId = sRequestPath.substr(m_nPrefixLength, sRequestPath.length() - m_nPrefixLength);

    EmployTeamLogos *pTeamLogos = findWsjcppEmploy<EmployTeamLogos>();
    TeamLogo *pLogo = pTeamLogos->findTeamLogo(sTeamId);
    
    WsjcppLightWebHttpResponse resp(pRequest->getSockFd());
    if (pLogo == nullptr) {
        resp.cacheSec(60).notFound().sendEmpty();
    } else {
        resp.cacheSec(60).ok().sendBuffer(pLogo->sFilename, pLogo->pBuffer, pLogo->nBufferSize);
    }
    return true;
}


