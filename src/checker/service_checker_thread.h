#ifndef SERVICE_CHECKER_THREAD_H
#define SERVICE_CHECKER_THREAD_H

#include <scoreboard.h>
#include <employ_config.h>

// struct 

class ServiceCheckerThread {
	public:
		// enum for checker return code
        static int CHECKER_CODE_UP;
        static int CHECKER_CODE_CORRUPT;
		static int CHECKER_CODE_MUMBLE;
		static int CHECKER_CODE_DOWN;
		static int CHECKER_CODE_SHIT;

		ServiceCheckerThread(
			const Ctf01dTeamDef &teamConf,
			const Ctf01dServiceDef &serviceConf
		);
		void start();
		void run();

	private:
		std::string TAG;
		pthread_t m_checkerThread;
		EmployConfig *m_pConfig;
		Ctf01dTeamDef m_teamConf;
		Ctf01dServiceDef m_serviceConf;

		int runChecker(Ctf01dFlag &flag, const std::string &sCommand);
		// int runChecker(Flag &flag, const std::string &sCommand);
		
		// void run();
};

#endif // SERVICE_CHECKER_THREAD_H
