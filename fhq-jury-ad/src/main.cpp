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
#include <team.h>
#include <utils_logger.h>
#include <light_http_server.h>
#include <utils_help_parse_args.h>
#include <utils_search_lazy_conf.h>
#include <http_handler.h>
#include <storages.h>
#include <create_defaults.h>
#include <unistd.h>
#include <limits.h>
#include <fs.h>

LightHttpServer g_httpServer;
std::vector<ServiceCheckerThread *> g_vThreads;

// ---------------------------------------------------------------------

void quitApp(int signum) {
  std::cout << std::endl << "Terminating server by signal " << signum << std::endl;
  g_httpServer.stop();
  exit(1);
}

// ---------------------------------------------------------------------

int main(int argc, char* argv[]) {
    std::string TAG = "MAIN";

    HelpParseArgs helpParseArgs(argc, argv);

    helpParseArgs.addHelp("help", "-h", false,
        "This help");

    helpParseArgs.addHelp("clean", "-cl", false, 
        "Clean all flags from storage");

    helpParseArgs.addHelp("version", "-v", false,
        "Print version");
    
    helpParseArgs.addHelp("search-lazy-conf", "-sl", false, 
        "Start search lazy-conf service - helpfull function");

    helpParseArgs.addHelp("start", "-s", false, 
        "Start jury");

    helpParseArgs.addHelp("check", "-ch", false,
        "Just check config");

    helpParseArgs.addHelp("check-http", "-cht", false, 
        "Start alone http server for check");

    helpParseArgs.addHelp("--workspace-dir", "-wd", true, 
        "Custom workspace folder with configs, logging, services and etc.");

    helpParseArgs.addHelp("lazy-start", "-lzs", false, 
        "Start jury with scanning network for make list of teams use running machines with lazy-conf service");
    
    // helpParseArgs.addHelp("hub-search", "-hs", true
    //    "Show all list of services available from service-hub");

    // helpParseArgs.addHelp("hub-install", "-hi", true, 
    //    "Download service from service-hub");

    // helpParseArgs.addHelp("--servicehub-add", "-sh-add", true, 
    //     "Added new link to servicehub");

    // helpParseArgs.addHelp("--servicehub-remove", "-sh-rm", true, 
    //     "Remove link to servicehub");

    // helpParseArgs.addHelp("--servicehub-update", "-sh-upd", false, 
    //     "Show links to servicehub");

    // helpParseArgs.addHelp("--servicehub-list", "-sh-ls", true, 
    //     "Show list of services");

    // helpParseArgs.addHelp("--servicehub-download", "-sh-download", true, 
    //     "Download service sources (+checker) to cache");

    // helpParseArgs.addHelp("--servicehub-enable", "-sh-en", true, 
    //     "Enable service to current game");

    // helpParseArgs.addHelp("--servicehub-disable", "-sh-dis", true, 
    //     "Disable service from current game");

    // helpParseArgs.addHelp("--servicehub-export", "-sh-exp", true, 
    //     "Export services from current game to custom folder");

    // helpParseArgs.addHelp("--servicehub-cache-clear", "-sh-cacl", true, 
    //     "Clean cache folder");

    std::string sArgsErrors;
    if (!helpParseArgs.checkArgs(sArgsErrors)){
        Log::err(TAG, "Arguments has errors " + sArgsErrors);
        return -1;
    }

    if (helpParseArgs.has("version")) {
        std::cout << "" << JURY_AD_APP_NAME << " " << JURY_AD_VERSION << "\n";
        return 0;
    }

    if (helpParseArgs.has("help")) {
        helpParseArgs.printHelp();
        return 0;
    }

    std::string sWorkspace = "/usr/share/fhq-jury-ad/jury.d"; // default workspace
    if (helpParseArgs.has("--workspace-dir")) {
        // todo replace workspace path
        sWorkspace = helpParseArgs.option("--workspace-dir");
        // TODO check directory existing and apply dir
    }
    
    /*if (sWorkspace.length() > 0) {
        if (sWorkspace[0] != '/') { // linux
            char cwd[PATH_MAX];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("Current working dir: %s\n", cwd);
                sWorkspace = std::string(cwd) + "/" + sWorkspace;
            } else {
                std::cout << "getcwd() error" << std::endl;
            }
        }
    }*/

    if (!FS::dirExists(sWorkspace)) {
        std::cout << "Error: Folder " << sWorkspace << " does not exists \n";
        return -1;
    }

    // create default folders and files
    if (!CreateDefaults::make(sWorkspace)) {
        std::cout << "Could not create some folders or files in " << sWorkspace << " please check access" << std::endl;
        return -1;
    }

    std::string sLogDir = sWorkspace + "/logs";
    if (!FS::dirExists(sLogDir)) {
        std::cout << "Error: Folder " << sLogDir << " does not exists \n";
        return -1;
    }

    Log::setDir(sLogDir);
    std::cout << "Logger: '" + sWorkspace + "/logs/' \n";
    Log::info(TAG, "Version: " + std::string(JURY_AD_VERSION));
    if (helpParseArgs.has("search-lazy-conf")){
        SearchLazyConf searchLazyConf(8080);
        searchLazyConf.scan();
        // TODO print list of found teams

        return 0;
    }

    bool bLazyStart = helpParseArgs.has("lazy-start");

    Config *pConfig = new Config(sWorkspace);
    if(!pConfig->applyConfig(bLazyStart)){
        Log::err(TAG, "Configuration file has some problems");
        return -1;
    }

    if (helpParseArgs.has("check")) {
        return 0;
    }

    if (helpParseArgs.has("clean")) {
        Log::warn(TAG, "Cleaning flags from storage...");
        pConfig->storage()->clean();
        Log::ok(TAG, "DONE");
        return 0;
    }

    signal( SIGINT, quitApp );
    signal( SIGTERM, quitApp );

    if (helpParseArgs.has("check-http")) {
        pConfig->setStorage(Storages::create("file", pConfig->scoreboard(), pConfig->gameStartUTCInSec(), pConfig->gameEndUTCInSec())); // replace storage to ram for tests
        HttpHandler *pScoreboard = new HttpHandler(pConfig);
        // std::cout << "==== SCOREBOARD ==== \n" << pConfig->scoreboard()->toString() << "\n";
        g_httpServer.start(
            pConfig->scoreboardPort(),
            pConfig->scoreboardHtmlFolder(),
            (ILightHttpHandler *)pScoreboard
        ); // will be block thread
        return 0;
    }

    if (helpParseArgs.has("start") || helpParseArgs.has("lazy-start") ) {
        Log::info(TAG, "Starting...");
        HttpHandler *pScoreboard = new HttpHandler(pConfig);
            
        for (unsigned int iservice = 0; iservice < pConfig->servicesConf().size(); iservice++) {
            for (unsigned int iteam = 0; iteam < pConfig->teamsConf().size(); iteam++) {
                Team teamConf = pConfig->teamsConf()[iteam];
                Service serviceConf = pConfig->servicesConf()[iservice];
                
                pConfig->storage()->updateScoreboard(teamConf, serviceConf);
                pConfig->scoreboard()->setServiceStatus(teamConf.id(), serviceConf.id(), ModelServiceStatus::SERVICE_DOWN);

                ServiceCheckerThread *thr = new ServiceCheckerThread(pConfig, teamConf, serviceConf);
                thr->start();
                g_vThreads.push_back(thr);
            }
        }

        Log::ok(TAG, "Start scoreboard on " + std::to_string(pConfig->scoreboardPort()));
        // pConfig->setStorage(new RamStorage(pConfig->scoreboard())); // replace storage to ram for tests
        g_httpServer.start(
            pConfig->scoreboardPort(),
            pConfig->scoreboardHtmlFolder(),
            (ILightHttpHandler *)pScoreboard
        ); // will be block thread

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
