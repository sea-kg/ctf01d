#ifndef EMPLOY_CONFIG_H
#define EMPLOY_CONFIG_H

#include <wsjcpp_employees.h>

// ----------------------------------------------------------------------

class Ctf01dServiceDef {
    public:
        Ctf01dServiceDef();
        
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

// ----------------------------------------------------------------------

class Ctf01dTeamDef {
    public:
        Ctf01dTeamDef();
        
        void setId(const std::string &sId);
        std::string id() const;

        void setName(const std::string &sName);
        std::string name() const;

        void setIpAddress(const std::string &sIpAddress);
        std::string ipAddress() const;

        void setActive(bool bActive);
        bool isActive() const;

        void setLogo(const std::string &sLogo);
        std::string logo() const;

    private:
        bool m_bActive;
        std::string m_sTeamID;
        std::string m_sName;
        std::string m_sIpAddress;
        std::string m_sLogo;
};

// ----------------------------------------------------------------------

class EmployConfig : public WsjcppEmployBase {
    public:
        EmployConfig();
        static std::string name() { return "EmployConfig"; }
        virtual bool init() override;
        virtual bool deinit() override;
        void setWorkDir(const std::string &sWorkDir);
        std::string getWorkDir();

    private:
        std::string TAG;
        std::string m_sWorkDir;
};

#endif // EMPLOY_CONFIG_H
