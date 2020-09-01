#ifndef EMPLOY_SCOREBOARD_H
#define EMPLOY_SCOREBOARD_H

#include <wsjcpp_employees.h>


class Ctf01dServiceCostsAndStatistics {
    public:
        Ctf01dServiceCostsAndStatistics(const std::string &sServiceId); // TODO also add base stollen points and number of service
        int allStolenFlagsForService(); // TODO must start from get
        void incrementStolenFlagsForService(); // TODO must start from do
        void setStolenFlagsForService(int nStolenFlags); // TODO move to updateProportionalStolenFlagsForService
        double updateProportionalStolenFlagsForService(int m_nAllStolenFlags);
        double updateCostStolenFlagForService(int nStolenPoints, double nSumOfReverseProportionalStolenFlags); // TODO nStolenPoints must be calculate on constructor
        double costStolenFlag(); // TODO must start from get

        int allDefenceFlagsForService(); // TODO must start from get
        void incrementDefenceFlagsForService(); // TODO must start from do
        void setDefenceFlagsForService(int nDdefenceFlags);
        double updateProportionalDefenceFlagsForService(int m_nAllDefenceFlags);
        double updateCostDefenceFlagForService(int nDefencePoints, double nSumOfReverseProportionalDefenceFlags); // TODO nDefencePoints must be calculate on constructor
        double costDefenceFlag(); // TODO must start from get

        std::string getFirstBloodTeamId();
        void setFirstBloodTeamId(const std::string &sFirstBlood);

    private:
        std::string TAG;
        std::string m_sServiceId;
        std::string m_sFirstBlood;

        int m_nAllStolenFlagsForService;
        double m_nReverseProportionalStolenFlags;
        double m_nCostStolenFlag;

        int m_nAllDefenceFlagsForService;
        double m_nReverseProportionalDefenceFlags;
        double m_nCostDefenceFlag;
};

class EmployScoreboard : public WsjcppEmployBase {
    public:
        EmployScoreboard();
        static std::string name() { return "EmployScoreboard"; }
        virtual bool init() override;
        virtual bool deinit() override;

    private:
        std::string TAG;
};

#endif // EMPLOY_SCOREBOARD_H
