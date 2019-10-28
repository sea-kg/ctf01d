#ifndef READ_TEAMS_CONF_H
#define READ_TEAMS_CONF_H

#include <string>
#include <vector>
#include <team.h>

class ReadTeamsConf {
	public: 
        ReadTeamsConf(const std::string &sWorkspaceDir);
		// services configuration
		bool read(std::vector<Team> &vTeams);
    private:
        std::string TAG;
        std::string m_sWorkspaceDir;
};

#endif // READ_TEAMS_CONF_H