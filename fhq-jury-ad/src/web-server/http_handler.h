#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include <light_http_server.h>
#include <config.h>
#include <json.hpp>

class HttpHandler : ILightHttpHandler {
    public:
        HttpHandler(Config *pConfig);
        virtual bool handle(ILightHttpRequest *pRequest);

    private:
        std::string TAG;
        void prepareIndexHtml();
        void string_trim(std::string &sLine);

        Config *m_pConfig;
        std::string m_sIndexHtml;
        nlohmann::json m_jsonTeams; // prepare data for list of teams
        nlohmann::json m_jsonServices; // prepare data for list of teams
};

#endif // HTTP_HANDLER_H
