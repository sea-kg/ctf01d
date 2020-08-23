
#include "argument_processor_ctf01d_main.h"
#include <wsjcpp_core.h>

// ---------------------------------------------------------------------
// ArgumentProcessorCtf01dMain

ArgumentProcessorCtf01dMain::ArgumentProcessorCtf01dMain() 
: WsjcppArgumentProcessor({"ctf01d_main"}, "TODO short description", "TODO long description") {
    TAG = "ArgumentProcessorCtf01dMain";
    // registrySingleArgument("--single", "What exactly do this single param?");
    registryParameterArgument("-mysql-host", "hostname", "Force apply host to mysql database");
    registryParameterArgument("-work-directory", "path", "Work Directory (folder with configs)");
    // registryExample("here example of command");
    // registryProcessor(new ArgumentProcessorOtherProcessor());
}

// ---------------------------------------------------------------------

bool ArgumentProcessorCtf01dMain::applySingleArgument(const std::string &sProgramName, const std::string &sArgumentName) {
    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

// ---------------------------------------------------------------------

bool ArgumentProcessorCtf01dMain::applyParameterArgument(
    const std::string &sProgramName, 
    const std::string &sArgumentName, 
    const std::string &sValue
) {
    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

// ---------------------------------------------------------------------

int ArgumentProcessorCtf01dMain::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    WsjcppLog::err(TAG, "Not implemented");
    return -1; 
}

