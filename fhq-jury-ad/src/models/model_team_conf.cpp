#include "model_team_conf.h"

ModelTeamConf::ModelTeamConf(){

}

void ModelTeamConf::setId(const std::string &sTeamId){
    m_sTeamID = sTeamId;
}

std::string ModelTeamConf::id() const {
    return m_sTeamID;
}

void ModelTeamConf::setNum(int nNum){
    m_nNum = nNum;
}

int ModelTeamConf::num() const {
    return m_nNum;
}

void ModelTeamConf::setName(const std::string &sName){
    m_sName = sName;
}

std::string ModelTeamConf::name() const {
    return m_sName;
}

void ModelTeamConf::setIpAddress(const std::string &sIpAddress){
    m_sIpAddress = sIpAddress;
}

std::string ModelTeamConf::ipAddress() const {
    return m_sIpAddress;
}

void ModelTeamConf::setActive(bool bActive){
    m_bActive = bActive;
}

bool ModelTeamConf::isActive() const {
    return m_bActive;
}

void ModelTeamConf::setLogo(const std::string &sLogo){
    m_sLogo = sLogo;
}

std::string ModelTeamConf::logo() const {
    return m_sLogo;
}