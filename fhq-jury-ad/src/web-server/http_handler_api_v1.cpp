#include "http_handler_api_v1.h"
#include <fstream>
#include <string>
#include <regex>
#include <algorithm>
#include <time.h>

#include <utils_logger.h>
#include <light_http_request.h>
#include <light_http_response.h>
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
        m_jsonGame["teams"].push_back(teamInfo);
    }

    prepareIndexHtml();
}

// ----------------------------------------------------------------------

void HttpHandlerApiV1::prepareIndexHtml() {
    // Loading index-template.html
    m_sIndexHtml = "";
    std::string fullPath = m_pConfig->scoreboardHtmlFolder() + "/index.html";
    std::ifstream ifs(fullPath.c_str());
    m_sIndexHtml.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

    // generate teams-services table
    std::string sContent = 
        "<div class='scoreboard' id='table_scoreboard'>\n" // TODO must be none
        "    <div class='hdrs'>\n"
        "        <div class='place'>#</div>\n"
        "        <div class='team-logo'></div>\n"
        "        <div class='team'>Team</div>\n"
        "        <div class='score'>Score</div>\n";

    for (unsigned int iservice = 0; iservice < m_pConfig->servicesConf().size(); iservice++) {
        Service serviceConf = m_pConfig->servicesConf()[iservice];
        sContent += "<div class='service'>" + serviceConf.name() + "<br><small>(service)</small></div>\n";
        
        nlohmann::json serviceInfo;
        serviceInfo["id"] = serviceConf.id();
        serviceInfo["name"] = serviceConf.name();
        m_jsonServices[serviceConf.id()] = serviceInfo;
    }

    sContent += "  </div>\n";

    for (unsigned int iteam = 0; iteam < m_pConfig->teamsConf().size(); iteam++) {
        Team teamConf = m_pConfig->teamsConf()[iteam];
        std::string sTeamId = teamConf.id();
            
        nlohmann::json teamInfo;
        teamInfo["id"] = teamConf.id();
        teamInfo["name"] = teamConf.name();
        teamInfo["ip_address"] = teamConf.ipAddress();
        m_jsonTeams[sTeamId] = teamInfo;

        sContent += 
            "    <div class='tm' id='" + sTeamId + "'>\n"
            "        <div class='place' id='" + sTeamId + "_place' >" + std::to_string(iteam + 1) + "</div>\n"
            "        <div class='team-logo' id='" + sTeamId + "_logo' ><img class='team-logo' src='" + teamConf.logo() + "'/></div>\n"
            "        <div class='team' id='" + sTeamId + "_name'>\n"
            "           <div class='team-name'>" + teamConf.name() + "</div>\n"
            "           <div class='team-ip'> id: " +sTeamId + ", ip: " + teamConf.ipAddress() + "</div>\n"
            "        </div>\n"
            "        <div class='score' id='" + sTeamId + "_score'>0</div>\n";

        for (unsigned int iservice = 0; iservice < m_pConfig->servicesConf().size(); iservice++) {
            Service serviceConf = m_pConfig->servicesConf()[iservice];
             sContent += "<div class='service'>"
                    "<div class='service-status down' id='" + sTeamId +  "_" + serviceConf.id() + "'> "
                    "   <div class='service-att-def' id='" + sTeamId +  "_" + serviceConf.id() + "_ad'>0 / 0</div>"
                    "   <div class='service-sla' id='" + sTeamId +  "_" + serviceConf.id() + "_sla'>100.0%</div>"
                    "</div>"
                "</div>\n";
        }

        sContent += "    </div>\n";
    }
    sContent += "</div>";

    // replace meta content table
    {
        std::string sContentDefine = "{{content}}";
        std::size_t index = m_sIndexHtml.find(sContentDefine, 0);
        if (index != std::string::npos) {
            m_sIndexHtml.replace(index, sContentDefine.length(), sContent);
        }
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
        || path == "/flag"
        || path == "/";
}

// ----------------------------------------------------------------------

bool HttpHandlerApiV1::handle(const std::string &sWorkerId, LightHttpRequest *pRequest){
    std::string _tag = TAG + "-" + sWorkerId;
    if(pRequest->requestPath() == "/api/v1/game") {
        pRequest->response(
            LightHttpResponse::RESP_OK, 
            "application/json", 
            m_jsonGame.dump());
        return true;
    } else if (pRequest->requestPath() == "/") {
        pRequest->response(LightHttpResponse::RESP_OK, "text/html", m_sIndexHtml);
        return true;
    } else if(pRequest->requestPath() == "/api/v1/scoreboard") {

        pRequest->response(
            LightHttpResponse::RESP_OK, 
            "application/json", 
            m_pConfig->scoreboard()->toJson().dump());
        return true;
    } else if(pRequest->requestPath() == "/api/v1/teams") {
        pRequest->response(
            LightHttpResponse::RESP_OK, 
            "application/json", 
            m_jsonTeams.dump());
        return true;
    } else if(pRequest->requestPath() == "/api/v1/services") {
        pRequest->response(
            LightHttpResponse::RESP_OK, 
            "application/json", 
            m_jsonServices.dump());
        return true;
        
    } else if(pRequest->requestPath() == "/flag") {
        
        auto now = std::chrono::system_clock::now().time_since_epoch();
        int nCurrentTimeSec = std::chrono::duration_cast<std::chrono::seconds>(now).count();

        if (nCurrentTimeSec < m_pConfig->gameStartUTCInSec()) {
            pRequest->response(
                LightHttpResponse::RESP_BAD_REQUEST, 
                "text/html", 
                "Error(-8): Game not started yet");
            Log::warn(TAG, "Error(-8): Game not started yet");
            return true;
        }

        if (nCurrentTimeSec > m_pConfig->gameEndUTCInSec()) {
            pRequest->response(
                LightHttpResponse::RESP_BAD_REQUEST, 
                "text/html", 
                "Error(-9): Game already ended");
            Log::warn(TAG, "Error(-9): Game already ended");
            return true;
        }

        std::map<std::string,std::string>::iterator itTeamID;
        itTeamID = pRequest->requestQueryParams().find("teamid");
        if (itTeamID == pRequest->requestQueryParams().end()) {
            pRequest->response(
                LightHttpResponse::RESP_BAD_REQUEST, 
                "text/html", 
                "Error(-10): Not found get-parameter 'teamid'");
            Log::warn(TAG, "Error(-10): Not found get-parameter 'teamid'");
            return true;
        }

        std::map<std::string,std::string>::iterator itFlag;
        itFlag = pRequest->requestQueryParams().find("flag");
        if (itFlag == pRequest->requestQueryParams().end()) {
            pRequest->response(
                LightHttpResponse::RESP_BAD_REQUEST, 
                "text/html", 
                "Error(-11): Not found get-parameter 'flag'");
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
            pRequest->response(
                LightHttpResponse::RESP_BAD_REQUEST, 
                "text/html", 
                "Error(-130): this is team not found");
            Log::warn(TAG, "Error(-130): this is team not found");
            return true;
        }

        const std::regex reFlagFormat("[a-f0-9]{8,8}-[a-f0-9]{4,4}-[a-f0-9]{4,4}-[a-f0-9]{4,4}-[a-f0-9]{12,12}");
        std::string sFlag = itFlag->second;
        std::transform(sFlag.begin(), sFlag.end(), sFlag.begin(), ::tolower);
        if (!std::regex_match(sFlag, reFlagFormat)) {
            pRequest->response(
                LightHttpResponse::RESP_BAD_REQUEST, 
                "text/html", 
                "Error(-140): flag has wrong format");
            Log::warn(TAG, "Error(-140): flag has wrong format");
            return true;
        }
        // TODO insert to flags_recieved

        Flag flag;
        if (!m_pConfig->storage()->findFlagByValue(sFlag, flag)) {
            pRequest->response(
                LightHttpResponse::RESP_FORBIDDEN,
                "text/html", 
                "Error(-150): flag is too old or flag never existed or flag alredy stole");
            Log::err(TAG, "Error(-150): Recieved flag {" + sFlag + "} from {" + sTeamId + "}");
            return true;
        }
        long nCurrentTimeMSec = (long)nCurrentTimeSec;
        nCurrentTimeMSec = nCurrentTimeMSec*1000;

        if (flag.timeEnd() < nCurrentTimeMSec) {
            pRequest->response(
                LightHttpResponse::RESP_FORBIDDEN,
                "text/html", 
                "Error(-151): flag is too old");
            Log::err(TAG, "Error(-151): Recieved flag {" + sFlag + "} from {" + sTeamId + "}");
            return true;
        }

        if (flag.teamStole() == sTeamId) {
            pRequest->response(
                LightHttpResponse::RESP_FORBIDDEN,
                "text/html", 
                "Error(-160): flag already stole by your team");
            Log::err(TAG, "Error(-160): Recieved flag {" + sFlag + "} from {" + sTeamId + "}");
            return true;
        }

        if (flag.teamStole() != "") {
            pRequest->response(
                LightHttpResponse::RESP_FORBIDDEN,
                "text/html", 
                "Error(-170): flag already stoled by '" + flag.teamStole() + "'");
            Log::err(TAG, "Error(-170): Recieved flag {" + sFlag + "} from {" + sTeamId + "}");
            return true;
        }

        if (flag.teamId() == sTeamId) {
            pRequest->response(
                LightHttpResponse::RESP_FORBIDDEN,
                "text/html", 
                "Error(-180): this is your flag");
            Log::err(TAG, "Error(-180): Recieved flag {" + sFlag + "} from {" + sTeamId + "}");
            return true;
        }
        
        std::string sServiceStatus = m_pConfig->scoreboard()->serviceStatus(sTeamId, flag.serviceId());

        // std::cout << "sServiceStatus: " << sServiceStatus << "\n";

        if (sServiceStatus != ModelServiceStatus::SERVICE_UP) {
            pRequest->response(
                LightHttpResponse::RESP_FORBIDDEN,
                "text/html", 
                "Error(-190): Your same service is dead");
            Log::err(TAG, "Error(-190): Recieved flag {" + sFlag + "} from {" + sTeamId + "}");
            return true;
        }

        if (!m_pConfig->storage()->updateTeamStole(flag.value(), sTeamId)){
            pRequest->response(
                LightHttpResponse::RESP_FORBIDDEN,
                "text/html", 
                "Error(-300): You are late");
            Log::err(TAG, "Error(-300): Recieved flag {" + sFlag + "} from {" + sTeamId + "}");
            return true;
        }

        // TODO light update scoreboard
        m_pConfig->scoreboard()->incrementAttackScore(sTeamId, flag.serviceId());

        pRequest->response(
            LightHttpResponse::RESP_OK, 
            "text/html", 
            "Accepted");
        Log::ok(TAG, "Accepted: Recieved flag {" + sFlag + "} from {" + sTeamId + "}");
        return true;
    }
    return false;
}

// ----------------------------------------------------------------------
