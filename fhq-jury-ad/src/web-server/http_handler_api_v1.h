#ifndef HTTP_HANDLER_API_V1_H
#define HTTP_HANDLER_API_V1_H

#include <light_http_handler_base.h>
#include <config.h>
#include <json.hpp>

class HttpHandlerApiV1 : LightHttpHandlerBase {
    public:
        HttpHandlerApiV1(Config *pConfig);
        virtual bool canHandle(const std::string &sWorkerId, LightHttpRequest *pRequest);
        virtual bool handle(const std::string &sWorkerId, LightHttpRequest *pRequest);

    private:
        std::string TAG;
        void prepareIndexHtml();

        Config *m_pConfig;
        std::string m_sIndexHtml;

        nlohmann::json m_jsonGame;
        nlohmann::json m_jsonTeams; // prepare data for list of teams
        nlohmann::json m_jsonServices; // prepare data for list of teams

};

#endif // HTTP_HANDLER_API_V1_H
