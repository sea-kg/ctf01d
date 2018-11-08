#ifndef RAM_STORAGE_H
#define RAM_STORAGE_H

#include <istorage.h>
#include <model_scoreboard.h>

class RamStorage : public IStorage {
    public:
        RamStorage(ModelScoreboard *pScoreboard, int nGameStartUTCInSec, int nGameEndUTCInSec);

        void setDatabasePath(const std::string &sDatabasePath);

        // IStorage
        virtual std::string type();
        virtual bool applyConfigFromFile(const std::string &sFilePath, 
            std::vector<ModelTeamConf> &vTeamsConf, 
            std::vector<ModelServiceConf> &vServicesConf);
        virtual void clean();
        virtual void addLiveFlag(const ModelTeamConf &teamConf, const ModelServiceConf &serviceConf, const ModelFlag &sFlag);
        virtual std::vector<ModelFlag> endedFlags(const ModelTeamConf &teamConf, const ModelServiceConf &serviceConf);
        virtual void updateFlag(const ModelTeamConf &teamConf, const ModelServiceConf &serviceConf, const ModelFlag &sFlag);
        virtual void updateScoreboard(const ModelTeamConf &teamConf, const ModelServiceConf &serviceConf);
        virtual bool findFlagByValue(const std::string &sFlag, ModelFlag &resultFlag);
        virtual bool updateTeamStole(const std::string &sFlag, int nTeamNum);
        virtual void removeFlag(ModelFlag &flag);
        virtual void moveToArchive(ModelFlag &flag);

    private:
        std::string TAG;
        std::mutex m_mutexFlags;

        ModelScoreboard *m_pScoreboard;
        int m_nGameStartUTCInSec;
        int m_nGameEndUTCInSec;

        std::string m_sDatabasePath;
        std::vector<ModelFlag *> m_vFlagLives;
        std::vector<ModelFlag *> m_vFlagArchive;
        std::map<std::string, ModelFlag *> m_mapFlagLive;
};

#endif // RAM_STORAGE_H
