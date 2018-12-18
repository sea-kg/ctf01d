#ifndef CONFIG_H
#define CONFIG_H

#include <istorage.h>
#include <model_scoreboard.h>
#include <model_team_conf.h>
#include <model_service_conf.h>

class Config {
	public:
		Config(const std::string &sWorkspaceDir);
		bool applyConfig(bool bLazyStart);

		// services configuration
		std::vector<ModelServiceConf> &servicesConf();

		// teams configuration
		std::vector<ModelTeamConf> &teamsConf();

		// scoreboard configuration
		int scoreboardPort();
		std::string scoreboardHtmlFolder();
		bool scoreboardRandom();

		// game configuration
		std::string gameId();
		std::string gameName();
		int flagTimeliveInMin();
		int gameStartUTCInSec();
		int gameEndUTCInSec();

		// storage configuration
		IStorage *storage();
		void setStorage(IStorage *pStorage);
		ModelScoreboard *scoreboard();

	private:
		bool applyGameConf(bool bLazyStart);
		bool applyServerConf(bool bLazyStart);
		bool applyScoreboardConf(bool bLazyStart);
		bool applyTeamsConf(bool bLazyStart);
		bool applyCheckersConf(bool bLazyStart);

		bool fileExists(const std::string &sFilename);
        bool dirExists(const std::string &sDirname);
		std::vector<std::string> listOfDirs(const std::string &sDirname);
		std::vector<std::string> listOfFiles(const std::string &sDirname);

		std::string TAG;
		IStorage *m_pStorage;
		ModelScoreboard *m_pScoreboard;
		int m_nScoreboardPort;
		std::string m_sScoreboardHtmlFolder;
		bool m_bScoreboardRandom;
		std::string m_sWorkspaceDir;
		std::string m_sUseStorage;

		// game conf
		int m_nFlagTimeliveInMin;
		std::string m_sGameId;
		std::string m_sGameName;
		std::string m_sGameStart;
		std::string m_sGameEnd;
		int m_nGameStartUTCInSec; // UTC in seconds
		int m_nGameEndUTCInSec; // UTC in seconds
		
		std::vector<ModelTeamConf> m_vTeamsConf;
		std::vector<ModelServiceConf> m_vServicesConf;

};

#endif // CONFIG
