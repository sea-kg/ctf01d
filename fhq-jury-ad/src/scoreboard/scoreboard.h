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
#include <service_costs_and_statistics.h>

class Scoreboard {
    public:

        Scoreboard(
            bool bRandom,
            int nGameStartInSec,
            int nGameEndInSec,
            int nGameCoffeeBreakStartInSec,
            int nGameCoffeeBreakEndInSec,
            int nFlagTimeLiveInSec,
            int nBacisCostsStolenFlagInPoints,
            const std::vector<Team> &vTeamsConf, 
            const std::vector<Service> &vServicesConf,
            Storage *pStorage
        );

        void setServiceStatus(const std::string &sTeamId, const std::string &sServiceId, const std::string &sStatus);
        void setServiceScore(const std::string &sTeamId, const std::string &sServiceId, int nDefence, int nAttack, double nSLA);
        void incrementTries(const std::string &sTeamId);
        void initStateFromStorage();

        void incrementAttackScore(const std::string &sTeamId, const std::string &sServiceId);
        void incrementDefenceScore(const Flag &flag);
        void incrementFlagsPuttedAndServiceUp(const Flag &flag);
        void insertFlagPutFail(const Flag &flag, const std::string &sServiceStatus, const std::string &sDescrStatus);
        void updateScore(const std::string &sTeamId, const std::string &sServiceId);
        std::string serviceStatus(const std::string &sTeamId, const std::string &sServiceId);

        std::vector<Flag> outdatedFlagsLive(const std::string &sTeamId, const std::string &sServiceId);
        bool findFlagLive(const std::string &sFlagValue, Flag &flag);
        void removeFlagLive(const Flag &flag);

        std::string toString();
        const nlohmann::json &toJson();

    private:
        std::string TAG;
        Storage *m_pStorage;
        std::vector<Service> m_vServices;
        std::vector<Team> m_vTeams;
        int m_nBacisCostsStolenFlagInPoints;
        int m_nGameStartInSec;
        int m_nGameEndInSec;
        int m_nGameCoffeeBreakStartInSec;
        int m_nGameCoffeeBreakEndInSec;
        int m_nFlagTimeLiveInSec;

        void sortPlaces(); // TODO merge this function with update costs
        void updateCosts();

        std::map<std::string, ServiceCostsAndStatistics *> m_mapServiceCostsAndStatistics;
        int m_nAllStolenFlags;
        int m_nAllDefenceFlags;

        std::string randomServiceStatus();
        bool m_bRandom;
        std::map<std::string, TeamStatusRow *> m_mapTeamsStatuses;

        std::mutex m_mutexJson;
        nlohmann::json m_jsonScoreboard; // prepare data for scoreboard
        void initJsonScoreboard();
        void updateJsonScoreboard();
        // nlohmann::json m_jsonGF; // prepare data for flags costs
        
        // flags live for fast check
        std::mutex m_mutexFlagsLive;
        std::map<std::string, Flag> m_mapFlagsLive;
};

#endif // SCOREBOARD_H
