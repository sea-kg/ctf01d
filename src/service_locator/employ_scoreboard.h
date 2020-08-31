#ifndef EMPLOY_SCOREBOARD_H
#define EMPLOY_SCOREBOARD_H

#include <wsjcpp_employees.h>

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
