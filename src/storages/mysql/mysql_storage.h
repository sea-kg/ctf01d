#ifndef MYSQL_STORAGE_H
#define MYSQL_STORAGE_H

#include <storages.h>
#include <map>
#include <mysql/mysql.h>

class MySqlStorage : public Storage {
    public:
        MySqlStorage(int nGameStartUTCInSec, int nGameEndUTCInSec);
        static std::string type() { return "mysql"; };

        // Storage
        virtual bool applyConfigFromYaml(
            WsjcppYaml &yamlConfig
        );
        virtual void clean();
        virtual void insertFlagLive(const Flag &flag);
        virtual std::vector<Flag> listOfLiveFlags();
        virtual void insertFlagPutFail(const Flag &flag, const std::string &sReason);
        virtual void insertFlagCheckFail(const Flag &flag, const std::string &sReason);
        virtual void insertFlagAttempt(const std::string &sTeamId, const std::string &sFlag);
        virtual int numberOfFlagAttempts(const std::string &sTeamId);
        virtual void insertToArchive(Flag &flag);
        virtual void insertToFlagsDefence(const Flag &flag, int nPoints);
        virtual void insertToFlagsStolen(const Flag &flag, const std::string &sTeamId, int nPoints);
        
        virtual bool isAlreadyStole(const Flag &flag, const std::string &sTeamId);
        virtual bool isSomebodyStole(const Flag &flag);

        virtual int numberOfFlagSuccessPutted(const std::string &sTeamId, const std::string &sServiceId); // TODO remove
        virtual int numberOfDefenceFlagForService(const std::string &sServiceId);
        virtual int numberOfStolenFlagsForService(const std::string &sServiceId);

        virtual std::vector<Flag> outdatedFlags(const std::string &sTeamId, const std::string &sServiceId);
        virtual void updateFlag(const std::string &sTeamId, const std::string &sServiceId, const Flag &sFlag);
        
        virtual int getDefenceFlags(const std::string &sTeamId, const std::string &sServiceId);
        virtual int getDefencePoints(const std::string &sTeamId, const std::string &sServiceId);

        virtual int getStollenFlags(const std::string &sTeamId, const std::string &sServiceId);
        virtual int getStollenPoints(const std::string &sTeamId, const std::string &sServiceId);
        virtual bool findFlagByValue(const std::string &sFlag, Flag &resultFlag);
        virtual bool updateTeamStole(const std::string &sFlag, const std::string &sTeamId);
        virtual void deleteFlagLive(const Flag &flag);

    private:
        std::string TAG;
        // int m_nGameStartUTCInSec;
        // int m_nGameEndUTCInSec;
        std::string m_sGameStartUTCInMS;
        std::string m_sGameEndUTCInMS;

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