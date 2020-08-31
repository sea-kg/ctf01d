
#include "argument_processor_ctf01d_main.h"
#include <wsjcpp_core.h>
#include <employ_config.h>
#include <argument_processor_start.h>
#include <argument_processor_clean.h>
#include <argument_processor_check.h>
#include <argument_processor_version.h>
#include <argument_processor_web_test.h>


// ---------------------------------------------------------------------
// ArgumentProcessorCtf01dMain

ArgumentProcessorCtf01dMain::ArgumentProcessorCtf01dMain() 
: WsjcppArgumentProcessor({"ctf01d_main"}, "Jury System for ctf-attack-defence", "Jury System for ctf-attack-defence") {
    TAG = "ArgumentProcessorCtf01dMain";
    // registrySingleArgument("--single", "What exactly do this single param?");
    registryParameterArgument("-work-dir", "path", "Custom workspace folder with configs, logging, checker scripts and etc. (env: CTF01D_WORKDIR)");
    registryParameterArgument("-db-host", "hostname", "Force apply host to database (env: CTF01D_DB_HOST)");
    registryExample("ctf01d -work-dir ./data_test -db-host localhost start");
    registryProcessor(new ArgumentProcessorVersion());
    registryProcessor(new ArgumentProcessorCheck());
    registryProcessor(new ArgumentProcessorClean());
    registryProcessor(new ArgumentProcessorWebTest());
    registryProcessor(new ArgumentProcessorStart());
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
        return true;
    }

    if (sArgumentName == "-db-host") {
        pConfig->setDatabaseHost(sValue);
        return true;
    }

    WsjcppLog::err(TAG, "Not implemented");
    return false;
}

// ---------------------------------------------------------------------

int ArgumentProcessorCtf01dMain::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    WsjcppLog::err(TAG, "Not implemented");
    return -1; 
}