#include "http_handler_api_v1.h"
#include <fstream>
#include <string>
#include <regex>
#include <algorithm>
#include <time.h>

#include <utils_logger.h>
#include <light_http_server.h>
#include <ts.h>
#include <fs.h>
#include <str.h>

// ----------------------------------------------------------------------

HttpHandlerApiV1::HttpHandlerApiV1(Config *pConfig) 
    : LightHttpHandlerBase("api-v1") {

    m_pConfig = pConfig;    

    TAG = "HttpHandlerApiV1";
    
    m_jsonGame["game_name"] = m_pConfig->gameName();
    m_jsonGame["game_start"] = TS::formatTimeUTC(m_pConfig->gameStartUTCInSec()) + " (UTC)";
    m_jsonGame["game_end"] = TS::formatTimeUTC(m_pConfig->gameEndUTCInSec()) + " (UTC)";
    m_jsonGame["game_has_coffee_break"] = m_pConfig->gameHasCoffeeBreak();
    m_jsonGame["game_coffee_break_start"] = TS::formatTimeUTC(m_pConfig->gameCoffeeBreakStartUTCInSec()) + " (UTC)";
    m_jsonGame["game_coffee_break_end"] = TS::formatTimeUTC(m_pConfig->gameCoffeeBreakEndUTCInSec()) + " (UTC)";
    m_jsonGame["teams"] = nlohmann::json::array();
    m_jsonGame["services"] = nlohmann::json::array();

    for (unsigned int i = 0; i < m_pConfig->servicesConf().size(); i++) {
        Service serviceConf = m_pConfig->servicesConf()[i];
        nlohmann::json serviceInfo;
        serviceInfo["id"] = serviceConf.id();
        serviceInfo["name"] = serviceConf.name();
        m_jsonGame["services"].push_back(serviceInfo);
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

bool HttpHandlerApiV1::canHandle(const std::string &sWorkerId, LightHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    std::string path = pRequest->requestPath();
    return path == "/api/v1/game"
        || path == "/api/v1/scoreboard"
        || path == "/api/v1/teams"
        || path == "/api/v1/services"
        || path == "/flag";
}

// ----------------------------------------------------------------------

bool HttpHandlerApiV1::handle(const std::string &sWorkerId, LightHttpRequest *pRequest){
    std::string _tag = TAG + "-" + sWorkerId;
    LightHttpResponse response(pRequest->sockFd());
    
    if(pRequest->requestPath() == "/api/v1/game") {
        std::string sJsonResponse = m_jsonGame.dump();
        response.noCache().ok().sendBuffer("game.json", sJsonResponse.c_str(), sJsonResponse.length());
        return true;
    } else if (pRequest->requestPath() == "/") {
        response.noCache().ok().sendText(m_sIndexHtml);
        return true;
    } else if(pRequest->requestPath() == "/api/v1/scoreboard") {
        std::string sJsonResponse = m_pConfig->scoreboard()->toJson().dump();
        response.noCache().ok().sendBuffer("scoreboard.json", sJsonResponse.c_str(), sJsonResponse.length());
        return true;
    } else if(pRequest->requestPath() == "/api/v1/teams") {
        std::string sJsonResponse = m_jsonTeams.dump();
        response.noCache().ok().sendBuffer("teams.json", sJsonResponse.c_str(), sJsonResponse.length());
        return true;
    } else if(pRequest->requestPath() == "/api/v1/services") {
        std::string sJsonResponse = m_jsonServices.dump();
        response.noCache().ok().sendBuffer("services.json", sJsonResponse.c_str(), sJsonResponse.length());
        return true;
    } else if(pRequest->requestPath() == "/flag") {
        response.noCache();

        auto now = std::chrono::system_clock::now().time_since_epoch();
        int nCurrentTimeSec = std::chrono::duration_cast<std::chrono::seconds>(now).count();

        if (nCurrentTimeSec < m_pConfig->gameStartUTCInSec()) {
            response.badRequest().sendText("Error(-8): Game not started yet");
            Log::warn(TAG, "Error(-8): Game not started yet");
            return true;
        }

        if (nCurrentTimeSec > m_pConfig->gameEndUTCInSec()) {
            response.badRequest().sendText("Error(-9): Game already ended");
            Log::warn(TAG, "Error(-9): Game already ended");
            return true;
        }

        std::map<std::string,std::string>::iterator itTeamID;
        itTeamID = pRequest->requestQueryParams().find("teamid");
        if (itTeamID == pRequest->requestQueryParams().end()) {
            response.badRequest().sendText("Error(-10): Not found get-parameter 'teamid'");
            Log::warn(TAG, "Error(-10): Not found get-parameter 'teamid'");
            return true;
        }

        std::map<std::string,std::string>::iterator itFlag;
        itFlag = pRequest->requestQueryParams().find("flag");
        if (itFlag == pRequest->requestQueryParams().end()) {
            response.badRequest().sendText("Error(-11): Not found get-parameter 'flag'");
            Log::warn(TAG, "Error(-11): Not found get-parameter 'flag'");
            return true;
        }

        std::string sTeamId = itTeamID->second;
        Str::trim(sTeamId);
        std::transform(sTeamId.begin(), sTeamId.end(), sTeamId.begin(), ::tolower);
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
        std::string sFlag = itFlag->second;
        std::transform(sFlag.begin(), sFlag.end(), sFlag.begin(), ::tolower);
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

        if (flag.teamStole() != "") {
            response.forbidden().sendText("Error(-170): flag already stoled by '" + flag.teamStole() + "'");
            Log::err(TAG, "Error(-170): Recieved flag {" + sFlag + "} from {" + sTeamId + "} (flag already stolen by '" + flag.teamStole() + "' team)");
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

        if (!m_pConfig->storage()->updateTeamStole(flag.value(), sTeamId)) {
            response.forbidden().sendText("Error(-300): You are late");
            Log::err(TAG, "Error(-300): Recieved flag {" + sFlag + "} from {" + sTeamId + "} (You are late)");
            return true;
        }

        // TODO light update scoreboard
        // TODO send how match points will be added
        m_pConfig->scoreboard()->incrementAttackScore(sTeamId, flag.serviceId());
        response.ok().sendText("Accepted");
        Log::ok(TAG, "Accepted: Recieved flag {" + sFlag + "} from {" + sTeamId + "} (Accepted)");
        return true;
    }
    return false;
}

// ----------------------------------------------------------------------
