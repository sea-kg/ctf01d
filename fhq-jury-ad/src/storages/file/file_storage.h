#ifndef FILE_STORAGE_H
#define FILE_STORAGE_H

#include <storages.h>
#include <scoreboard.h>

class FileStorage : public Storage {
    public:
        FileStorage(Scoreboard *pScoreboard, int nGameStartUTCInSec, int nGameEndUTCInSec);
        static std::string type() { return "file"; };

        // Storage
        virtual bool applyConfigFromFile(const std::string &sFilePath, 
            std::vector<Team> &vTeamsConf, 
            std::vector<Service> &vServicesConf);
        virtual void clean();
        virtual void addLiveFlag(const Team &teamConf, const Service &serviceConf, const Flag &sFlag);
        virtual std::vector<Flag> endedFlags(const Team &teamConf, const Service &serviceConf);
        virtual void updateFlag(const Team &teamConf, const Service &serviceConf, const Flag &sFlag);
        virtual void updateScoreboard(const Team &teamConf, const Service &serviceConf);
        virtual bool findFlagByValue(const std::string &sFlag, Flag &resultFlag);
        virtual bool updateTeamStole(const std::string &sFlag, const std::string &sTeamId);
        virtual void removeFlag(Flag &flag);
        virtual void moveToArchive(Flag &flag);

    private:
        std::string TAG;
        std::mutex m_mutexFlags;

        Scoreboard *m_pScoreboard;
        int m_nGameStartUTCInSec;
        int m_nGameEndUTCInSec;

        std::string m_sDatabasePath;
        std::vector<Flag *> m_vFlagLives;
        std::vector<Flag *> m_vFlagArchive;
        std::map<std::string, Flag *> m_mapFlagLive;
};

#endif // FILE_STORAGE_H