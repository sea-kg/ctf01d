
#include "argument_processor_version.h"
#include <wsjcpp_core.h>

// ---------------------------------------------------------------------
// ArgumentProcessorVersion

ArgumentProcessorVersion::ArgumentProcessorVersion() 
: WsjcppArgumentProcessor({"version"}, "Print version", "Print version") {
    TAG = "ArgumentProcessorVersion";
    // registrySingleArgument("--single", "What exactly do this single param?");
    // registryParameterArgument("-param", "N", "What need this param?");
    // registryExample("here example of command");
    // registryProcessor(new ArgumentProcessorOtherProcessor());
}

// ---------------------------------------------------------------------

bool ArgumentProcessorVersion::applySingleArgument(const std::string &sProgramName, const std::string &sArgumentName) {
    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

// ---------------------------------------------------------------------

bool ArgumentProcessorVersion::applyParameterArgument(
    const std::string &sProgramName, 
    const std::string &sArgumentName, 
    const std::string &sValue
) {
    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

// ---------------------------------------------------------------------

int ArgumentProcessorVersion::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    std::cout << "" << WSJCPP_APP_NAME << " " << WSJCPP_APP_VERSION << "\n";
    return 0; 
}

