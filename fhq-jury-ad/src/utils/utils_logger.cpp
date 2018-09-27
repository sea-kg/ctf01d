#include "utils_logger.h"
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <iostream>
#include <thread>
#include <time.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <sstream>

// Last log messages
std::mutex *g_LOG_MUTEX = NULL;

// ---------------------------------------------------------------------

bool Log::fileExists(const std::string &sFilename) {
    struct stat st;
    bool bExists = (stat(sFilename.c_str(), &st) == 0);
    if (bExists) {
        return (st.st_mode & S_IFDIR) == 0;
    }
	return false;
}

// ---------------------------------------------------------------------

bool Log::dirExists(const std::string &sFilename) {
    struct stat st;
    bool bExists = (stat(sFilename.c_str(), &st) == 0);
    if (bExists) {
        return (st.st_mode & S_IFDIR) != 0;
    }
	return false;
}

// ---------------------------------------------------------------------

void Log::info(const std::string &sTag, const std::string &sMessage){
	Color::Modifier def(Color::FG_DEFAULT);
    Log::add(def, "INFO",sTag, sMessage);
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

// ---------------------------------------------------------------------

void Log::setDir(const std::string &sDir) {
    g_LOG_DIR = sDir;
    g_LOG_FILE = g_LOG_DIR + "/jury-ad_" + Log::currentTime_filename() + ".log";
}

// ---------------------------------------------------------------------

std::string Log::currentTime(){
    // milleseconds
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int millisec = lrint(tv.tv_usec/1000.0); // Round to nearest millisec
    if (millisec>=1000) { // Allow for rounding up to nearest second
        millisec -=1000;
        tv.tv_sec++;
    }

    // datetime
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d %X.", &tstruct);
    return std::string(buf) + std::to_string(millisec);
}

// ---------------------------------------------------------------------

std::string Log::currentTime_filename() {
    // milleseconds
    struct timeval tv;
    gettimeofday(&tv, NULL);

    // datetime
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", &tstruct);
    return std::string(buf);
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

    g_LOG_MUTEX->lock();
    Color::Modifier def(Color::FG_DEFAULT);
    std::string sLogMessage = Log::currentTime() + ", " + Log::threadId() + " [" + sType + "] " + sTag + ": " + sMessage;
    std::cout << clr << sLogMessage << def << std::endl;

    std::ofstream logFile(g_LOG_FILE, std::ios::app);
    if (!logFile) {
        std::cout << "Error Opening File" << std::endl;
        g_LOG_MUTEX->unlock();
        return;
    }

    logFile << sLogMessage << std::endl;
    logFile.close();
    g_LOG_MUTEX->unlock();
}