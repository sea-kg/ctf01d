#ifndef SERVICE_H
#define SERVICE_H

#include <string>

class Service {
    public:
        Service();
        
        void setId(const std::string &sServiceId);
        std::string id() const;

        void setName(const std::string &sName);
        std::string name() const;

        void setScriptPath(const std::string &sScriptPath);
        std::string scriptPath() const;

        void setScriptDir(const std::string &sScriptDir);
        std::string scriptDir() const;

        void setEnabled(bool bEnabled);
        bool isEnabled() const;

        void setScriptWaitInSec(int nSec);
        int scriptWaitInSec() const;

        void setTimeSleepBetweenRunScriptsInSec(int nSec);
        int timeSleepBetweenRunScriptsInSec() const;

    private:
        int m_nNum;
        bool m_bEnabled;
        int m_nScriptWaitInSec;
        int m_nTimeSleepBetweenRunScriptsInSec;
        std::string m_sID;
        std::string m_sName;
        std::string m_sScriptPath;
        std::string m_sScriptDir;
};

#endif // SERVICE_H
