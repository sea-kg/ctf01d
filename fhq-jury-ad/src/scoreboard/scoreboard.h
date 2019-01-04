#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <team_status_row.h>
#include <team.h>
#include <service.h>
#include <flag.h>
#include <string>
#include <map>
#include <vector>
#include <mutex>
#include <json.hpp>
#include <storage.h>

class Scoreboard {
    public:

        Scoreboard(bool bRandom,
            int nGameStartInSec,
            int nGameEndInSec,
            int nFlagTimeLiveInSec,
            const std::vector<Team> &vTeamsConf, 
            const std::vector<Service> &vServicesConf,
            Storage *pStorage
        );

        void setServiceStatus(const std::string &sTeamId, const std::string &sServiceId, const std::string &sStatus);
        void setServiceScore(const std::string &sTeamId, const std::string &sServiceId, int nDefence, int nAttack, double nSLA);
        void incrementTries(const std::string &sTeamId);
        void initStateFromStorage();

        void incrementAttackScore(const std::string &sTeamId, const std::string &sServiceId);
        void incrementDefenceScore(const std::string &sTeamId, const std::string &sServiceId);
        void incrementFlagsPutted(const std::string &sTeamId, const std::string &sServiceId);
        std::string serviceStatus(const std::string &sTeamId, const std::string &sServiceId);

        void addFlagLive(const Flag &flag);
        std::vector<Flag> outdatedFlagsLive(const std::string &sTeamId, const std::string &sServiceId);
        bool findFlagLive(const std::string &sFlagValue, Flag &flag);
        void removeFlagLive(const Flag &flag);

        std::string toString();
        const nlohmann::json &toJson();

    private:
        std::string TAG;
        Storage *m_pStorage;
        
        void sortPlaces();

        std::mutex m_mutexJson;
        int m_nGameStartInSec;
        int m_nGameEndInSec;
        int m_nFlagTimeLiveInSec;
        std::vector<Service> m_vServices;

        std::string randomServiceStatus();
        bool m_bRandom;
        std::map<std::string, TeamStatusRow *> m_mapTeamsStatuses;
        nlohmann::json m_jsonScoreboard; // prepare data for scoreboard
        
        // flags live for fast check
        std::mutex m_mutexFlagsLive;
        std::map<std::string, Flag> m_mapFlagsLive;
};

#endif // SCOREBOARD_H
