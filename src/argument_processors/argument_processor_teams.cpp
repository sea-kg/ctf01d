
#include "argument_processor_teams.h"
#include <wsjcpp_core.h>
#include <wsjcpp_employees.h>
#include <employ_config.h>
#include "ctf01d_store.h"

// ---------------------------------------------------------------------
// ArgumentProcessorTeams

ArgumentProcessorTeams::ArgumentProcessorTeams() 
: WsjcppArgumentProcessor({"teams"}, "Teams in config.yml", "Teams in config.yml") {
    TAG = "ArgumentProcessorTeams";
    registryProcessor(new ArgumentProcessorTeamsList());
    registryProcessor(new ArgumentProcessorTeamsSearch());
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
                    << " - " << team.getId() << " " << std::endl
                    << "     name: " << team.getName() << std::endl
                    << "     ip-address: " << team.ipAddress() << std::endl
                    << "     logo: " << team.logo() << std::endl
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

// ---------------------------------------------------------------------
// ArgumentProcessorTeamsSearch

ArgumentProcessorTeamsSearch::ArgumentProcessorTeamsSearch() 
: WsjcppArgumentProcessor({"search", "-s"}, "Search teams", "Search existing teams") {
    TAG = "ArgumentProcessorTeamsSearch";
}

// ---------------------------------------------------------------------

int ArgumentProcessorTeamsSearch::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    
    Ctf01dStore store;
    if (!store.init()) {
        WsjcppLog::err(TAG, "Could not init ctf01d-store");
        return -1;
    }

    std::vector<Ctf01dStoreTeamInfo> vFoundTeams = store.searchTeam(vSubParams);
    if (vFoundTeams.size() == 0) {
        std::cout << "Nothing found by terms: " << std::endl;
        for (int i = 0; i < vSubParams.size(); i++) {
            std::cout << " - " << vSubParams[i] << std::endl;
        }
        return -1;
    }

    std::cout << "Found teams: " << std::endl;
    for (int i = 0; i < vFoundTeams.size(); i++) {
        Ctf01dStoreTeamInfo teamInfo = vFoundTeams[i];
        std::cout
            << "team id='" << teamInfo.getId() << "';"
            << " name: '" << teamInfo.getName() << "'"
            << std::endl
        ;
    }
    return 0;
}
