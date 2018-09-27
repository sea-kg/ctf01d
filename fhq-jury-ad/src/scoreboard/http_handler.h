#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include <utils_light_http_server.h>
#include <config.h>
#include <json.hpp>

class HttpHandler : ILightHttpHandler {
    public:
        HttpHandler(JuryConfiguration *pConfig);
        virtual bool handle(ILightHttpRequest *pRequest);

    private:
        std::string formatTimeUTC(int nTime);
        void prepareIndexHtml();

        JuryConfiguration *m_pConfig;
        std::string m_sIndexHtml;
        nlohmann::json m_jsonTeams; // prepare data for list of teams
        nlohmann::json m_jsonServices; // prepare data for list of teams
};

#endif // HTTP_HANDLER_H
