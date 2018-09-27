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
        ModelTeamStatus(int nTeamNum, const std::vector<ModelServiceConf> &vServicesConf);
        int teamNum();

        void setPlace(int nPlace);
        int place();

        void setScore(double nScore);
        double score();
        
        void setServiceStatus(int nServiceNum, std::string sStatus);
        std::string serviceStatus(int nServiceNum);

        void setServiceScore(int nServiceNum, int nNewDefence, int nNewAttack, double nNewSLA);

        std::string servicesToString();

    private:
        std::mutex m_mutex;
        int m_nTeamNum;
        int m_nPlace;
        double m_nScore;
        std::map<int, ModelServiceStatus *> m_mapServicesStatus;
};

#endif // MODEL_TEAM_STATUS_H
