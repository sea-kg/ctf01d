#include <argument_processor_ctf01d_main.h>
#include <wsjcpp_core.h>

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

    ArgumentProcessorCtf01dMain *pMain = new ArgumentProcessorCtf01dMain();
    WsjcppArguments prog(argc, argv, pMain);
    return prog.exec();
}
