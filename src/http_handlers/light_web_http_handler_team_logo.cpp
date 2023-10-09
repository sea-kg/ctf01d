/**********************************************************************************
 *          Project
 *  _______ _________ _______  _______  __    ______
 * (  ____ \\__   __/(  ____ \(  __   )/  \  (  __  \
 * | (    \/   ) (   | (    \/| (  )  |\/) ) | (  \  )
 * | |         | |   | (__    | | /   |  | | | |   ) |
 * | |         | |   |  __)   | (/ /) |  | | | |   | |
 * | |         | |   | (      |   / | |  | | | |   ) |
 * | (____/\   | |   | )      |  (__) |__) (_| (__/  )
 * (_______/   )_(   |/       (_______)\____/(______/
 *
 * MIT License
 * Copyright (c) 2018-2023 Evgenii Sopov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ***********************************************************************************/

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


