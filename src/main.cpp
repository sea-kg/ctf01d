#include <argument_processor_ctf01d_main.h>
#include <wsjcpp_core.h>
#include <employ_config.h>

// ---------------------------------------------------------------------

int main(int argc, const char* argv[]) {
    std::string TAG = "MAIN";
    std::string appName = std::string(WSJCPP_APP_NAME);
    std::string appVersion = std::string(WSJCPP_APP_VERSION);

    // previous logs in current directory
    if (!WsjcppCore::dirExists(".ctf01d")) {
        WsjcppCore::makeDir(".ctf01d");
    }
    WsjcppLog::setPrefixLogFile("ctf01d");
    WsjcppLog::setLogDirectory(".ctf01d");

    // try find config.yml
    
    std::vector<std::string> vPossibleFolders = {
        "./",
        "./data_sample/",
        "/root/data/"
    };

    for (int i = 0; i < vPossibleFolders.size(); i++) {
        std::string sWorkDir = vPossibleFolders[i];
        if (sWorkDir[0] != '/') {
            sWorkDir = WsjcppCore::getCurrentDirectory() + "/" + sWorkDir;    
        }
        sWorkDir = WsjcppCore::doNormalizePath(sWorkDir);
        if (WsjcppCore::fileExists(sWorkDir + "/config.yml")) {
            std::cout << "Automatically detected workdir: " << sWorkDir << std::endl;
            EmployConfig *pConfig = findWsjcppEmploy<EmployConfig>();
            pConfig->setWorkDir(sWorkDir);
            break;
        }
    }

    ArgumentProcessorCtf01dMain *pMain = new ArgumentProcessorCtf01dMain();
    WsjcppArguments prog(argc, argv, pMain);
    int nRet = prog.exec();
    return nRet;
}
