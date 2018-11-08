#include "http_handler.h"
#include <fstream>
#include <string>
#include <istorage.h>
#include <regex>
#include <algorithm>
#include <time.h>
#include <utils_logger.h>

HttpHandler::HttpHandler(JuryConfiguration *pConfig){
    m_pConfig = pConfig;
    TAG = "HttpHandler";
    prepareIndexHtml();
}

// ----------------------------------------------------------------------

void HttpHandler::prepareIndexHtml(){
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
        ModelServiceConf serviceConf = m_pConfig->servicesConf()[iservice];
        sContent += "<div class='service'>" + serviceConf.name() + "<br><small>(service)</small></div>\n";
        
        nlohmann::json serviceInfo;
        serviceInfo["num"] = serviceConf.num();
        serviceInfo["name"] = serviceConf.name();
        m_jsonServices[serviceConf.id()] = serviceInfo;
    }

    sContent += "  </div>\n";

    for (unsigned int iteam = 0; iteam < m_pConfig->teamsConf().size(); iteam++) {
        ModelTeamConf teamConf = m_pConfig->teamsConf()[iteam];
        std::string sTeamId = teamConf.id();
            
        nlohmann::json teamInfo;
        teamInfo["num"] = teamConf.num();
        teamInfo["name"] = teamConf.name();
        teamInfo["ip_address"] = teamConf.ipAddress();
        m_jsonTeams[sTeamId] = teamInfo;

        sContent += 
            "    <div class='tm' id='" + sTeamId + "'>\n"
            "        <div class='place' id='" + sTeamId + "_place' >" + std::to_string(iteam + 1) + "</div>\n"
            "        <div class='team-logo' id='" + sTeamId + "_logo' ><img class='team-logo' src='" + teamConf.logo() + "'/></div>\n"
            "        <div class='team' id='" + sTeamId + "_name'>\n"
            "           <div class='team-name'>" + teamConf.name() + "</div>\n"
            "           <div class='team-ip'> id: " + std::to_string(teamConf.num()) + ", ip: " + teamConf.ipAddress() + "</div>\n"
            "        </div>\n"
            "        <div class='score' id='" + sTeamId + "_score'>0</div>\n";

        for (unsigned int iservice = 0; iservice < m_pConfig->servicesConf().size(); iservice++) {
            ModelServiceConf serviceConf = m_pConfig->servicesConf()[iservice];
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

    // replace meta game name
    {
        std::string sGameNameDefine = "{{game_name}}";
        size_t index = m_sIndexHtml.find(sGameNameDefine, 0);
        if (index != std::string::npos) {
            std::string sGameName = m_pConfig->gameName();
            if (m_pConfig->scoreboardRandom()) {
                sGameName += " (RANDOM)";
            }
            m_sIndexHtml = m_sIndexHtml.replace(index, sGameNameDefine.length(), sGameName);
        }
    }

    // replace meta game time range
    {
        std::string sGameTimeRangeDefine = "{{game_time_range}}";
        size_t index = m_sIndexHtml.find(sGameTimeRangeDefine, 0);
        if (index != std::string::npos) {
            std::string sGameTimeRange = ""
                + formatTimeUTC(m_pConfig->gameStartUTCInSec())
                + " (UTC) - "
                + formatTimeUTC(m_pConfig->gameEndUTCInSec())
                + " (UTC) ";
            m_sIndexHtml = m_sIndexHtml.replace(index, sGameTimeRangeDefine.length(), sGameTimeRange);
        }
    }

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

std::string HttpHandler::formatTimeUTC(int nTimeInSec) {
    // datetime
    std::time_t tm_ = long(nTimeInSec);
    // struct tm tstruct = *localtime(&tm_);
    struct tm tstruct = *gmtime ( &tm_ );

    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tstruct);
    return std::string(buf);
}

// ----------------------------------------------------------------------

bool HttpHandler::handle(ILightHttpRequest *pRequest){
    if (pRequest->requestPath() == "/") {
        pRequest->response(LightHttpRequest::RESP_OK, "text/html", m_sIndexHtml);
        return true;
    } else if(pRequest->requestPath() == "/api/v1/scoreboard") {

        pRequest->response(
            LightHttpRequest::RESP_OK, 
            "application/json", 
            m_pConfig->scoreboard()->toJson().dump());
        return true;
    } else if(pRequest->requestPath() == "/api/v1/teams") {
        pRequest->response(
            LightHttpRequest::RESP_OK, 
            "application/json", 
            m_jsonTeams.dump());
        return true;
    } else if(pRequest->requestPath() == "/api/v1/services") {
        pRequest->response(
            LightHttpRequest::RESP_OK, 
            "application/json", 
            m_jsonServices.dump());
        return true;
        
    } else if(pRequest->requestPath() == "/flag") {
        
        auto now = std::chrono::system_clock::now().time_since_epoch();
        int nCurrentTimeSec = std::chrono::duration_cast<std::chrono::seconds>(now).count();

        if (nCurrentTimeSec < m_pConfig->gameStartUTCInSec()) {
            pRequest->response(
                LightHttpRequest::RESP_BAD_REQUEST, 
                "text/html", 
                "Error(-8): Game not started yet");
            Log::warn(TAG, "Error(-8): Game not started yet");
            return true;
        }

        if (nCurrentTimeSec > m_pConfig->gameEndUTCInSec()) {
            pRequest->response(
                LightHttpRequest::RESP_BAD_REQUEST, 
                "text/html", 
                "Error(-9): Game already ended");
            Log::warn(TAG, "Error(-9): Game already ended");
            return true;
        }

        std::map<std::string,std::string>::iterator itTeamID;
        itTeamID = pRequest->requestQueryParams().find("teamid");
        if (itTeamID == pRequest->requestQueryParams().end()) {
            pRequest->response(
                LightHttpRequest::RESP_BAD_REQUEST, 
                "text/html", 
                "Error(-10): Not found get-parameter 'teamid'");
            Log::warn(TAG, "Error(-10): Not found get-parameter 'teamid'");
            return true;
        }

        std::map<std::string,std::string>::iterator itFlag;
        itFlag = pRequest->requestQueryParams().find("flag");
        if (itFlag == pRequest->requestQueryParams().end()) {
            pRequest->response(
                LightHttpRequest::RESP_BAD_REQUEST, 
                "text/html", 
                "Error(-11): Not found get-parameter 'flag'");
            Log::warn(TAG, "Error(-11): Not found get-parameter 'flag'");
            return true;
        }

        std::string sTeamID = itTeamID->second;
        int nTeamNum = atoi(sTeamID.c_str());
        if (nTeamNum == 0) {
            pRequest->response(
                LightHttpRequest::RESP_BAD_REQUEST, 
                "text/html", 
                "Error(-12): 'teamid' must be number");
            Log::warn(TAG, "Error(-12): 'teamid' must be number");
            return true;
        }

        bool bTeamFound = false;
        for (unsigned int iteam = 0; iteam < m_pConfig->teamsConf().size(); iteam++) {
            ModelTeamConf teamConf = m_pConfig->teamsConf()[iteam];
            if (teamConf.num() == nTeamNum) {
                bTeamFound = true;
            }
        }

        if (!bTeamFound) {
            pRequest->response(
                LightHttpRequest::RESP_BAD_REQUEST, 
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
                LightHttpRequest::RESP_BAD_REQUEST, 
                "text/html", 
                "Error(-140): flag has wrong format");
            Log::warn(TAG, "Error(-140): flag has wrong format");
            return true;
        }
        // TODO insert to flags_recieved

        ModelFlag flag;
        if (!m_pConfig->storage()->findFlagByValue(sFlag, flag)) {
            pRequest->response(
                LightHttpRequest::RESP_FORBIDDEN,
                "text/html", 
                "Error(-150): flag is too old or flag never existed or flag alredy stole");
            Log::err(TAG, "Error(-150): Recieved flag {" + sFlag + "} from {team" + std::to_string(nTeamNum) + "}");
            return true;
        }
        long nCurrentTimeMSec = (long)nCurrentTimeSec;
        nCurrentTimeMSec = nCurrentTimeMSec*1000;

        if (flag.timeEnd() < nCurrentTimeMSec) {
            pRequest->response(
                LightHttpRequest::RESP_FORBIDDEN,
                "text/html", 
                "Error(-151): flag is too old");
            Log::err(TAG, "Error(-151): Recieved flag {" + sFlag + "} from {team" + std::to_string(nTeamNum) + "}");
            return true;
        }

        if (flag.teamStole() == nTeamNum) {
            pRequest->response(
                LightHttpRequest::RESP_FORBIDDEN,
                "text/html", 
                "Error(-160): flag already stole by your team");
            Log::err(TAG, "Error(-160): Recieved flag {" + sFlag + "} from {team" + std::to_string(nTeamNum) + "}");
            return true;
        }

        if (flag.teamStole() != 0) {
            pRequest->response(
                LightHttpRequest::RESP_FORBIDDEN,
                "text/html", 
                "Error(-170): flag already stole");
            Log::err(TAG, "Error(-170): Recieved flag {" + sFlag + "} from {team" + std::to_string(nTeamNum) + "}");
            return true;
        }

        if (flag.teamNum() == nTeamNum) {
            pRequest->response(
                LightHttpRequest::RESP_FORBIDDEN,
                "text/html", 
                "Error(-180): this is your flag");
            Log::err(TAG, "Error(-180): Recieved flag {" + sFlag + "} from {team" + std::to_string(nTeamNum) + "}");
            return true;
        }
        
        std::string sServiceStatus = m_pConfig->scoreboard()->serviceStatus(nTeamNum, flag.serviceNum());

        // std::cout << "sServiceStatus: " << sServiceStatus << "\n";

        if (sServiceStatus != ModelServiceStatus::SERVICE_UP) {
            pRequest->response(
                LightHttpRequest::RESP_FORBIDDEN,
                "text/html", 
                "Error(-190): Your same service is dead");
            Log::err(TAG, "Error(-190): Recieved flag {" + sFlag + "} from {team" + std::to_string(nTeamNum) + "}");
            return true;
        }

        if (!m_pConfig->storage()->updateTeamStole(flag.value(), nTeamNum)){
            pRequest->response(
                LightHttpRequest::RESP_FORBIDDEN,
                "text/html", 
                "Error(-300): You are late");
            Log::err(TAG, "Error(-300): Recieved flag {" + sFlag + "} from {team" + std::to_string(nTeamNum) + "}");
            return true;
        }

        // TODO light update scoreboard
        m_pConfig->scoreboard()->incrementAttackScore(nTeamNum, flag.serviceNum());

        pRequest->response(
            LightHttpRequest::RESP_OK, 
            "text/html", 
            "Accepted");
        Log::ok(TAG, "Accepted: Recieved flag {" + sFlag + "} from {team" + std::to_string(nTeamNum) + "}");
        return true;
    }
    return false;
}
