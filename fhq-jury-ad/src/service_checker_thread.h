#ifndef SERVICE_CHECKER_THREAD_H
#define SERVICE_CHECKER_THREAD_H

#include "config.h"
#include <model_scoreboard.h>
#include <model_team_conf.h>
#include <model_service_conf.h>

// struct 

class ServiceCheckerThread {
	public:
		// enum for checker return code
        static int CHECKER_CODE_UP;
        static int CHECKER_CODE_CORRUPT;
		static int CHECKER_CODE_MUMBLE;
		static int CHECKER_CODE_DOWN;
		static int CHECKER_CODE_SHIT;

		ServiceCheckerThread(Config *pConfig, const ModelTeamConf &teamConf, const ModelServiceConf &serviceConf);
		void start();
		void run();

	private:
		std::string TAG;
		pthread_t m_checkerThread;
		Config *m_pConfig;
		ModelTeamConf m_teamConf;
		ModelServiceConf m_serviceConf;

		int runChecker(ModelFlag &flag, const std::string &sCommand);
		// int runChecker(ModelFlag &flag, const std::string &sCommand);
		
		// void run();
};

#endif // SERVICE_CHECKER_THREAD_H
