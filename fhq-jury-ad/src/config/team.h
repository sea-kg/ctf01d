#ifndef TEAM_H
#define TEAM_H

#include <string>

class Team {
    public:
        Team();
        
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

#endif // TEAM_H
