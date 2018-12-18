#ifndef MYSQL_STORAGE_H
#define MYSQL_STORAGE_H

#include <istorage.h>
#include <model_scoreboard.h>
#include <config.h>
#include <map>
#include <mysql/mysql.h>

class MySqlStorage : public IStorage {
    public:
        MySqlStorage(ModelScoreboard *pScoreboard, int nGameStartUTCInSec, int nGameEndUTCInSec);

        // IStorage
        virtual std::string type();
        virtual bool applyConfigFromFile(const std::string &sFilePath, 
            std::vector<ModelTeamConf> &vTeamsConf, 
            std::vector<ModelServiceConf> &vServicesConf);
        virtual void clean();
        virtual void addLiveFlag(const ModelTeamConf &teamConf, const ModelServiceConf &service, const ModelFlag &sFlag);
        virtual std::vector<ModelFlag> endedFlags(const ModelTeamConf &teamConf, const ModelServiceConf &serviceConf);
        virtual void updateFlag(const ModelTeamConf &teamConf, const ModelServiceConf &serviceConf, const ModelFlag &sFlag);
        virtual void updateScoreboard(const ModelTeamConf &teamConf, const ModelServiceConf &service);
        virtual bool findFlagByValue(const std::string &sFlag, ModelFlag &resultFlag);
        virtual bool updateTeamStole(const std::string &sFlag, const std::string &sTeamId);
        virtual void removeFlag(ModelFlag &flag);
        virtual void moveToArchive(ModelFlag &flag);

    private:
        std::string TAG;
        ModelScoreboard *m_pScoreboard;
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