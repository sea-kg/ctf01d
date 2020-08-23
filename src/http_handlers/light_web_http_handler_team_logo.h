#ifndef LIGHT_WEB_HTTP_HANDLER_TEAM_LOGO_H
#define LIGHT_WEB_HTTP_HANDLER_TEAM_LOGO_H

#include <wsjcpp_light_web_server.h>

class LightWebHttpHandlerTeamLogo : public WsjcppLightWebHttpHandlerBase {
    public:
        LightWebHttpHandlerTeamLogo();
        virtual bool canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest);
        virtual bool handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest);

    private:
        std::string TAG;
        std::string m_sPrefix;
        int m_nPrefixLength;
};

#endif // LIGHT_WEB_HTTP_HANDLER_TEAM_LOGO_H
