
#include "argument_processor_services.h"
#include <wsjcpp_core.h>
#include <wsjcpp_employees.h>
#include <employ_config.h>

// ---------------------------------------------------------------------
// ArgumentProcessorServices

ArgumentProcessorServices::ArgumentProcessorServices() 
: WsjcppArgumentProcessor({"services"}, "Services in config.yml", "Services in config.yml") {
    TAG = "ArgumentProcessorServices";
    registryProcessor(new ArgumentProcessorServicesList());
}

// ---------------------------------------------------------------------
// ArgumentProcessorServicesList

ArgumentProcessorServicesList::ArgumentProcessorServicesList() 
: WsjcppArgumentProcessor({"list", "ls"}, "List services", "List of services exists in config.yml") {
    TAG = "ArgumentProcessorServicesList";
}

// ---------------------------------------------------------------------

int ArgumentProcessorServicesList::exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) {
    if (vSubParams.size() != 0) {
        WsjcppLog::err(TAG, "Unexpected parameters");
        return -1;
    }
    
    WsjcppLog::info(TAG, "Starting...");
    if (!WsjcppEmployees::init({})) { // TODO silent
        WsjcppLog::err(TAG, "Failed.");
        return -1;
    }

    EmployConfig *pEmployConfig = findWsjcppEmploy<EmployConfig>();

    // TODO move to EmployScoreboard::init
    const std::vector<Ctf01dServiceDef> &vServices = pEmployConfig->servicesConf();
    std::cout
        << std::endl
        << "Services:"
        << std::endl
    ;
    if (vServices.size() > 0) {
        for (int i = 0; i < vServices.size(); i++) {
            Ctf01dServiceDef service = vServices[i];
            if (service.isEnabled()) {
                std::cout
                    << " - id: " << service.id() << " " << std::endl
                    << "   name: " << service.name() << std::endl
                    << "   script-path: " << service.scriptPath() << std::endl
                    << "   script-dir: " << service.scriptDir() << std::endl
                    << "   script-timeout: " << service.scriptWaitInSec() << std::endl
                    << "   script-round: " << service.timeSleepBetweenRunScriptsInSec() << std::endl
                    << std::endl;
            }
        }
        std::cout << std::endl;
    } else {
        std::cout
            << std::endl
            << "Not found services, for add someone please call 'ctf01d services install servicename'"
            << std::endl
            << std::endl
        ;
    }
    return 0;
}
