#include "utils_logger.h"
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <iostream>
#include <thread>
#include <stdio.h>
#include <sstream>
#include <fs.h>
#include <wsjcpp_core.h>

// Last log messages
std::mutex *g_LOG_MUTEX = NULL;

// ---------------------------------------------------------------------

void Log::info(const std::string &sTag, const std::string &sMessage){
    Color::Modifier def(Color::FG_DEFAULT);
    Log::add(def, "INFO", sTag, sMessage);
}

// ---------------------------------------------------------------------

void Log::err(const std::string &sTag, const std::string &sMessage){
	Color::Modifier red(Color::FG_RED);
    Log::add(red, "ERR", sTag, sMessage);
}

// ---------------------------------------------------------------------

void Log::warn(const std::string &sTag, const std::string &sMessage){
	Color::Modifier yellow(Color::FG_YELLOW);
    Log::add(yellow, "WARN", sTag, sMessage);
}

// ---------------------------------------------------------------------

void Log::ok(const std::string &sTag, const std::string &sMessage) {
    Color::Modifier green(Color::FG_GREEN);
    Log::add(green, "OK", sTag, sMessage);
}

// ---------------------------------------------------------------------

void Log::initGlobalVariables(){
    // create mutex if not created
    if(g_LOG_MUTEX == NULL){
        g_LOG_MUTEX = new std::mutex();
        // std::cout << Log::currentTime() + ", " + Log::threadId() + " Init mutex for log\r\n";
    }
}

// ---------------------------------------------------------------------

std::string Log::g_LOG_DIR = "./";
std::string Log::g_LOG_FILE = "";
long Log::g_LOG_START_TIME = 0;

// ---------------------------------------------------------------------

void Log::logRotate_updateFilename() {
    long t = WsjcppCore::currentTime_seconds();
    // rotate log if started now or if time left more then 10 min
    if (g_LOG_START_TIME == 0 || t - g_LOG_START_TIME > 600) {
        g_LOG_START_TIME = t;
        g_LOG_FILE = g_LOG_DIR + "/fhq-jury-ad_" + WsjcppCore::formatTimeForFilename(g_LOG_START_TIME) + ".log";
    }
}

// ---------------------------------------------------------------------

void Log::setDir(const std::string &sDir) {
    g_LOG_DIR = sDir;
    Log::logRotate_updateFilename();
}

// ---------------------------------------------------------------------

std::string Log::threadId() {
    std::thread::id this_id = std::this_thread::get_id();
    std::stringstream stream;
    stream << std::hex << this_id;
    return "0x" + std::string(stream.str());
}

// ---------------------------------------------------------------------

void Log::add(Color::Modifier clr, const std::string &sType, const std::string &sTag, const std::string &sMessage){
    Log::initGlobalVariables();
    Log::logRotate_updateFilename();

    std::lock_guard<std::mutex> lock(*g_LOG_MUTEX);

    Color::Modifier def(Color::FG_DEFAULT);
    std::string sLogMessage = WsjcppCore::currentTime_logformat() + ", " + Log::threadId() + " [" + sType + "] " + sTag + ": " + sMessage;
    std::cout << clr << sLogMessage << def << std::endl;

    std::ofstream logFile(g_LOG_FILE, std::ios::app);
    if (!logFile) {
        std::cout << "Error Opening File" << std::endl;
        return;
    }

    logFile << sLogMessage << std::endl;
    logFile.close();
}
