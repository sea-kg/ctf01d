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

#include "employ_database.h"
#include <wsjcpp_core.h>
#include <employ_config.h>
#include <cmath>
#include <stdio.h>
#include <string>
#include <map>
#include <mutex>
#include <vector>

// ---------------------------------------------------------------------
// Ctf01dDatabaseFile

Ctf01dDatabaseFile::Ctf01dDatabaseFile(const std::string &sFilename, const std::string &sSqlCreateTable) {
    TAG = "Ctf01dDatabaseFile-" + sFilename;
    m_pDatabaseFile = nullptr;
    m_sFilename = sFilename;
    m_sSqlCreateTable = sSqlCreateTable;
    EmployConfig *pConfig = findWsjcppEmploy<EmployConfig>();
    std::string sDatabaseDir = pConfig->getWorkDir() + "/db";
    if (!WsjcppCore::dirExists(sDatabaseDir)) {
        !WsjcppCore::makeDir(sDatabaseDir);
    }
    m_sFileFullpath = sDatabaseDir + "/" + m_sFilename;
};

Ctf01dDatabaseFile::~Ctf01dDatabaseFile() {
    if (m_pDatabaseFile != nullptr) {
        sqlite3_close(m_pDatabaseFile);
    }
}

bool Ctf01dDatabaseFile::open() {
    // open connection to a DB
    int nRet = sqlite3_open_v2(
        m_sFileFullpath.c_str(),
        &m_pDatabaseFile,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
        NULL
    );
    if (nRet != SQLITE_OK) {
        WsjcppLog::throw_err(TAG, "Failed to open conn: " + std::to_string(nRet));
        return false;
    }
    // Run the SQL (convert the string to a C-String with c_str() )
    char *zErrMsg = 0;
    nRet = sqlite3_exec(m_pDatabaseFile, m_sSqlCreateTable.c_str(), 0, 0, &zErrMsg);
    if (nRet != SQLITE_OK) {
        WsjcppLog::throw_err(TAG, "Problem with create table: " + std::string(zErrMsg));
        return false;
    }
    WsjcppLog::ok(TAG, "Opened database file " + m_sFileFullpath);
    return true;
}

bool Ctf01dDatabaseFile::insert(std::string sSqlInsert) {
    // TODO mutex
    char *zErrMsg = 0;
    int nRet = sqlite3_exec(m_pDatabaseFile, sSqlInsert.c_str(), 0, 0, &zErrMsg);
    if (nRet != SQLITE_OK) {
        WsjcppLog::throw_err(TAG, "Problem with insert: " + std::string(zErrMsg) + "\n SQL-query: " + sSqlInsert);
        return false;
    }
    return true;
}

int Ctf01dDatabaseFile::selectCount(std::string sSqlSelectCount) {
    sqlite3_stmt* pQuery = nullptr;
    int ret = sqlite3_prepare_v2(m_pDatabaseFile, sSqlSelectCount.c_str(), -1, &pQuery, NULL);
    // prepare the statement
    if (ret != SQLITE_OK) {
        WsjcppLog::throw_err(TAG, "Failed to prepare select count: " + std::string(sqlite3_errmsg(m_pDatabaseFile)) + "\n SQL-query: " + sSqlSelectCount);
    }
    // step to 1st row of data
    ret = sqlite3_step(pQuery);
    if (ret != SQLITE_ROW) { // see documentation, this can return more values as success
        WsjcppLog::throw_err(TAG, "Failed to step for select count: " + std::string(sqlite3_errmsg(m_pDatabaseFile)) + "\n SQL-query: " + sSqlSelectCount);
    }
    int nRet = sqlite3_column_int(pQuery, 0);
    if (pQuery != nullptr) sqlite3_finalize(pQuery);
    return nRet;
}

// ---------------------------------------------------------------------
// EmployDatabase

REGISTRY_WJSCPP_SERVICE_LOCATOR(EmployDatabase)

EmployDatabase::EmployDatabase()
: WsjcppEmployBase(EmployDatabase::name(), {"EmployConfig"}) {
    TAG = EmployDatabase::name();
    m_pFlagsPutFails = nullptr;
    m_pFlagsAttempts = nullptr;
}

bool EmployDatabase::init() {
    int nRet = 0;
    if (SQLITE_OK != (nRet = sqlite3_initialize())) {
        WsjcppLog::throw_err(TAG, "Failed to initialize build-in sqlite3 library: " + std::to_string(nRet));
        return false;
    }
    WsjcppLog::ok(TAG, "Initialize build-in sqlite3 library");

    m_pFlagsPutFails = new Ctf01dDatabaseFile("flags_put_fails.db",
        "CREATE TABLE IF NOT EXISTS flags_put_fails ( "
        "  id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "  serviceid VARCHAR(50) NOT NULL, "
        "  flag_id VARCHAR(50) NOT NULL, "
        "  flag VARCHAR(36) NOT NULL, "
        "  teamid VARCHAR(50) NOT NULL, "
        "  date_start INTEGER NOT NULL,"
        "  date_end INTEGER NOT NULL,"
        "  reason VARCHAR(50) NOT NULL"
        ");"
    );
    if (!m_pFlagsPutFails->open()) {
        return false;
    }

    m_pFlagsAttempts = new Ctf01dDatabaseFile("flags_attempts.db",
        "CREATE TABLE IF NOT EXISTS flags_attempts ( "
        "  id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "  flag VARCHAR(36) NOT NULL, "
        "  teamid VARCHAR(50) NOT NULL, "
        "  dt INTEGER NOT NULL"
        ");"
    );
    if (!m_pFlagsAttempts->open()) {
        return false;
    }

    return true;
}

bool EmployDatabase::deinit() {
    WsjcppLog::info(TAG, "deinit");
    delete m_pFlagsPutFails;
    sqlite3_shutdown();
    return true;
}

void EmployDatabase::insertToFlagPutFail(const Ctf01dFlag &flag, const std::string &sReason) {
    std::string sQuery = "INSERT INTO flags_put_fails(serviceid, flag_id, flag, teamid, "
        "   date_start, date_end, reason) VALUES("
        "'" + flag.getServiceId() + "', "
        + "'" + flag.getId() + "', "
        + "'" + flag.getValue() + "', "
        + "'" + flag.getTeamId() + "', "
        + std::to_string(flag.getTimeStartInMs()) + ", "
        + std::to_string(flag.getTimeEndInMs()) + ", "
        + "'" + sReason + "'"
        + ");";
    if (!m_pFlagsPutFails->insert(sQuery)) {
        WsjcppLog::err(TAG, "Error insert " + sQuery);
    }
}

void EmployDatabase::insertFlagAttempt(std::string sTeamId, std::string sFlag) {
    std::string sQuery = "INSERT INTO flags_attempts(flag, teamid, dt) "
        " VALUES('" + sFlag + "', '" + sTeamId + "', " + std::to_string(WsjcppCore::getCurrentTimeInMilliseconds()) + ");";

    if (!m_pFlagsAttempts->insert(sQuery)) {
        WsjcppLog::err(TAG, "Error insert");
    }
}

int EmployDatabase::numberOfFlagAttempts(std::string sTeamId) {
    return m_pFlagsAttempts->selectCount(
        "SELECT COUNT(*) FROM flags_attempts WHERE teamid = '" + sTeamId + "';"
    );
}