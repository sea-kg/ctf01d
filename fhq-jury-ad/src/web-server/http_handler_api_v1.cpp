#include "http_handler_api_v1.h"
#include <fstream>
#include <string>
#include <regex>
#include <algorithm>
#include <time.h>

#include <utils_logger.h>
#include <wsjcpp_light_web_server.h>
#include <wsjcpp_core.h>
#include <fs.h>
#include <str.h>

// ----------------------------------------------------------------------

HttpHandlerApiV1::HttpHandlerApiV1(Config *pConfig) 
: WsjcppLightWebHttpHandlerBase("api-v1") {

    m_pConfig = pConfig;    

    TAG = "HttpHandlerApiV1";
    
    m_jsonGame["game_name"] = m_pConfig->gameName();
    m_jsonGame["game_start"] = WsjcppCore::formatTimeUTC(m_pConfig->gameStartUTCInSec()) + " (UTC)";
    m_jsonGame["game_end"] = WsjcppCore::formatTimeUTC(m_pConfig->gameEndUTCInSec()) + " (UTC)";
    m_jsonGame["game_has_coffee_break"] = m_pConfig->gameHasCoffeeBreak();
    m_jsonGame["game_coffee_break_start"] = WsjcppCore::formatTimeUTC(m_pConfig->gameCoffeeBreakStartUTCInSec()) + " (UTC)";
    m_jsonGame["game_coffee_break_end"] = WsjcppCore::formatTimeUTC(m_pConfig->gameCoffeeBreakEndUTCInSec()) + " (UTC)";
    m_jsonGame["teams"] = nlohmann::json::array();
    m_jsonGame["services"] = nlohmann::json::array();

    for (unsigned int i = 0; i < m_pConfig->servicesConf().size(); i++) {
        Service serviceConf = m_pConfig->servicesConf()[i];
        if (serviceConf.isEnabled()) {
            nlohmann::json serviceInfo;
            serviceInfo["id"] = serviceConf.id();
            serviceInfo["name"] = serviceConf.name();
            serviceInfo["round_time_in_sec"] = serviceConf.timeSleepBetweenRunScriptsInSec();
            m_jsonGame["services"].push_back(serviceInfo);
        }
    }

    for (unsigned int i = 0; i < m_pConfig->teamsConf().size(); i++) {
        Team teamConf = m_pConfig->teamsConf()[i];
        std::string sTeamId = teamConf.id();
        nlohmann::json teamInfo;
        teamInfo["id"] = teamConf.id();
        teamInfo["name"] = teamConf.name();
        teamInfo["ip_address"] = teamConf.ipAddress();
        teamInfo["logo"] = teamConf.logo();
        m_jsonGame["teams"].push_back(teamInfo);
        m_jsonTeams["teams"].push_back(teamInfo);
    }
}

// ----------------------------------------------------------------------

bool HttpHandlerApiV1::canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    std::string path = pRequest->getRequestPath();
    return path == "/api/v1/game"
        || path == "/api/v1/scoreboard"
        || path == "/api/v1/teams"
        || path == "/api/v1/services"
        || path == "/flag";
}

// ----------------------------------------------------------------------

bool HttpHandlerApiV1::handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest){
    std::string _tag = TAG + "-" + sWorkerId;
    WsjcppLightWebHttpResponse response(pRequest->getSockFd());
    
    if(pRequest->getRequestPath() == "/api/v1/game") {
        std::string sJsonResponse = m_jsonGame.dump();
        response.noCache().ok().sendBuffer("game.json", sJsonResponse.c_str(), sJsonResponse.length());
        return true;
    } else if (pRequest->getRequestPath() == "/") {
        response.noCache().ok().sendText(m_sIndexHtml);
        return true;
    } else if(pRequest->getRequestPath() == "/api/v1/scoreboard") {
        std::string sJsonResponse = m_pConfig->scoreboard()->toJson().dump();
        response.noCache().ok().sendBuffer("scoreboard.json", sJsonResponse.c_str(), sJsonResponse.length());
        return true;
    } else if(pRequest->getRequestPath() == "/api/v1/teams") {
        std::string sJsonResponse = m_jsonTeams.dump();
        response.noCache().ok().sendBuffer("teams.json", sJsonResponse.c_str(), sJsonResponse.length());
        return true;
    } else if(pRequest->getRequestPath() == "/api/v1/services") {
        std::string sJsonResponse = m_jsonServices.dump();
        response.noCache().ok().sendBuffer("services.json", sJsonResponse.c_str(), sJsonResponse.length());
        return true;
    } else if(pRequest->getRequestPath() == "/flag") {
        response.noCache();

        auto now = std::chrono::system_clock::now().time_since_epoch();
        int nCurrentTimeSec = std::chrono::duration_cast<std::chrono::seconds>(now).count();

        if (nCurrentTimeSec < m_pConfig->gameStartUTCInSec()) {
            response.badRequest().sendText("Error(-8): Game not started yet");
            Log::warn(TAG, "Error(-8): Game not started yet");
            return true;
        }

        if (m_pConfig->gameHasCoffeeBreak() 
            && nCurrentTimeSec > m_pConfig->gameCoffeeBreakStartUTCInSec()
            && nCurrentTimeSec < m_pConfig->gameCoffeeBreakEndUTCInSec()
        ) {
            response.badRequest().sendText("Error(-8): Game on coffeebreak now");
            Log::warn(TAG, "Error(-8): Game on coffeebreak now");
            return true;
        }

        if (nCurrentTimeSec > m_pConfig->gameEndUTCInSec()) {
            response.badRequest().sendText("Error(-9): Game already ended");
            Log::warn(TAG, "Error(-9): Game already ended");
            return true;
        }
        std::vector<WsjcppLightWebHttpRequestQueryValue> vParams = pRequest->getRequestQueryParams();
        
        std::string sTeamId = "";
        std::string sFlag = "";
        for (int i = 0; i < vParams.size(); i++) {
            std::string sParamName = vParams[i].getName();
            if (sParamName == "teamid") {
                sTeamId = vParams[i].getValue();
                sTeamId = WsjcppCore::trim(sTeamId);
                sTeamId = WsjcppCore::toLower(sTeamId);
            } else if (sParamName == "flag") {
                sFlag = vParams[i].getValue();
                sFlag = WsjcppCore::trim(sFlag);
                sFlag = WsjcppCore::toLower(sFlag);
            }

        }

        if (sTeamId == "") {
            static const std::string sError = "Error(-10): Not found get-parameter 'teamid' or parameter is empty";
            response.badRequest().sendText(sError);
            Log::warn(TAG, sError);
            return true;
        }

        if (sFlag == "") {
            static const std::string sError = "Error(-11): Not found get-parameter 'flag' or parameter is empty";
            response.badRequest().sendText(sError);
            Log::warn(TAG, sError);
            return true;
        }

        /*int nTeamNum = atoi(sTeamID.c_str());
        if (nTeamNum == 0) {
            pRequest->response(
                LightHttpRequest::RESP_BAD_REQUEST, 
                "text/html", 
                "Error(-12): 'teamid' must be number");
            Log::warn(TAG, "Error(-12): 'teamid' must be number");
            return true;
        }*/

        bool bTeamFound = false;
        for (unsigned int iteam = 0; iteam < m_pConfig->teamsConf().size(); iteam++) {
            Team teamConf = m_pConfig->teamsConf()[iteam];
            if (teamConf.id() == sTeamId) {
                bTeamFound = true;
            }
        }

        if (!bTeamFound) {
            response.badRequest().sendText("Error(-130): this is team not found");
            Log::err(TAG, "Error(-130): this is team not found");
            return true;
        }

        const std::regex reFlagFormat("[a-f0-9]{8,8}-[a-f0-9]{4,4}-[a-f0-9]{4,4}-[a-f0-9]{4,4}-[a-f0-9]{12,12}");
        if (!std::regex_match(sFlag, reFlagFormat)) {
            response.badRequest().sendText("Error(-140): flag has wrong format");
            Log::err(TAG, "Error(-140): flag has wrong format");
            return true;
        }
        m_pConfig->scoreboard()->incrementTries(sTeamId);
        m_pConfig->storage()->insertFlagAttempt(sTeamId, sFlag);

        Flag flag;
        if (!m_pConfig->scoreboard()->findFlagLive(sFlag, flag)) {
            response
                .forbidden()
                .sendText("Error(-150): flag is too old or flag never existed or flag alredy stole. "
                    "Recieved flag {" + sFlag + "} from {" + sTeamId + "} (flag is too old or flag never existed)");
            return true;
        }

        long nCurrentTimeMSec = (long)nCurrentTimeSec;
        nCurrentTimeMSec = nCurrentTimeMSec*1000;

        if (flag.timeEnd() < nCurrentTimeMSec) {
            response.forbidden().sendText("Error(-151): flag is too old");
            Log::err(TAG, "Error(-151): Recieved flag {" + sFlag + "} from {" + sTeamId + "} (flag is too old)");
            return true;
        }

        if (flag.teamStole() == sTeamId) {
            response.forbidden().sendText("Error(-160): flag already stole by your team");
            Log::err(TAG, "Error(-160): Recieved flag {" + sFlag + "} from {" + sTeamId + "} (flag already stole by your team)");
            return true;
        }
        
        if (flag.teamId() == sTeamId) {
            response.forbidden().sendText("Error(-180): this is your flag");
            Log::err(TAG, "Error(-180): Recieved flag {" + sFlag + "} from {" + sTeamId + "} (this is your flag)");
            return true;
        }

        std::string sServiceStatus = m_pConfig->scoreboard()->serviceStatus(sTeamId, flag.serviceId());

        // std::cout << "sServiceStatus: " << sServiceStatus << "\n";

        if (sServiceStatus != ServiceStatusCell::SERVICE_UP) {
            response.forbidden().sendText("Error(-190): Your same service is dead");
            Log::err(TAG, "Error(-190): Recieved flag {" + sFlag + "} from {" + sTeamId + "} (Your same service is dead)");
            return true;
        }

        if (m_pConfig->storage()->isAlreadyStole(flag, sTeamId)) {
            response.forbidden().sendText("Error(-170): flag already stoled by your");
            Log::err(TAG, "Error(-170): Recieved flag {" + sFlag + "} from {" + sTeamId + "} (flag already stolen by '" + flag.teamStole() + "' team)");
            return true;
        }

        // TODO light update scoreboard
        int nPoints = m_pConfig->scoreboard()->incrementAttackScore(flag, sTeamId);
        std::string sPoints = std::to_string(double(nPoints) / 10.0);

        response.ok().sendText("Accepted (+ " + sPoints + " points)");
        Log::ok(TAG, "Accepted: Recieved flag {" + sFlag + "} from {" + sTeamId + "} (Accepted + " + sPoints + ")");
        return true;
    }
    return false;
}

// ----------------------------------------------------------------------
