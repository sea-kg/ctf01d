#ifndef EMPLOY_TEAM_LOGOS_H
#define EMPLOY_TEAM_LOGOS_H

#include <wsjcpp_employees.h>

class EmployTeamLogos : public WsjcppEmployBase {
    public:
        EmployTeamLogos();
        static std::string name() { return "EmployTeamLogos"; }
        virtual bool init() override;
        virtual bool deinit() override;

    private:
        std::string TAG;
};

#endif // EMPLOY_TEAM_LOGOS_H
