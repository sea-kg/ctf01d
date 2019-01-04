#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include <vector>
#include <flag.h>
#include <service.h>
#include <team.h>

class Storage {
public:
    // Storage(const std::string &sType);
    static std::string type() { return "unknown"; };

    virtual bool applyConfigFromFile(
        const std::string &sFilePath, 
        std::vector<Team> &vTeamsConf,
        std::vector<Service> &vServicesConf
    ) = 0;

    virtual void clean() = 0;

    // add new live flag
    virtual void insertFlagLive(const Flag &sFlag) = 0;

    virtual std::vector<Flag> listOfLiveFlags() = 0;

    // when flag put fail
    virtual void insertFlagPutFail(const Flag &flag, const std::string &sReason) = 0;

    // when flag check fail
    virtual void insertFlagCheckFail(const Flag &flag, const std::string &sReason) = 0;

    // add flag attempt
    virtual void insertFlagAttempt(const std::string &sTeamId, const std::string &sFlag) = 0;

    // count of flag attempts for init scoreboard
    virtual int numberOfFlagAttempts(const std::string &sTeamId) = 0;

    virtual int numberOfFlagSuccessPutted(const std::string &sTeamId, const std::string &sServiceId) = 0;

    // move flag to archive
    virtual void insertToArchive(Flag &flag) = 0;

    // list of flags with ended if server up and check another flag lost on down
    virtual std::vector<Flag> outdatedFlags(const Team &teamConf, const Service &service) = 0;

    // update flag status and update scoreboard table for team 
    virtual void updateFlag(const Team &teamConf, const Service &serviceConf, const Flag &sFlag) = 0;

    // return defence value by team and by service
    virtual int defenceValue(const std::string &sTeamId, const std::string &sServiceId) = 0;

    // return defence value by team and by service
    virtual int attackValue(const std::string &sTeamId, const std::string &sServiceId) = 0;

    // DEPRECATED
    // find flag
    virtual bool findFlagByValue(const std::string &sFlag, Flag &resultFlag) = 0;

    // update team stole with check
    virtual bool updateTeamStole(const std::string &sFlag, const std::string &sTeamId) = 0;

    // remove flag
    virtual void deleteFlagLive(const Flag &flag) = 0;
};

#endif // STORAGE_H