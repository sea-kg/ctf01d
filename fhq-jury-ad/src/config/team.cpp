#include "team.h"

Team::Team(){

}

void Team::setId(const std::string &sTeamId){
    m_sTeamID = sTeamId;
}

std::string Team::id() const {
    return m_sTeamID;
}

void Team::setName(const std::string &sName){
    m_sName = sName;
}

std::string Team::name() const {
    return m_sName;
}

void Team::setIpAddress(const std::string &sIpAddress){
    m_sIpAddress = sIpAddress;
}

std::string Team::ipAddress() const {
    return m_sIpAddress;
}

void Team::setActive(bool bActive){
    m_bActive = bActive;
}

bool Team::isActive() const {
    return m_bActive;
}

void Team::setLogo(const std::string &sLogo){
    m_sLogo = sLogo;
}

std::string Team::logo() const {
    return m_sLogo;
}