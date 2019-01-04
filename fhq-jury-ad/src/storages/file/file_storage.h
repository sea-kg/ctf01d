#ifndef FILE_STORAGE_H
#define FILE_STORAGE_H

#include <storages.h>

class FileStorage : public Storage {
    public:
        FileStorage(int nGameStartUTCInSec, int nGameEndUTCInSec);
        static std::string type() { return "file"; };

        // Storage
        virtual bool applyConfigFromFile(const std::string &sFilePath, 
            std::vector<Team> &vTeamsConf, 
            std::vector<Service> &vServicesConf);
        virtual void clean();
        virtual void insertFlagLive(const Flag &flag);
        virtual std::vector<Flag> listOfLiveFlags();
        virtual void insertFlagPutFail(const Flag &flag, const std::string &sReason);
        virtual void insertFlagCheckFail(const Flag &flag, const std::string &sReason);
        virtual void insertFlagAttempt(const std::string &sTeamId, const std::string &sFlag);
        virtual int numberOfFlagAttempts(const std::string &sTeamId);
        virtual void insertToArchive(Flag &flag);
        virtual int numberOfFlagSuccessPutted(const std::string &sTeamId, const std::string &sServiceId);
        virtual std::vector<Flag> outdatedFlags(const Team &teamConf, const Service &serviceConf);
        virtual void updateFlag(const Team &teamConf, const Service &serviceConf, const Flag &sFlag);
        virtual int defenceValue(const std::string &sTeamId, const std::string &sServiceId);
        virtual int attackValue(const std::string &sTeamId, const std::string &sServiceId);
        virtual bool findFlagByValue(const std::string &sFlag, Flag &resultFlag);
        virtual bool updateTeamStole(const std::string &sFlag, const std::string &sTeamId);
        virtual void deleteFlagLive(const Flag &flag);

    private:
        std::string TAG;
        std::mutex m_mutexFlags;

        int m_nGameStartUTCInSec;
        int m_nGameEndUTCInSec;

        std::string m_sDatabasePath;
        std::vector<Flag *> m_vFlagLives;
        std::vector<Flag *> m_vFlagArchive;
        std::map<std::string, Flag *> m_mapFlagLive;
};

#endif // FILE_STORAGE_H