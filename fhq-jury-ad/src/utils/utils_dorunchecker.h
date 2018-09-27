#ifndef UTILS_DORUNCHECKER_H
#define UTILS_DORUNCHECKER_H

#include <string>
#include <stdio.h>
#include <iostream>
#include <mutex>
#include <deque>

class DoRunChecker {
    public:
        DoRunChecker(
            const std::string &sScript,
            const std::string &sIp,
            const std::string &sCommand,
            const std::string &sFlagId,
            const std::string &sFlag
        );
        void start(int nTimeoutMS);
        bool hasError();
        int exitCode();
        bool isTimeout();
        const std::string &outputString();
        void run();

    private:
        std::string TAG;
        std::string m_sScript;
        std::string m_sIp;
        std::string m_sCommand;
        std::string m_sFlagId;
        std::string m_sFlag;
        int m_nTimeoutMS;
        pid_t m_nPid;
        pthread_t m_pProcessThread;
        int m_nExitCode;
        bool m_bHasError;
        bool m_bFinished;
        bool m_bFinishedByTimeout;
        std::string m_sOutput;
};


#endif // UTILS_DORUNCHECKER_H