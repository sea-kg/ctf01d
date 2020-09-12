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
            WsjcppYaml &yamlConfig,
            const std::string &sHost
        ) override;
        virtual void clean() override;
        virtual void insertFlagLive(const Ctf01dFlag &flag) override;
        virtual std::vector<Ctf01dFlag> listOfLiveFlags() override;
        virtual void insertFlagPutFail(const Ctf01dFlag &flag, const std::string &sReason) override;
        virtual void insertFlagCheckFail(const Ctf01dFlag &flag, const std::string &sReason) override;
        virtual void insertFlagAttempt(const std::string &sTeamId, const std::string &sFlag) override;
        virtual int numberOfFlagAttempts(const std::string &sTeamId) override;
        virtual void insertToArchive(Ctf01dFlag &flag) override;
        virtual void insertToFlagsDefence(const Ctf01dFlag &flag, int nPoints) override;
        virtual void insertToFlagsStolen(const Ctf01dFlag &flag, const std::string &sTeamId, int nPoints) override;
        
        virtual bool isAlreadyStole(const Ctf01dFlag &flag, const std::string &sTeamId) override;
        virtual bool isSomebodyStole(const Ctf01dFlag &flag) override;

        virtual int numberOfFlagSuccessPutted(const std::string &sTeamId, const std::string &sServiceId) override; // TODO remove
        virtual int numberOfDefenceFlagForService(const std::string &sServiceId) override;
        virtual int numberOfStolenFlagsForService(const std::string &sServiceId) override;

        virtual std::vector<Ctf01dFlag> outdatedFlags(const std::string &sTeamId, const std::string &sServiceId) override;
        virtual void updateFlag(const std::string &sTeamId, const std::string &sServiceId, const Ctf01dFlag &sFlag) override;
        
        virtual int getDefenceFlags(const std::string &sTeamId, const std::string &sServiceId) override;
        virtual int getDefencePoints(const std::string &sTeamId, const std::string &sServiceId) override;

        virtual int getStollenFlags(const std::string &sTeamId, const std::string &sServiceId) override;
        virtual int getStollenPoints(const std::string &sTeamId, const std::string &sServiceId) override;
        virtual bool findFlagByValue(const std::string &sFlag, Ctf01dFlag &resultFlag) override;
        virtual void deleteFlagLive(const Ctf01dFlag &flag) override;

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