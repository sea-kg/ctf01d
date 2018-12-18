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

        void setServiceStatus(const std::string &sTeamId, const std::string &sServiceId, const std::string &sStatus);
        void setServiceScore(const std::string &sTeamId, const std::string &sServiceId, int nDefence, int nAttack, double nSLA);

        void incrementAttackScore(const std::string &sTeamId, const std::string &sServiceId);
        void incrementDefenceScore(const std::string &sTeamId, const std::string &sServiceId);
        std::string serviceStatus(const std::string &sTeamId, const std::string &sServiceId);

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
        std::map<std::string, ModelTeamStatus *> m_mapTeamsStatuses;
        nlohmann::json m_jsonScoreboard; // prepare data for scoreboard
};

#endif // MODEL_SCOREBOARD_H
