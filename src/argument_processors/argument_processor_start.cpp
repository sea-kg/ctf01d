
#include "argument_processor_start.h"
#include <wsjcpp_core.h>
#include <employ_config.h>
#include <light_web_http_handler_team_logo.h>
#include <http_handler_web_folder.h>
#include <http_handler_api_v1.h>

// ---------------------------------------------------------------------
// ArgumentProcessorStart

ArgumentProcessorStart::ArgumentProcessorStart() 
: WsjcppArgumentProcessor({"start"}, "TODO short description", "TODO long description") {
    TAG = "ArgumentProcessorStart";
    // registrySingleArgument("--single", "What exactly do this single param?");
    // registryParameterArgument("-param", "N", "What need this param?");
    registryExample("ctf01d -work-dir ./data_test -db-host localhost start");
}

// ---------------------------------------------------------------------

bool ArgumentProcessorStart::applySingleArgument(const std::string &sProgramName, const std::string &sArgumentName) {
    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

// ---------------------------------------------------------------------

bool ArgumentProcessorStart::applyParameterArgument(
    const std::string &sProgramName, 
    const std::string &sArgumentName, 
    const std::string &sValue
) {
    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

// ---------------------------------------------------------------------

int ArgumentProcessorStart::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    WsjcppLog::info(TAG, "Starting...");
    WsjcppEmployees::init({});

    EmployConfig *pEmployConfig = findWsjcppEmploy<EmployConfig>();

    pEmployConfig->scoreboard()->initStateFromStorage();

    for (unsigned int iservice = 0; iservice < pEmployConfig->servicesConf().size(); iservice++) {
        for (unsigned int iteam = 0; iteam < pEmployConfig->teamsConf().size(); iteam++) {
            Ctf01dTeamDef teamConf = pEmployConfig->teamsConf()[iteam];
            Ctf01dServiceDef serviceConf = pEmployConfig->servicesConf()[iservice];

            // reset status to down
            pEmployConfig->scoreboard()->setServiceStatus(teamConf.id(), serviceConf.id(), ServiceStatusCell::SERVICE_DOWN);
            // pConfig->scoreboard()->setTeamTries();

            ServiceCheckerThread *thr = new ServiceCheckerThread(teamConf, serviceConf);
            thr->start();
            m_vThreads.push_back(thr);
        }
    }

    WsjcppLog::ok(TAG, "Starting scoreboard on http://localhost:" + std::to_string(pEmployConfig->scoreboardPort()) + "/");
    
    WsjcppLightWebServer httpServer;
    
    httpServer.addHandler(new LightWebHttpHandlerTeamLogo());
    httpServer.addHandler(new HttpHandlerWebFolder(pEmployConfig->scoreboardHtmlFolder()));
    httpServer.addHandler(new HttpHandlerApiV1());
    httpServer.setPort(pEmployConfig->scoreboardPort());
    httpServer.setMaxWorkers(10);
    httpServer.startSync(); // will be block thread

    // TODO: stop all threads

    /*while(1) {
        Log::info(TAG, "wait 2 minutes");
        std::this_thread::sleep_for(std::chrono::minutes(2));
        Log::info(TAG, "wait ended");
    }*/

    return -1; 
}

