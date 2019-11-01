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
        TeamStatusRow(const std::string &sTeamId, const std::vector<Service> &vServicesConf, int nGameStartInSec, int nGameEndInSec);
        const std::string &teamId();

        void setPlace(int nPlace);
        int place();

        void setPoints(int nPoints);
        int getPoints();

        void setServiceStatus(const std::string &sServiceId, std::string sStatus);
        std::string serviceStatus(const std::string &sServiceId);

        void setTries(int nScore);
        int tries();

        std::string servicesToString();

        void incrementDefence(const std::string &sServiceId, int nFlagPoints);
        int getDefenceFlags(const std::string &sServiceId);
        int getDefencePoints(const std::string &sServiceId);
        void setServiceDefenceFlagsAndPoints(const std::string &sServiceId, int nDefenceFlags, int nDefencePoints);

        void incrementAttack(const std::string &sServiceId, int nFlagPoints);
        void setServiceAttackFlagsAndPoints(const std::string &sServiceId, int nAttackFlags, int nAttackPoints);
        int getAttackFlags(const std::string &sServiceId);
        int getAttackPoints(const std::string &sServiceId);

        void setServiceFlagsPutted(const std::string &sServiceId, int nFlagsPutted);
        double serviceUptime(const std::string &sServiceId);
        void updateScore(const std::string &sServiceId);

    private:
        std::mutex m_mutex;
        std::string m_sTeamId;
        int m_nPlace;
        int m_nPoints;
        int m_nTries;
        std::map<std::string, ServiceStatusCell *> m_mapServicesStatus;
};

#endif // TEAM_STATUS_ROW_H
