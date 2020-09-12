
#include "argument_processor_check.h"
#include <wsjcpp_core.h>
#include <wsjcpp_employees.h>
// ---------------------------------------------------------------------
// ArgumentProcessorCheck

ArgumentProcessorCheck::ArgumentProcessorCheck() 
: WsjcppArgumentProcessor({"check"}, "Check config && connection", "Check config && connection") {
    TAG = "ArgumentProcessorCheck";
    // registrySingleArgument("--single", "What exactly do this single param?");
    // registryParameterArgument("-param", "N", "What need this param?");
    // registryExample("here example of command");
    // registryProcessor(new ArgumentProcessorOtherProcessor());
}

// ---------------------------------------------------------------------

bool ArgumentProcessorCheck::applySingleArgument(const std::string &sProgramName, const std::string &sArgumentName) {
    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

// ---------------------------------------------------------------------

bool ArgumentProcessorCheck::applyParameterArgument(
    const std::string &sProgramName, 
    const std::string &sArgumentName, 
    const std::string &sValue
) {
    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

// ---------------------------------------------------------------------

int ArgumentProcessorCheck::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    WsjcppLog::info(TAG, "Checking configuration...");
    if (!WsjcppEmployees::init({})) {
        WsjcppLog::err(TAG, "Failed.");
        return -1;    
    }
    WsjcppLog::ok(TAG, "Configuration is ok.");
    return 0;
    
}

