#ifndef MYSQL_STORAGE_H
#define MYSQL_STORAGE_H

#include <storages.h>
#include <scoreboard.h>
#include <config.h>
#include <map>
#include <mysql/mysql.h>

class MySqlStorage : public Storage {
    public:
        MySqlStorage(Scoreboard *pScoreboard, int nGameStartUTCInSec, int nGameEndUTCInSec);
        static std::string type() { return "mysql"; };

        // Storage
        virtual bool applyConfigFromFile(const std::string &sFilePath, 
            std::vector<Team> &vTeamsConf, 
            std::vector<Service> &vServicesConf);
        virtual void clean();
        virtual void addLiveFlag(const Team &teamConf, const Service &service, const Flag &sFlag);
        virtual std::vector<Flag> endedFlags(const Team &teamConf, const Service &serviceConf);
        virtual void updateFlag(const Team &teamConf, const Service &serviceConf, const Flag &sFlag);
        virtual void updateScoreboard(const Team &teamConf, const Service &service);
        virtual bool findFlagByValue(const std::string &sFlag, Flag &resultFlag);
        virtual bool updateTeamStole(const std::string &sFlag, const std::string &sTeamId);
        virtual void removeFlag(Flag &flag);
        virtual void moveToArchive(Flag &flag);

    private:
        std::string TAG;
        Scoreboard *m_pScoreboard;
        int m_nGameStartUTCInSec;
        int m_nGameEndUTCInSec;

        bool checkAndInstall(MYSQL *pConn);

        // database
        std::map<std::string, MYSQL *> m_mapConnections;
        MYSQL *getDatabaseConnection();

        std::string m_sDatabaseHost;
        std::string m_sDatabaseName;
        std::string m_sDatabaseUser;
        std::string m_sDatabasePass;
        int m_nDatabasePort;
};

#endif // MYSQL_STORAGE_H