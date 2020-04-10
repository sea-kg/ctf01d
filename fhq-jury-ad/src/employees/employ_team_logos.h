#ifndef EMPLOY_TEAM_LOGOS_H
#define EMPLOY_TEAM_LOGOS_H

#include <wsjcpp_employees.h>

struct TeamLogo {
    std::string sTeamId;
    char *pBuffer;
    int nBufferSize;
    std::string sFilename;
    std::string sFilepath;
};

class EmployTeamLogos : public WsjcppEmployBase {
    public:
        EmployTeamLogos();
        static std::string name() { return "EmployTeamLogos"; }
        virtual bool init() override;
        virtual bool deinit() override;
        bool loadTeamLogo(const std::string &sTeamId, const std::string &sFilepath);
        TeamLogo *findTeamLogo(const std::string &sTeamId);
    private:
        std::string TAG;
        std::map<std::string, TeamLogo *> m_mapTeamLogos;
};

#endif // EMPLOY_TEAM_LOGOS_H
