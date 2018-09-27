#ifndef MODEL_SCOREBOARD_H
#define MODEL_SCOREBOARD_H

#include <model_team_status.h>
#include <model_team_conf.h>
#include <model_service_conf.h>
#include <string>
#include <map>
#include <vector>
#include <mutex>
#include <json.hpp>

class ModelScoreboard {
    public:

        ModelScoreboard(bool bRandom, int nGameStart, int nGameEnd, const std::vector<ModelTeamConf> &vTeamsConf, const std::vector<ModelServiceConf> &vServicesConf);

        void setServiceStatus(int nTeamNum, int nServiceNum, const std::string &sStatus);
        void setServiceScore(int nTeamNum, int nServiceNum, int nDefence, int nAttack, double nSLA);

        void incrementAttackScore(int nTeamNum, int nServiceNum);
        void incrementDefenceScore(int nTeamNum, int nServiceNum);
        std::string serviceStatus(int nTeamNum, int nServiceNum);

        double calculateSLA(int flags_success, const ModelServiceConf &serviceConf);

        std::string toString();
        const nlohmann::json &toJson();

    private:
        std::string TAG;
        std::mutex m_mutexJson;
        int m_nGameStart;
        int m_nGameEnd;

        std::string randomServiceStatus();
        bool m_bRandom;
        std::map<int, ModelTeamStatus *> m_mapTeamsStatuses;
        nlohmann::json m_jsonScoreboard; // prepare data for scoreboard
};

#endif // MODEL_SCOREBOARD_H
