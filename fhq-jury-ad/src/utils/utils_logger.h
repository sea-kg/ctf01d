#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#include <config.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <mutex>
#include <deque>
#include <wsjcpp_core.h>

class Log {

    public:
        static std::string g_LOG_DIR;
        static std::string g_LOG_FILE;
        static long g_LOG_START_TIME;
        static void info(const std::string &sTag, const std::string &sMessage);
        static void err(const std::string &sTag, const std::string &msg);
        static void warn(const std::string &sTag, const std::string &sMessage);
        static void ok(const std::string &sTag, const std::string &sMessage);
        static void initGlobalVariables();
        static void setDir(const std::string &sDir);
        static void logRotate_updateFilename();
        static std::string threadId();

    private:
        static void add(WsjcppColorModifier clr, const std::string &sType, const std::string &sTag, const std::string &sMessage);
        
};

extern std::mutex *g_LOG_MUTEX;

#endif // UTILS_LOG_H
