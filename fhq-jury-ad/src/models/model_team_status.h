#ifndef MODEL_TEAM_STATUS_H
#define MODEL_TEAM_STATUS_H

#include <string>
#include <map>
#include <mutex>
#include <vector>
#include <model_service_conf.h>
#include <model_service_status.h>

class ModelTeamStatus {
    public:
        ModelTeamStatus(const std::string &sTeamId, const std::vector<ModelServiceConf> &vServicesConf);
        const std::string &teamId();

        void setPlace(int nPlace);
        int place();

        void setScore(double nScore);
        double score();
        
        void setServiceStatus(const std::string &sServiceId, std::string sStatus);
        std::string serviceStatus(const std::string &sServiceId);

        void setServiceScore(const std::string &sServiceId, int nNewDefence, int nNewAttack, double nNewSLA);

        std::string servicesToString();

    private:
        std::mutex m_mutex;
        std::string m_sTeamId;
        int m_nPlace;
        double m_nScore;
        std::map<std::string, ModelServiceStatus *> m_mapServicesStatus;
};

#endif // MODEL_TEAM_STATUS_H
