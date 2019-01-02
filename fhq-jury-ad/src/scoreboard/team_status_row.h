#ifndef TEAM_STATUS_ROW_H
#define TEAM_STATUS_ROW_H

#include <string>
#include <map>
#include <mutex>
#include <vector>
#include <service.h>
#include <service_status_cell.h>

class TeamStatusRow {
    public:
        TeamStatusRow(const std::string &sTeamId, const std::vector<Service> &vServicesConf);
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
        std::map<std::string, ServiceStatusCell *> m_mapServicesStatus;
};

#endif // TEAM_STATUS_ROW_H
