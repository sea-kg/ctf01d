#ifndef EMPLOY_SCOREBOARD_H
#define EMPLOY_SCOREBOARD_H

#include <wsjcpp_employees.h>
#include <json.hpp>

class Ctf01dServiceCostsAndStatistics {
    public:
        Ctf01dServiceCostsAndStatistics(
            const std::string &sServiceId,
            int nBasicCostOfStolenFlag,
            int nServicesSize,
            int nTeamsSize
        );
        int getAllStolenFlagsForService();
        void doIncrementStolenFlagsForService(int nAllStolenFlags);
        double updateProportionalStolenFlagsForService(int nStolenFlags, int m_nAllStolenFlags);
        double updateProportionalStolenFlagsForService(int nAllStolenFlags);
        double updateCostStolenFlag(double nSumOfReverseProportionalStolenFlags);
        double getCostStolenFlag();

        int getAllDefenceFlagsForService();
        void doIncrementDefenceFlagsForService(int nAllDefenceFlags);
        
        double updateProportionalDefenceFlagsForService(int nDefenceFlags, int nAllDefenceFlags);
        double updateProportionalDefenceFlagsForService(int nAllDefenceFlags);
        double updateCostDefenceFlag(double nSumOfReverseProportionalDefenceFlags);
        double getCostDefenceFlag();

        std::string getFirstBloodTeamId();
        void setFirstBloodTeamId(const std::string &sFirstBlood);
        void updateJsonCosts(nlohmann::json &jsonCosts);

    private:
        std::string TAG;
        std::string m_sServiceId;
        std::string m_sFirstBloodTeamId;
        double m_nBasicCostOfStolenFlag;
        double m_nBasicCostOfDefenceFlag;
        int m_nServicesSize;
        int m_nTeamsSize;

        int m_nAllStolenFlagsForService;
        double m_nReverseProportionalStolenFlags;
        double m_nCostStolenFlag;

        int m_nAllDefenceFlagsForService;
        double m_nReverseProportionalDefenceFlags;
        double m_nCostDefenceFlag;
};

// ---------------------------------------------------------------------

class ServiceStatusCell {
    public:
        // enum for service status
        static std::string SERVICE_UP;
        static std::string SERVICE_DOWN;
        static std::string SERVICE_MUMBLE;
        static std::string SERVICE_CORRUPT;
        static std::string SERVICE_SHIT;
        static std::string SERVICE_WAIT;
        static std::string SERVICE_COFFEEBREAK;

        ServiceStatusCell(const std::string &sServiceId);
        const std::string &serviceId();

        void setDefenceFlags(int nDefenceFlags);
        int getDefenceFlags();
        void incrementDefenceFlags();
        
        void setDefencePoints(int nDefencePoints);
        int getDefencePoints();
        void addDefencePoints(int nDefencePoints);

        void setAttackFlags(int nAttackFlags);
        int getAttackFlags();
        void incrementAttackFlags();

        void setAttackPoints(int nAttackPoints);
        int getAttackPoints();
        void addAttackPoints(int nAttackPoints);

        void setFlagsPutted(int nFlagsPutted);
        int flagsPutted();

        int getUptimeInSec();
        void setUpPointTime(int nUpPointTimeInSec);

        void setStatus(const std::string &sStatus);
        std::string status();

    private:
        std::string TAG;
        std::mutex m_mutexServiceStatus;
        std::string m_sServiceId;
        std::string m_sStatus; // may be char[10] ?
        int m_nDefenceFlags;
        int m_nAttackFlags;
        int m_nAttackPoints;
        int m_nDefencePoints;
        int m_nUpPointTimeInSec;

        // for SLA / uptime
        int m_nFlagsPutted; // TODO remove
};

// ---------------------------------------------------------------------

class TeamStatusRow {
    public:
        TeamStatusRow(const std::string &sTeamId, int nGameStartInSec, int nGameEndInSec);
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
        void updateScore(const std::string &sServiceId);

    private:
        std::mutex m_mutex;
        std::string m_sTeamId;
        int m_nPlace;
        int m_nPoints;
        int m_nTries;
        std::map<std::string, ServiceStatusCell *> m_mapServicesStatus;
};

// ---------------------------------------------------------------------

class EmployScoreboard : public WsjcppEmployBase {
    public:
        EmployScoreboard();
        static std::string name() { return "EmployScoreboard"; }
        virtual bool init() override;
        virtual bool deinit() override;


    private:
        std::string TAG;

        std::map<std::string, Ctf01dServiceCostsAndStatistics *> m_mapServiceCostsAndStatistics;
        int m_nAllStolenFlags;
        int m_nAllDefenceFlags;
};

#endif // EMPLOY_SCOREBOARD_H
