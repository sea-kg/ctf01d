#ifndef ARGUMENT_PROCESSOR_TEAMS_H
#define ARGUMENT_PROCESSOR_TEAMS_H

#include <wsjcpp_arguments.h>

class ArgumentProcessorTeams : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorTeams();
};

class ArgumentProcessorTeamsList : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorTeamsList();

        virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

class ArgumentProcessorTeamsSearch : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorTeamsSearch();

        virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

#endif // ARGUMENT_PROCESSOR_TEAMS_H
