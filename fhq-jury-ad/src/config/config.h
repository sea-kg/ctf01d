#ifndef CONFIG_H
#define CONFIG_H

#include <storages.h>
#include <scoreboard.h>
#include <team.h>
#include <service.h>
#include <wsjcpp_yaml.h>

class Config {
	public:
		Config(const std::string &sWorkspaceDir);
		bool applyConfig();

		// services configuration
		std::vector<Service> &servicesConf();

		// teams configuration
		std::vector<Team> &teamsConf();

		// scoreboard configuration
		int scoreboardPort();
		std::string scoreboardHtmlFolder();
		bool scoreboardRandom();

		// game configuration
		std::string gameId();
		std::string gameName();
		int flagTimeliveInMin();
		int bacisCostsStolenFlagInPoints();
		int gameStartUTCInSec();
		int gameEndUTCInSec();

		bool gameHasCoffeeBreak();
		int gameCoffeeBreakStartUTCInSec();
		int gameCoffeeBreakEndUTCInSec();

		// storage configuration
		Storage *storage();
		void setStorage(Storage *pStorage);
		Scoreboard *scoreboard();

	private:
		bool applyGameConf(WsjcppYaml &yamlConfig);
		bool applyServerConf(WsjcppYaml &yamlConfig);
		bool applyScoreboardConf(WsjcppYaml &yamlConfig);
		bool applyCheckersConf(WsjcppYaml &yamlConfig);
		bool readTeamsConf(WsjcppYaml &yamlConfig);

		std::string TAG;
		Storage *m_pStorage;
		Scoreboard *m_pScoreboard;
		int m_nScoreboardPort;
		std::string m_sScoreboardHtmlFolder;
		bool m_bScoreboardRandom;
		std::string m_sWorkspaceDir;
		std::string m_sUseStorage;
		

		// game conf
		int m_nFlagTimeliveInMin;
		int m_nBacisCostsStolenFlagInPoints;
		std::string m_sGameId;
		std::string m_sGameName;
		std::string m_sGameStart;
		std::string m_sGameEnd;
		int m_nGameStartUTCInSec; // UTC in seconds
		int m_nGameEndUTCInSec; // UTC in seconds

		bool m_bHasCoffeeBreak;
		std::string m_sGameCoffeeBreakStart;
		std::string m_sGameCoffeeBreakEnd;
		int m_nGameCoffeeBreakStartUTCInSec; // UTC in seconds
		int m_nGameCoffeeBreakEndUTCInSec; // UTC in seconds
		
		
		std::vector<Team> m_vTeamsConf;
		std::vector<Service> m_vServicesConf;

};

#endif // CONFIG
