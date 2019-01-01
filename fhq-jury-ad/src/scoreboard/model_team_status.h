#ifndef MODEL_TEAM_STATUS_H
#define MODEL_TEAM_STATUS_H

#include <string>
#include <map>
#include <mutex>
#include <vector>
#include <service.h>
#include <model_service_status.h>

class ModelTeamStatus {
    public:
        ModelTeamStatus(const std::string &sTeamId, const std::vector<Service> &vServicesConf);
        const std::string &teamId();

        void setPlace(int nPlace);
        int place();

        void setScore(double nScore);
        double score();
        
        void setServiceStatus(const std::string &sServiceId, std::string sStatus);
        std::string serviceStatus(const std::string &sServiceId);

        void setServiceScore(const std::string &sServiceId, int nNewDefence, int nNewAttack, double nNewSLA);

        void setTries(int nScore);
        int tries();

        std::string servicesToString();

    private:
        std::mutex m_mutex;
        std::string m_sTeamId;
        int m_nPlace;
        double m_nScore;
        int m_nTries;
        std::map<std::string, ModelServiceStatus *> m_mapServicesStatus;
};

#endif // MODEL_TEAM_STATUS_H
