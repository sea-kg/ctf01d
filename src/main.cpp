// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <signal.h>
// #include <iostream>
// #include <sys/stat.h>
// #include <sys/types.h>
// #include <sys/time.h>
// #include <unistd.h>
// #include <errno.h>
// #include <fcntl.h>
// #include <syslog.h>
// #include <chrono>
// #include <thread>
// #include <algorithm>
// #include <service_checker_thread.h>
// #include <light_web_http_handler_team_logo.h>
// #include <http_handler_web_folder.h>
// #include <http_handler_api_v1.h>
// #include <storages.h>
// #include <unistd.h>
// #include <limits.h>
// #include <wsjcpp_core.h>
#include <argument_processor_ctf01d_main.h>

// ---------------------------------------------------------------------

int main(int argc, const char* argv[]) {
    ArgumentProcessorCtf01dMain *pMain = new ArgumentProcessorCtf01dMain();
    WsjcppArguments prog(argc, argv, pMain);
    return prog.exec();
}
