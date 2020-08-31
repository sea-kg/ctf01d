
#include "argument_processor_web_test.h"
#include <wsjcpp_core.h>
#include <employ_config.h>
#include <light_web_http_handler_team_logo.h>
#include <http_handler_web_folder.h>
#include <http_handler_api_v1.h>

// ---------------------------------------------------------------------
// ArgumentProcessorWebTest

ArgumentProcessorWebTest::ArgumentProcessorWebTest() 
: WsjcppArgumentProcessor({"web-test"}, "Start alone http server for test", "Start alone http server for test") {
    TAG = "ArgumentProcessorWebTest";
    // registrySingleArgument("--single", "What exactly do this single param?");
    // registryParameterArgument("-param", "N", "What need this param?");
    // registryExample("here example of command");
    // registryProcessor(new ArgumentProcessorOtherProcessor());
}

// ---------------------------------------------------------------------

bool ArgumentProcessorWebTest::applySingleArgument(const std::string &sProgramName, const std::string &sArgumentName) {
    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

// ---------------------------------------------------------------------

bool ArgumentProcessorWebTest::applyParameterArgument(
    const std::string &sProgramName, 
    const std::string &sArgumentName, 
    const std::string &sValue
) {
    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

// ---------------------------------------------------------------------

int ArgumentProcessorWebTest::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    WsjcppLog::info(TAG, "Web Test...");
    if (!WsjcppEmployees::init({})) {
        WsjcppLog::err(TAG, "Failed.");
        return -1;
    }

    EmployConfig *pEmployConfig = findWsjcppEmploy<EmployConfig>();

    WsjcppLightWebServer httpServer;
    httpServer.addHandler(new LightWebHttpHandlerTeamLogo());
    httpServer.addHandler(new HttpHandlerWebFolder(pEmployConfig->scoreboardHtmlFolder()));
    httpServer.addHandler(new HttpHandlerApiV1());
    httpServer.setPort(pEmployConfig->scoreboardPort());
    httpServer.setMaxWorkers(5);
    httpServer.startSync(); // will be block thread

    WsjcppLog::err(TAG, "Not implemented");
    return -1; 
}

