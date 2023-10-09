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

#include "employ_team_logos.h"
#include <wsjcpp_core.h>
#include <employ_config.h>

// ---------------------------------------------------------------------
// EmployTeamLogos

REGISTRY_WJSCPP_SERVICE_LOCATOR(EmployTeamLogos)

EmployTeamLogos::EmployTeamLogos() 
: WsjcppEmployBase(EmployTeamLogos::name(), { EmployConfig::name() }) {
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

