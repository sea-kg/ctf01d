
#include "argument_processor_clean.h"
#include <wsjcpp_core.h>
#include <wsjcpp_employees.h>
#include <employ_config.h>
// ---------------------------------------------------------------------
// ArgumentProcessorClean

ArgumentProcessorClean::ArgumentProcessorClean() 
: WsjcppArgumentProcessor({"clean"}, "Clean all flags from storage", "Clean all flags from storage") {
    TAG = "ArgumentProcessorClean";
    // registrySingleArgument("--single", "What exactly do this single param?");
    // registryParameterArgument("-param", "N", "What need this param?");
    // registryExample("here example of command");
    // registryProcessor(new ArgumentProcessorOtherProcessor());
}

// ---------------------------------------------------------------------

bool ArgumentProcessorClean::applySingleArgument(const std::string &sProgramName, const std::string &sArgumentName) {
    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

// ---------------------------------------------------------------------

bool ArgumentProcessorClean::applyParameterArgument(
    const std::string &sProgramName, 
    const std::string &sArgumentName, 
    const std::string &sValue
) {
    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

// ---------------------------------------------------------------------

int ArgumentProcessorClean::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    WsjcppLog::info(TAG, "Cleaning flags from storage...");
    if (!WsjcppEmployees::init({})) {
        WsjcppLog::err(TAG, "Failed");
        return 0;
    }
    EmployConfig *pEmployConfig = findWsjcppEmploy<EmployConfig>();
    pEmployConfig->storage()->clean();
    WsjcppLog::ok(TAG, "Clean completed.");
    return 0; 
}

