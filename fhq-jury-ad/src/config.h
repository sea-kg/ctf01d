#ifndef CONFIG_H
#define CONFIG_H

#include <istorage.h>
#include <model_scoreboard.h>
#include <model_team_conf.h>
#include <model_service_conf.h>

class JuryConfiguration {
	public:
		JuryConfiguration(const std::string &sWorkspaceDir);
		bool applyConfig(bool bLazyStart);

		// basic configuration
		int flagTimeliveInMin();
		void setFlagTimeliveInMin(int nMinutes);

		// services configuration
		std::vector<ModelServiceConf> &servicesConf();
		ModelServiceConf *findOrCreateServiceConf(int nServiceNum);
		void setServiceName(int nServiceNum, const std::string &sServiceName);
		void setServiceScriptPath(int nServiceNum, const std::string &sServiceName);
		void setServiceEnabled(int nServiceNum, bool bEnabled);
		void setServiceScriptWait(int nServiceNum, int nScriptWait);
		void setServiceTimeSleep(int nServiceNum, int nScriptWait);

		// teams configuration
		std::vector<ModelTeamConf> &teamsConf();
		ModelTeamConf *findOrCreateTeamConf(int nTeamNum);
		void setTeamName(int nTeamNum, const std::string &sTeamName);
		void setTeamLogo(int nTeamNum, const std::string &sTeamLogo);
		void setTeamIpAddress(int nTeamNum, const std::string &sTeamIpAddress);
		void setTeamActive(int nTeamNum, bool bEnabled);

		// scoreboard configuration
		void setScoreboardPort(int nPort);
		int scoreboardPort();
		void setScoreboardHtmlFolder(const std::string &sHtmlFolder);
		std::string scoreboardHtmlFolder();
		void setScoreboardRandom(bool bRandom);
		bool scoreboardRandom();

		// game configuration
		std::string gameName();
		void setGameName(const std::string &sGameName);
		void setGameStart(const std::string &sGameStart);
		int gameStartUTCInSec();
		void setGameEnd(const std::string &sGameEnd);
		int gameEndUTCInSec();

		// storage configuration
		void setUseStorage(const std::string &sUseStorage);
		IStorage *storage();
		void setStorage(IStorage *pStorage);
		ModelScoreboard *scoreboard();

	private:
		std::string TAG;
		IStorage *m_pStorage;
		ModelScoreboard *m_pScoreboard;
		int m_nScoreboardPort;
		std::string m_sScoreboardHtmlFolder;
		bool m_bScoreboardRandom;

		std::string m_sWorkspaceDir;
		std::string m_sUseStorage;
		int m_nFlagTimeliveInMin;
		std::string m_sGameID;
		std::string m_sGameName;
		std::string m_sGameStart;
		std::string m_sGameEnd;
		int m_nGameStartUTCInSec; // UTC in seconds
		int m_nGameEndUTCInSec; // UTC in seconds
		std::vector<ModelTeamConf> m_vTeamsConf;
		std::vector<ModelServiceConf> m_vServicesConf;

};

#endif // CONFIG
