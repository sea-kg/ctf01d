#ifndef MODEL_TEAM_CONF_H
#define MODEL_TEAM_CONF_H

#include <string>

class ModelTeamConf {
    public:
        ModelTeamConf();
        
        void setId(const std::string &sId);
        std::string id() const;

        void setNum(int nNum);
        int num() const;

        void setName(const std::string &sName);
        std::string name() const;

        void setIpAddress(const std::string &sIpAddress);
        std::string ipAddress() const;

        void setActive(bool bActive);
        bool isActive() const;

        void setLogo(const std::string &sLogo);
        std::string logo() const;

    private:
        int m_nNum;
        bool m_bActive;
        std::string m_sTeamID;
        std::string m_sName;
        std::string m_sIpAddress;
        std::string m_sLogo;
};

#endif // MODEL_TEAM_CONF_H
