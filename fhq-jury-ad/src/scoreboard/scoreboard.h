#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <model_team_status.h>
#include <team.h>
#include <service.h>
#include <flag.h>
#include <string>
#include <map>
#include <vector>
#include <mutex>
#include <json.hpp>

class Scoreboard {
    public:

        Scoreboard(bool bRandom, int nGameStart, int nGameEnd, const std::vector<Team> &vTeamsConf, const std::vector<Service> &vServicesConf);

        void setServiceStatus(const std::string &sTeamId, const std::string &sServiceId, const std::string &sStatus);
        void setServiceScore(const std::string &sTeamId, const std::string &sServiceId, int nDefence, int nAttack, double nSLA);

        void incrementAttackScore(const std::string &sTeamId, const std::string &sServiceId);
        void incrementDefenceScore(const std::string &sTeamId, const std::string &sServiceId);
        std::string serviceStatus(const std::string &sTeamId, const std::string &sServiceId);

        double calculateSLA(int flags_success, const Service &serviceConf);
        void addFlagLive(const Flag &flag);
        void removeFlagLive(const Flag &flag);

        std::string toString();
        const nlohmann::json &toJson();

    private:
        std::string TAG;
        std::mutex m_mutexJson;
        int m_nGameStart;
        int m_nGameEnd;

        std::string randomServiceStatus();
        bool m_bRandom;
        std::map<std::string, ModelTeamStatus *> m_mapTeamsStatuses;
        nlohmann::json m_jsonScoreboard; // prepare data for scoreboard
        // flags live for fast check
        std::mutex m_mutexFlagsLive;
        std::map<std::string, Flag> m_mapFlagsLive;
};

#endif // SCOREBOARD_H
