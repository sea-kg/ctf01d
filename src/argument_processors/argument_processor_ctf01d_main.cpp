
#include "argument_processor_ctf01d_main.h"
#include <wsjcpp_core.h>
#include <employ_config.h>

// ---------------------------------------------------------------------
// ArgumentProcessorCtf01dMain

ArgumentProcessorCtf01dMain::ArgumentProcessorCtf01dMain() 
: WsjcppArgumentProcessor({"ctf01d_main"}, "Jury System for ctf-attack-defence", "Jury System for ctf-attack-defence") {
    TAG = "ArgumentProcessorCtf01dMain";
    // registrySingleArgument("--single", "What exactly do this single param?");
    registryParameterArgument("-mysql-host", "hostname", "Force apply host to mysql database");
    registryParameterArgument("-work-dir", "path", "Custom workspace folder with configs, logging, checker scripts and etc.");
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
    EmployConfig *pConfig = findWsjcppEmploy<EmployConfig>();
    
    if (sArgumentName == "-work-dir") {
        std::string sWorkDir = WsjcppCore::getCurrentDirectory() + sValue;
        sWorkDir = WsjcppCore::doNormalizePath(sWorkDir);
        pConfig->setWorkDir(sWorkDir);
    }
    
    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

// ---------------------------------------------------------------------

int ArgumentProcessorCtf01dMain::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    WsjcppLog::err(TAG, "Not implemented");
    return -1; 
}

