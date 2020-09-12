#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <employ_flags.h>
#include <employ_scoreboard.h>
#include <string>
#include <json.hpp>
#include <storage.h>


class Scoreboard {
    public:

        Scoreboard(
            bool bRandom,
            int nGameStartInSec,
            int nGameEndInSec,
            int nGameCoffeeBreakStartInSec,
            int nGameCoffeeBreakEndInSec,
            int nFlagTimeLiveInSec,
            Storage *pStorage
        );

        void setServiceStatus(const std::string &sTeamId, const std::string &sServiceId, const std::string &sStatus);
        void incrementTries(const std::string &sTeamId);
        void initStateFromStorage();

        int incrementAttackScore(const Ctf01dFlag &flag, const std::string &sTeamId);
        void incrementDefenceScore(const Ctf01dFlag &flag);
        void incrementFlagsPuttedAndServiceUp(const Ctf01dFlag &flag);
        void insertFlagPutFail(const Ctf01dFlag &flag, const std::string &sServiceStatus, const std::string &sDescrStatus);
        void updateScore(const std::string &sTeamId, const std::string &sServiceId);
        std::string serviceStatus(const std::string &sTeamId, const std::string &sServiceId);

        std::vector<Ctf01dFlag> outdatedFlagsLive(const std::string &sTeamId, const std::string &sServiceId);
        bool findFlagLive(const std::string &sFlagValue, Ctf01dFlag &flag);
        void removeFlagLive(const Ctf01dFlag &flag);

        std::string toString();
        const nlohmann::json &toJson();

    private:
        std::string TAG;
        Storage *m_pStorage;
        EmployFlags *m_pEmployFlags;
        int m_nGameStartInSec;
        int m_nGameEndInSec;
        int m_nGameCoffeeBreakStartInSec;
        int m_nGameCoffeeBreakEndInSec;
        int m_nFlagTimeLiveInSec;

        void sortPlaces(); // TODO merge this function with update costs
        void updateCosts();

        std::map<std::string, Ctf01dServiceCostsAndStatistics *> m_mapServiceCostsAndStatistics;
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
        std::map<std::string, Ctf01dFlag> m_mapFlagsLive; // Must be in samewhere in storage
};

#endif // SCOREBOARD_H
