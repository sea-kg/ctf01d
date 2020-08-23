#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <chrono>
#include <thread>
#include <algorithm>
#include <service_checker_thread.h>
#include <light_web_http_handler_team_logo.h>
#include <http_handler_web_folder.h>
#include <http_handler_api_v1.h>
#include <utils_help_parse_args.h>
#include <storages.h>
#include <unistd.h>
#include <limits.h>
#include <resources_manager.h>
#include <wsjcpp_core.h>
#include <argument_processor_ctf01d_main.h>
#include <employ_config.h>

WsjcppLightWebServer g_httpServer;
std::vector<ServiceCheckerThread *> g_vThreads;

// ---------------------------------------------------------------------

void quitApp(int signum) {
    std::cout << std::endl << "Terminating server by signal " << signum << std::endl;
    g_httpServer.stop();
    exit(1);
}

// ---------------------------------------------------------------------

int main(int argc, const char* argv[]) {
    std::string TAG = "MAIN";

    WsjcppEmployees::init({});

    ArgumentProcessorCtf01dMain *pMain = new ArgumentProcessorCtf01dMain();
    WsjcppArguments prog(argc, argv, pMain);
    prog.exec();
    // return prog.exec();

    HelpParseArgs helpParseArgs(argc, argv);

    helpParseArgs.addHelp("help", "-h", false,
        "This help");

    helpParseArgs.addHelp("clean", "-cl", false, 
        "Clean all flags from storage");

    helpParseArgs.addHelp("version", "-v", false,
        "Print version");

    helpParseArgs.addHelp("start", "-s", false, 
        "Start jury");

    helpParseArgs.addHelp("check", "-ch", false,
        "Just check config");

    helpParseArgs.addHelp("check-http", "-cht", false, 
        "Start alone http server for check");

    helpParseArgs.addHelp("--extract-files", "-ef", false, 
        "Will be created all default files");

    std::string sArgsErrors;
    if (!helpParseArgs.checkArgs(sArgsErrors)){
        WsjcppLog::err(TAG, "Arguments has errors " + sArgsErrors);
        return -1;
    }

    if (helpParseArgs.has("version")) {
        std::cout << "" << WSJCPP_APP_NAME << " " << WSJCPP_APP_VERSION << "\n";
        return 0;
    }

    if (helpParseArgs.has("help")) {
        helpParseArgs.printHelp();
        return 0;
    }

    
    // CTF01D_WORKDIR
    EmployConfig *pEmployConfig = findWsjcppEmploy<EmployConfig>();

    std::string sWorkDir = pEmployConfig->getWorkDir();
    if (sWorkDir == "") {
        WsjcppLog::throw_err(TAG, "Work Directory not defined.");
    }
    
    // create default folders and files
    if (helpParseArgs.has("--extract-files")) {
        if (!WsjcppCore::dirExists(sWorkDir)) {
            WsjcppCore::makeDir(sWorkDir);
        }
        if (!ResourcesManager::make(sWorkDir)) {
            std::cout << "Could not create some folders or files in " << sWorkDir << " please check access" << std::endl;
            return -1;
        }
    }

    if (!WsjcppCore::dirExists(sWorkDir)) {
        WsjcppLog::err(TAG, "Directory " + sWorkDir + " does not exists");
        return -1;
    }

    /*if (sWorkDir.length() > 0) {
        if (sWorkDir[0] != '/') { // linux
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("Current working dir: %s\n", cwd);
                sWorkDir = std::string(cwd) + "/" + sWorkDir;
            } else {
                std::cout << "getcwd() error" << std::endl;
            }
        }
    }*/

    std::string sLogDir = sWorkDir + "/logs";
    if (!WsjcppCore::dirExists(sLogDir)) {
        std::cout << "Error: Folder " << sLogDir << " does not exists \n";
        return -1;
    }

    WsjcppLog::setPrefixLogFile("ctf01d");
    WsjcppLog::setLogDirectory(sLogDir);
    WsjcppLog::setRotationPeriodInSec(600); // every 10 min 
    // TODO rotation period must be in config.yml

    std::cout << "Logger: '" + sWorkDir + "/logs/' \n";
    WsjcppLog::info(TAG, "Version: " + std::string(WSJCPP_APP_VERSION));

    
    if (!pEmployConfig->applyConfig()) {
        WsjcppLog::err(TAG, "Configuration file has some problems");
        return -1;
    }

    if (helpParseArgs.has("check")) {
        return 0;
    }

    if (helpParseArgs.has("clean")) {
        WsjcppLog::warn(TAG, "Cleaning flags from storage...");
        pEmployConfig->storage()->clean();
        WsjcppLog::ok(TAG, "DONE");
        return 0;
    }

    // configure http handlers
    g_httpServer.addHandler(new LightWebHttpHandlerTeamLogo());
    g_httpServer.addHandler(new HttpHandlerWebFolder(pEmployConfig->scoreboardHtmlFolder()));

    signal( SIGINT, quitApp );
    signal( SIGTERM, quitApp );

    if (helpParseArgs.has("check-http")) {
        pEmployConfig->setStorage(Storages::create("file", pEmployConfig->gameStartUTCInSec(), pEmployConfig->gameEndUTCInSec())); // replace storage to ram for tests
        // std::cout << "==== SCOREBOARD ==== \n" << pConfig->scoreboard()->toString() << "\n";
        g_httpServer.addHandler(new HttpHandlerApiV1());
        g_httpServer.setPort(pEmployConfig->scoreboardPort());
        g_httpServer.setMaxWorkers(1);
        g_httpServer.startSync(); // will be block thread
        return 0;
    }

    if (helpParseArgs.has("start")) {
        WsjcppLog::info(TAG, "Starting...");

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
                g_vThreads.push_back(thr);
            }
        }

        WsjcppLog::ok(TAG, "Start scoreboard on " + std::to_string(pEmployConfig->scoreboardPort()));
        g_httpServer.addHandler(new HttpHandlerApiV1());
        // pConfig->setStorage(new RamStorage(pConfig->scoreboard())); // replace storage to ram for tests
        g_httpServer.setPort(pEmployConfig->scoreboardPort());
        g_httpServer.setMaxWorkers(10);
        g_httpServer.startSync(); // will be block thread

        // TODO: stop all threads

        /*while(1) {
            Log::info(TAG, "wait 2 minutes");
            std::this_thread::sleep_for(std::chrono::minutes(2));
            Log::info(TAG, "wait ended");
        }*/
        return 0;
    }

    helpParseArgs.printHelp();
    return 0;
}
