
#include "argument_processor_teams.h"
#include <wsjcpp_core.h>
#include <wsjcpp_employees.h>
#include <employ_config.h>

// ---------------------------------------------------------------------
// ArgumentProcessorTeams

ArgumentProcessorTeams::ArgumentProcessorTeams() 
: WsjcppArgumentProcessor({"teams"}, "Teams in config.yml", "Teams in config.yml") {
    TAG = "ArgumentProcessorTeams";
    registryProcessor(new ArgumentProcessorTeamsList());
}

// ---------------------------------------------------------------------
// ArgumentProcessorTeamsList

ArgumentProcessorTeamsList::ArgumentProcessorTeamsList() 
: WsjcppArgumentProcessor({"list", "ls"}, "List teams", "List of teams exists in configuration") {
    TAG = "ArgumentProcessorTeamsList";
}

// ---------------------------------------------------------------------

int ArgumentProcessorTeamsList::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    if (vSubParams.size() != 0) {
        WsjcppLog::err(TAG, "Unexpected parameters");
        return -1;
    }
    
    WsjcppLog::info(TAG, "Starting...");
    if (!WsjcppEmployees::init({})) {
        WsjcppLog::err(TAG, "Failed.");
        return -1;
    }

    EmployConfig *pEmployConfig = findWsjcppEmploy<EmployConfig>();

    // TODO move to EmployScoreboard::init
    const std::vector<Ctf01dTeamDef> &vTeams = pEmployConfig->teamsConf();
    std::cout
        << std::endl
        << "Teams:"
        << std::endl
    ;
    if (vTeams.size() > 0) {
        for (int i = 0; i < vTeams.size(); i++) {
            Ctf01dTeamDef team = vTeams[i];
            if (team.isActive()) {
                std::cout
                    << " - " << vTeams[i].getId() << " " << std::endl
                    << "     name: " << vTeams[i].getName() << std::endl
                    << "     ip-address: " << vTeams[i].ipAddress() << std::endl
                    << "     logo: " << vTeams[i].logo() << std::endl
                    << std::endl;
            }
        }
        std::cout << std::endl;
    } else {
        std::cout
            << std::endl
            << "Not found teams, for add someone please call 'ctf01d -work-dir ... teams add teamname'"
            << std::endl
            << std::endl
        ;
    }
    return 0;
}
