#ifndef HTTP_HANDLER_API_V1_H
#define HTTP_HANDLER_API_V1_H

#include <wsjcpp_light_web_server.h>
#include <employ_config.h>
#include <json.hpp>

class HttpHandlerApiV1 : public WsjcppLightWebHttpHandlerBase {
    public:
        HttpHandlerApiV1();
        virtual bool canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest);
        virtual bool handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest);

    private:
        std::string TAG;

        EmployConfig *m_pConfig;
        std::string m_sIndexHtml;

        nlohmann::json m_jsonGame;
        nlohmann::json m_jsonTeams; // prepare data for list of teams
        nlohmann::json m_jsonServices; // prepare data for list of teams

};

#endif // HTTP_HANDLER_API_V1_H
