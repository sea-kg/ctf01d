#ifndef FILE_STORAGE_H
#define FILE_STORAGE_H

#include <storages.h>
#include <model_scoreboard.h>

class FileStorage : public Storage {
    public:
        FileStorage(ModelScoreboard *pScoreboard, int nGameStartUTCInSec, int nGameEndUTCInSec);
        static std::string type() { return "file"; };

        // Storage
        virtual bool applyConfigFromFile(const std::string &sFilePath, 
            std::vector<ModelTeamConf> &vTeamsConf, 
            std::vector<ModelServiceConf> &vServicesConf);
        virtual void clean();
        virtual void addLiveFlag(const ModelTeamConf &teamConf, const ModelServiceConf &serviceConf, const Flag &sFlag);
        virtual std::vector<Flag> endedFlags(const ModelTeamConf &teamConf, const ModelServiceConf &serviceConf);
        virtual void updateFlag(const ModelTeamConf &teamConf, const ModelServiceConf &serviceConf, const Flag &sFlag);
        virtual void updateScoreboard(const ModelTeamConf &teamConf, const ModelServiceConf &serviceConf);
        virtual bool findFlagByValue(const std::string &sFlag, Flag &resultFlag);
        virtual bool updateTeamStole(const std::string &sFlag, const std::string &sTeamId);
        virtual void removeFlag(Flag &flag);
        virtual void moveToArchive(Flag &flag);

    private:
        std::string TAG;
        std::mutex m_mutexFlags;

        ModelScoreboard *m_pScoreboard;
        int m_nGameStartUTCInSec;
        int m_nGameEndUTCInSec;

        std::string m_sDatabasePath;
        std::vector<Flag *> m_vFlagLives;
        std::vector<Flag *> m_vFlagArchive;
        std::map<std::string, Flag *> m_mapFlagLive;
};

#endif // FILE_STORAGE_H