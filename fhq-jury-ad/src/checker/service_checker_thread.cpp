#include "service_checker_thread.h"
#include <unistd.h>

#include <dorunchecker.h>

#include <iostream>
#include <sstream>
#include <utils_logger.h>
#include <chrono>
#include <thread>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <wsjcpp_core.h>

int ServiceCheckerThread::CHECKER_CODE_UP = 101;
int ServiceCheckerThread::CHECKER_CODE_CORRUPT = 102;
int ServiceCheckerThread::CHECKER_CODE_MUMBLE = 103;
int ServiceCheckerThread::CHECKER_CODE_DOWN = 104;
int ServiceCheckerThread::CHECKER_CODE_SHIT = 400;

// ---------------------------------------------------------------------

ServiceCheckerThread::ServiceCheckerThread(Config *pConfig,
            const Team &teamConf, const Service &service) {
    
    m_pConfig = pConfig;
    m_teamConf = teamConf;
    m_serviceConf = service;

    TAG = "Checker: " + m_teamConf.id() + std::string( 15 - m_teamConf.id().length(), ' ')
         + m_serviceConf.id() + " ";
    Log::info(TAG, "Created thread");
}

// ----------------------------------------------------------------------
// newRequest

void* newServiceCheckerThread(void *arg) {
	// Log::info("newRequest", "");
	ServiceCheckerThread *pServerThread = (ServiceCheckerThread *)arg;
	pthread_detach(pthread_self());
	pServerThread->run();
	return 0;
}

// ----------------------------------------------------------------------

void ServiceCheckerThread::start() {
    pthread_create(&m_checkerThread, NULL, &newServiceCheckerThread, (void *)this);
}

// ---------------------------------------------------------------------

int ServiceCheckerThread::runChecker(Flag &flag, const std::string &sCommand) {
    if (sCommand != "put" &&  sCommand != "check") {
        Log::err(TAG, "runChecker - sCommand must be 'put' or 'check' ");
        return ServiceCheckerThread::CHECKER_CODE_SHIT;
    }

    // Used code from here
    // https://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c-using-posix

    std::string sShellCommand = m_serviceConf.scriptPath()
        + " " + m_teamConf.ipAddress()
        + " " + sCommand
        + " " + flag.id()
        + " " + flag.value();

    Log::info(TAG, "Start script " + sShellCommand);

    DoRunChecker process(m_serviceConf.scriptDir(), m_serviceConf.scriptPath(), m_teamConf.ipAddress(), sCommand, flag.id(), flag.value());
    process.start(m_serviceConf.scriptWaitInSec()*1000);

    if (process.isTimeout()) {
        return ServiceCheckerThread::CHECKER_CODE_MUMBLE;
    }

    if (process.hasError()) {
        Log::err(TAG, "Checker is shit");
        Log::err(TAG, "Error on run script service: " + process.outputString());
        return ServiceCheckerThread::CHECKER_CODE_SHIT;
    }

    int nExitCode = process.exitCode();
    if (nExitCode != ServiceCheckerThread::CHECKER_CODE_UP 
        && nExitCode != ServiceCheckerThread::CHECKER_CODE_MUMBLE
        && nExitCode != ServiceCheckerThread::CHECKER_CODE_CORRUPT
        && nExitCode != ServiceCheckerThread::CHECKER_CODE_DOWN) {
        Log::err(TAG, " Wrong checker exit code...\n"
            "\n" + process.outputString());
        return ServiceCheckerThread::CHECKER_CODE_SHIT;
    }

    return nExitCode;
}

// ---------------------------------------------------------------------

void ServiceCheckerThread::run() {
    // TODO: BUG: so here can be problem with mysql connection after 7-8 hours (terminate connection on MySQL side)
    // SOLUTION migrate to PostgreSQL

    // TODO check if game ended

    Log::info(TAG, "Starting thread...");
    /*if (QString::fromStdString(m_teamConf.ipAddress()).isEmpty()) {
        Log::err(TAG, "User IP Address is empty!!!");
        return;
    }*/

    std::string sScriptPath = m_serviceConf.scriptPath();
    /*
    // already checked on start
    if (!Log::fileExists(sScriptPath)) {
        Log::err(TAG, "FAIL: Script Path to checker not found '" + sScriptPath + "'");
        // TODO shit status
        return;
    }*/

    while(1) {
        int nCurrentTime = WsjcppCore::currentTime_seconds();
        if (
            m_pConfig->gameHasCoffeeBreak()
            && nCurrentTime > m_pConfig->gameCoffeeBreakStartUTCInSec()
            && nCurrentTime < m_pConfig->gameCoffeeBreakEndUTCInSec()
        ) {
            Log::info(TAG, "Game on coffeebreak");
            m_pConfig->scoreboard()->setServiceStatus(m_teamConf.id(), m_serviceConf.id(), ServiceStatusCell::SERVICE_COFFEEBREAK);
            return;
        }

        if (nCurrentTime > m_pConfig->gameEndUTCInSec()) {
            Log::warn(TAG, "Game ended (current time: " + std::to_string(nCurrentTime) + ")");
            return;
        };

        if (nCurrentTime < m_pConfig->gameStartUTCInSec()) {
            Log::warn(TAG, "Game started after: " + std::to_string(m_pConfig->gameStartUTCInSec() - nCurrentTime) + " seconds");
            m_pConfig->scoreboard()->setServiceStatus(m_teamConf.id(), m_serviceConf.id(), ServiceStatusCell::SERVICE_WAIT);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            continue;
        };

        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();

        // If there is more time left before the end of the game than the life of the flag,
        // then we establish a flag
        if (nCurrentTime < (m_pConfig->gameEndUTCInSec() - m_pConfig->flagTimeliveInMin()*60)) {
            Flag flag;
            flag.generateRandomFlag(m_pConfig->flagTimeliveInMin(), m_teamConf.id(), m_serviceConf.id());

            // int nExitCode2 = 
            // Log::ok(TAG, " runChecker: " + std::to_string(nExitCode));

            int nExitCode = this->runChecker(flag, "put");
            if (nExitCode == ServiceCheckerThread::CHECKER_CODE_UP) {
                // >>>>>>>>>>> service is UP <<<<<<<<<<<<<<
                Log::ok(TAG, " => service is up");
                m_pConfig->scoreboard()->incrementFlagsPuttedAndServiceUp(flag);
            } else if (nExitCode == ServiceCheckerThread::CHECKER_CODE_CORRUPT) {
                // >>>>>>>>>>> service is CORRUPT <<<<<<<<<<<<<<
                Log::warn(TAG, " => service is corrupt ");
                m_pConfig->scoreboard()->insertFlagPutFail(flag, ServiceStatusCell::SERVICE_CORRUPT, "corrupt");
                // m_pConfig->scoreboard()->setServiceStatus(m_teamConf.id(), m_serviceConf.id(), ServiceStatusCell::SERVICE_CORRUPT);
                // m_pConfig->scoreboard()->updateScore(flag.teamId(), flag.serviceId());
            } else if (nExitCode == ServiceCheckerThread::CHECKER_CODE_MUMBLE) {
                // >>>>>>>>>>> service is MUMBLE <<<<<<<<<<<<<<
                // m_pConfig->storage()->insertFlagPutFail(flag, "mumble_1");
                Log::warn(TAG, " => service is mumble (1) ");
                m_pConfig->scoreboard()->insertFlagPutFail(flag, ServiceStatusCell::SERVICE_MUMBLE, "mumble_1");
                // m_pConfig->scoreboard()->setServiceStatus(m_teamConf.id(), m_serviceConf.id(), ServiceStatusCell::SERVICE_MUMBLE);
                // m_pConfig->scoreboard()->updateScore(flag.teamId(), flag.serviceId());
            } else if (nExitCode == ServiceCheckerThread::CHECKER_CODE_DOWN) {
                // >>>>>>>>>>> service is DOWN <<<<<<<<<<<<<<
                m_pConfig->scoreboard()->insertFlagPutFail(flag, ServiceStatusCell::SERVICE_DOWN, "down");
                Log::warn(TAG, " => service is down ");
                // m_pConfig->scoreboard()->setServiceStatus(m_teamConf.id(), m_serviceConf.id(), ServiceStatusCell::SERVICE_DOWN);
                // m_pConfig->scoreboard()->updateScore(flag.teamId(), flag.serviceId());
            } else if (nExitCode == ServiceCheckerThread::CHECKER_CODE_SHIT) {
                // >>>>>>>>>>> checker is SHIT <<<<<<<<<<<<<<
                m_pConfig->scoreboard()->insertFlagPutFail(flag, ServiceStatusCell::SERVICE_SHIT, "shit");
                Log::err(TAG, " => checker is shit ");
                // m_pConfig->scoreboard()->setServiceStatus(m_teamConf.id(), m_serviceConf.id(), ServiceStatusCell::SERVICE_SHIT);
                // m_pConfig->scoreboard()->updateScore(flag.teamId(), flag.serviceId());
            } else {
                m_pConfig->scoreboard()->insertFlagPutFail(flag, ServiceStatusCell::SERVICE_SHIT, "internal_error");
                // m_pConfig->storage()->insertFlagPutFail(flag, "internal_error");
                Log::err(TAG, " => runChecker - wrong code return");
                // m_pConfig->scoreboard()->updateScore(flag.teamId(), flag.serviceId());
            }
        } else {
            Log::info(TAG, "Game ended after: " + std::to_string(m_pConfig->gameEndUTCInSec() - nCurrentTime) + " sec");
            // check some service status or just update to UP (Ha-Ha I'm the real evil!)
        }

        std::vector<Flag> vEndedFlags = m_pConfig->scoreboard()->outdatedFlagsLive(m_teamConf.id(), m_serviceConf.id());

        for (unsigned int i = 0; i < vEndedFlags.size(); i++) {
            Flag outdatedFlag = vEndedFlags[i];
            m_pConfig->scoreboard()->removeFlagLive(outdatedFlag);

            if (outdatedFlag.teamStole() != "") {
                continue;
            } else {
                // nobody stole outdatedFlag
                int nCheckExitCode = this->runChecker(outdatedFlag, "check");
                if (nCheckExitCode != ServiceCheckerThread::CHECKER_CODE_UP) {
                    // service is not up
                    m_pConfig->storage()->insertFlagCheckFail(outdatedFlag, "code_" + std::to_string(nCheckExitCode));
                } else {
                    // service is up
                    // TODO: only if last time (== flag time live) was up
                    if (!m_pConfig->storage()->isSomebodyStole(outdatedFlag)) {
                        m_pConfig->scoreboard()->incrementDefenceScore(outdatedFlag);
                    }
                }
            }
        }
        
        // TODO just update SLA ?
        end = std::chrono::system_clock::now();

        int elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
        int ms_sleep = m_serviceConf.timeSleepBetweenRunScriptsInSec()*1000;
        Log::info(TAG, "Elapsed milliseconds: " + std::to_string(elapsed_milliseconds) + "ms");
        std::this_thread::sleep_for(std::chrono::milliseconds(ms_sleep - elapsed_milliseconds));
    }
}
