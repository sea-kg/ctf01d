/**********************************************************************************
 *          Project
 *  _______ _________ _______  _______  __    ______
 * (  ____ \\__   __/(  ____ \(  __   )/  \  (  __  \
 * | (    \/   ) (   | (    \/| (  )  |\/) ) | (  \  )
 * | |         | |   | (__    | | /   |  | | | |   ) |
 * | |         | |   |  __)   | (/ /) |  | | | |   | |
 * | |         | |   | (      |   / | |  | | | |   ) |
 * | (____/\   | |   | )      |  (__) |__) (_| (__/  )
 * (_______/   )_(   |/       (_______)\____/(______/
 *
 * MIT License
 * Copyright (c) 2018-2023 Evgenii Sopov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ***********************************************************************************/

#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include <vector>
#include <employ_flags.h>
#include <wsjcpp_yaml.h>

class Storage {
public:
    // Storage(const std::string &sType);
    static std::string type() { return "unknown"; };

    virtual bool applyConfigFromYaml(
        WsjcppYaml &yamlConfig,
        const std::string &sHost
    ) = 0;

    virtual void clean() = 0;

    // add new live flag
    virtual void insertFlagLive(const Ctf01dFlag &sFlag) = 0;

    virtual std::vector<Ctf01dFlag> listOfLiveFlags() = 0;

    virtual int numberOfFlagSuccessPutted(const std::string &sTeamId, const std::string &sServiceId) = 0;

    // how much stolen flags from specify service
    virtual int numberOfStolenFlagsForService(const std::string &sServiceId) = 0;

    // move flag to archive
    virtual void insertToArchive(Ctf01dFlag &flag) = 0;

    // copy flag to defence
    virtual void insertToFlagsStolen(const Ctf01dFlag &flag, const std::string &sTeamId, int nPoints) = 0;

    virtual bool isAlreadyStole(const Ctf01dFlag &flag, const std::string &sTeamId) = 0;
    virtual bool isSomebodyStole(const Ctf01dFlag &flag) = 0;

    // list of flags with ended if server up and check another flag lost on down
    virtual std::vector<Ctf01dFlag> outdatedFlags(const std::string &sTeamId, const std::string &sServiceId) = 0;

    // update flag status and update scoreboard table for team 
    virtual void updateFlag(const std::string &sTeamId, const std::string &sServiceId, const Ctf01dFlag &sFlag) = 0;

    // return defence value by team and by service
    virtual int getStollenFlags(const std::string &sTeamId, const std::string &sServiceId) = 0;
    virtual int getStollenPoints(const std::string &sTeamId, const std::string &sServiceId) = 0;

    // DEPRECATED
    // find flag
    virtual bool findFlagByValue(const std::string &sFlag, Ctf01dFlag &resultFlag) = 0;

    // remove flag
    virtual void deleteFlagLive(const Ctf01dFlag &flag) = 0;
};

#endif // STORAGE_H