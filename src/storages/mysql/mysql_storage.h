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

#ifndef MYSQL_STORAGE_H
#define MYSQL_STORAGE_H

#include <storages.h>
#include <map>
#include <mysql/mysql.h>

class MySqlStorage : public Storage {
    public:
        MySqlStorage(int nGameStartUTCInSec, int nGameEndUTCInSec);
        static std::string type() { return "mysql"; };

        // Storage
        virtual bool applyConfigFromYaml(
            WsjcppYaml &yamlConfig,
            const std::string &sHost
        ) override;
        virtual void clean() override;
        virtual void insertFlagLive(const Ctf01dFlag &flag) override;
        virtual std::vector<Ctf01dFlag> listOfLiveFlags() override;
        virtual void insertToArchive(Ctf01dFlag &flag) override;
        virtual void insertToFlagsStolen(const Ctf01dFlag &flag, const std::string &sTeamId, int nPoints) override;

        virtual bool isAlreadyStole(const Ctf01dFlag &flag, const std::string &sTeamId) override;
        virtual bool isSomebodyStole(const Ctf01dFlag &flag) override;

        virtual int numberOfFlagSuccessPutted(const std::string &sTeamId, const std::string &sServiceId) override; // TODO remove
        virtual int numberOfStolenFlagsForService(const std::string &sServiceId) override;

        virtual std::vector<Ctf01dFlag> outdatedFlags(const std::string &sTeamId, const std::string &sServiceId) override;
        virtual void updateFlag(const std::string &sTeamId, const std::string &sServiceId, const Ctf01dFlag &sFlag) override;

        virtual int getStollenFlags(const std::string &sTeamId, const std::string &sServiceId) override;
        virtual int getStollenPoints(const std::string &sTeamId, const std::string &sServiceId) override;
        virtual bool findFlagByValue(const std::string &sFlag, Ctf01dFlag &resultFlag) override;
        virtual void deleteFlagLive(const Ctf01dFlag &flag) override;

    private:
        std::string TAG;
        // int m_nGameStartUTCInSec;
        // int m_nGameEndUTCInSec;
        std::string m_sGameStartUTCInMS;
        std::string m_sGameEndUTCInMS;

        bool checkAndInstall(MYSQL *pConn);

        // database
        std::map<std::string, MYSQL *> m_mapConnections;
        MYSQL *getDatabaseConnection();

        std::string m_sDatabaseHost;
        std::string m_sDatabaseName;
        std::string m_sDatabaseUser;
        std::string m_sDatabasePass;
        int m_nDatabasePort;
};

#endif // MYSQL_STORAGE_H