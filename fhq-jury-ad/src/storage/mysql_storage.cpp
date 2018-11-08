#include <mysql_storage.h>
#include <utils_logger.h>
#include <ini.h>
#include <mysql/mysql.h>

MySqlStorage::MySqlStorage(ModelScoreboard *pScoreboard, int nGameStartUTCInSec, int nGameEndUTCInSec) {
    m_pScoreboard = pScoreboard;
    TAG = "MySqlStorage";
    m_nGameStartUTCInSec = nGameStartUTCInSec;
    m_nGameEndUTCInSec = nGameEndUTCInSec;
    m_sDatabaseHost = "";
    m_sDatabaseName = "";
    m_sDatabaseUser = "";
    m_sDatabasePass = "";
    m_nDatabasePort = 3306;
}

// ----------------------------------------------------------------------

void MySqlStorage::setDatabaseHost(const std::string &sDatabaseHost) {
    m_sDatabaseHost = sDatabaseHost;
}

// ----------------------------------------------------------------------

void MySqlStorage::setDatabaseName(const std::string &sDatabaseName) {
    m_sDatabaseName = sDatabaseName;
}

// ----------------------------------------------------------------------

void MySqlStorage::setDatabaseUser(const std::string &sDatabaseUser) {
    m_sDatabaseUser = sDatabaseUser;
}

// ----------------------------------------------------------------------

void MySqlStorage::setDatabasePass(const std::string &sDatabasePass) {
    m_sDatabasePass = sDatabasePass;
}

// ----------------------------------------------------------------------

void MySqlStorage::setDatabasePort(int nDatabasePort) {
    m_nDatabasePort = nDatabasePort;
}

// ----------------------------------------------------------------------

std::string MySqlStorage::type() {
    return "mysql";
}

// ----------------------------------------------------------------------

static int handler_parse_mysql_storage_config(void* p, const char* section, const char* name, const char* value) {
    MySqlStorage* pStorage = (MySqlStorage*)p;
    std::string sSection(section);
    std::string sName(name);
    std::string sValue(value);

    if (sSection == "mysql_storage") {
        if (sName == "dbhost") {
            pStorage->setDatabaseHost(sValue);
        } else if (sName == "dbname") {
            pStorage->setDatabaseName(sValue);
        } else if (sName == "dbuser") {
            pStorage->setDatabaseUser(sValue);
        } else if (sName == "dbpass") {
            pStorage->setDatabasePass(sValue);
        } else if (sName == "dbport") {
            int nValue = std::stoi(sValue); // TODO try catch
            pStorage->setDatabasePort(nValue);
        } else {
            Log::warn("MySqlStorage", "Unknown props mysql_storage/" + sName);
        }
    }
    return 1;
}

// ----------------------------------------------------------------------

bool MySqlStorage::applyConfigFromFile(const std::string &sFilePath, 
            std::vector<ModelTeamConf> &vTeams, 
            std::vector<ModelServiceConf> &vServicesConf) {
    if (ini_parse(sFilePath.c_str(), handler_parse_mysql_storage_config, this) < 0) {
        Log::err(TAG, "Could not load config file");
        return false;
    }

    if (this->m_sDatabaseHost == "") {
        Log::err(TAG, "mysql_storage/dbhost - not found");
		return false;
	}

    if (this->m_sDatabaseName == "") {
		Log::err(TAG, "mysql_storage/dbname - not found");
		return false;
	}

	if (this->m_sDatabaseUser == "") {
		Log::err(TAG, "mysql_storage/dbuser - not found");
		return false;
	}

    if (this->m_sDatabasePass == "") {
		Log::err(TAG, "mysql_storage/dbpass - not found");
		return false;
	}

    // try connect to database
    MYSQL *pConn = getDatabaseConnection();
    if (pConn == NULL) {
        Log::err(TAG, "Failed to connect.");
        return false;
    }

    return checkAndInstall(pConn);
}

// ----------------------------------------------------------------------

struct MySQLDBUpdate {
    MySQLDBUpdate() : nVersion(0), sQuery("") {};
    MySQLDBUpdate(int nVersion, std::string sQuery) : nVersion(nVersion), sQuery(sQuery) {};
    int nVersion;
    std::string sQuery;
};

bool MySqlStorage::checkAndInstall(MYSQL *pConn) {
    std::string sQuery = "SELECT id, version FROM db_updates ORDER BY id DESC LIMIT 0,1";
    int nCurrVersion = 0;
    if (mysql_query(pConn, sQuery.c_str())) {
        std::string sError(mysql_error(pConn));
        if (sError.find("db_updates' doesn't exist") != std::string::npos) {
            Log::info(TAG, "Creating table db_updates .... ");
            std::string sTableDbUpdates = 
                "CREATE TABLE IF NOT EXISTS db_updates ("
			    "  id int(11) NOT NULL AUTO_INCREMENT,"
                "  version INT DEFAULT NULL,"
                "  dt datetime DEFAULT NULL,"
                "  PRIMARY KEY (id)"
                ") ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;";
            if (mysql_query(pConn, sTableDbUpdates.c_str())) {
                std::string sError2(mysql_error(pConn));
                Log::err(TAG, "Problem on create table db_updates " + sError2);
                return false;
            } else {
                Log::ok(TAG, "Table db_updates success created");
                nCurrVersion = 1;
            }
        } else {
            Log::err(TAG, "Problem with database " + sError);
            return false;
        }
    } else {
        MYSQL_RES *pRes = mysql_use_result(pConn);
        MYSQL_ROW row;
        if ((row = mysql_fetch_row(pRes)) != NULL) {
            nCurrVersion = std::stol(std::string(row[1]));
        } else {
            nCurrVersion = 1;
        }
        mysql_free_result(pRes);
    }

    std::vector<MySQLDBUpdate> vUpdates;
    vUpdates.push_back(MySQLDBUpdate(2, // don't change if after commit
        "CREATE TABLE `flags` ("
        "  `id` int UNSIGNED NOT NULL,"
        "  `serviceid` int UNSIGNED DEFAULT NULL,"
        "  `flag_id` varchar(50) DEFAULT NULL,"
        "  `flag` varchar(36) DEFAULT NULL,"
        "  `teamid` varchar(300) DEFAULT NULL,"
        "  `date_start` bigint DEFAULT NULL,"
        "  `date_end` bigint DEFAULT NULL,"
        "  `team_stole` int UNSIGNED DEFAULT NULL"
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
    ));

    vUpdates.push_back(MySQLDBUpdate(3,  // don't change if after commit
        "ALTER TABLE `flags` ADD PRIMARY KEY (`id`);"
    ));

    vUpdates.push_back(MySQLDBUpdate(4,  // don't change if after commit
        "ALTER TABLE `flags` ADD INDEX( `teamid`, `team_stole`);"
    ));

    vUpdates.push_back(MySQLDBUpdate(5, // don't change if after commit
        "CREATE TABLE `flags_live` ("
        "  `id` int UNSIGNED NOT NULL,"
        "  `serviceid` int UNSIGNED DEFAULT NULL,"
        "  `flag_id` varchar(50) DEFAULT NULL,"
        "  `flag` varchar(36) DEFAULT NULL,"
        "  `teamid` int UNSIGNED DEFAULT NULL,"
        "  `date_start` bigint DEFAULT NULL,"
        "  `date_end` bigint DEFAULT NULL,"
        "  `team_stole` int UNSIGNED DEFAULT NULL"
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
    ));

    vUpdates.push_back(MySQLDBUpdate(6,  // don't change if after commit
        "ALTER TABLE `flags_live` ADD PRIMARY KEY (`id`);"
    ));

    vUpdates.push_back(MySQLDBUpdate(7,  // don't change if after commit
        "ALTER TABLE `flags` MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=1;"
    ));

    vUpdates.push_back(MySQLDBUpdate(8,  // don't change if after commit
        "ALTER TABLE `flags_live` MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=1;"
    ));

    vUpdates.push_back(MySQLDBUpdate(9, // don't change if after commit
        "CREATE TABLE `flags_attempts` ("
        "  `id` int UNSIGNED NOT NULL,"
        "  `flag` varchar(36) DEFAULT NULL,"
        "  `teamid` int UNSIGNED DEFAULT NULL,"
        "  `dt` bigint DEFAULT NULL"
        ") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
    ));

    vUpdates.push_back(MySQLDBUpdate(10,  // don't change if after commit
        "ALTER TABLE `flags_attempts` ADD PRIMARY KEY (`id`);"
    ));

    Log::info(TAG, "Current database version: " + std::to_string(nCurrVersion));

    bool bFoundUpdate = true;
    while (bFoundUpdate) {
        bFoundUpdate = false;
        MySQLDBUpdate nextUpdate;

        for (unsigned int i = 0; i < vUpdates.size(); i++) {
            if (vUpdates[i].nVersion == nCurrVersion + 1) {
                bFoundUpdate = true;
                nextUpdate = vUpdates[i];
                break;
            }
        }
        if (!bFoundUpdate) {
            break;
        }

        std::string sVersion = std::to_string(nextUpdate.nVersion);
        Log::info(TAG, "Install update  " + sVersion + "...");
        if (mysql_query(pConn, nextUpdate.sQuery.c_str())) {
            std::string sError2(mysql_error(pConn));
            Log::err(TAG, "Failed install update " + sVersion + ": " + sError2);
            return false;
        } else {
            Log::ok(TAG, "Update " + sVersion + " success installed");
            std::string sInsertNewVersion = "INSERT INTO db_updates(version,dt) VALUES(" + sVersion + ",NOW());";
            mysql_query(pConn, sInsertNewVersion.c_str());
            nCurrVersion = nextUpdate.nVersion;
        }
    }

    return true;
}

// ----------------------------------------------------------------------

void MySqlStorage::clean() {
    MYSQL *pConn = getDatabaseConnection();
    // TODO check on null

    {
        std::string sQuery = "DELETE FROM flags_live;";
        if (mysql_query(pConn, sQuery.c_str())) {
            Log::err(TAG, "Error insert: " + std::string(mysql_error(pConn)));
            return;
        }
        MYSQL_RES *pRes = mysql_use_result(pConn);
        mysql_free_result(pRes);
    }
    
    {
        std::string sQuery = "DELETE FROM flags;";
        if (mysql_query(pConn, sQuery.c_str())) {
            Log::err(TAG, "Error insert: " + std::string(mysql_error(pConn)));
            return;
        }
        MYSQL_RES *pRes = mysql_use_result(pConn);
        mysql_free_result(pRes);
    }
}

// ----------------------------------------------------------------------

MYSQL *MySqlStorage::getDatabaseConnection() {
    std::string sThreadId = Log::threadId();
    MYSQL *pDatabase = NULL;
    std::map<std::string,MYSQL *>::iterator it;
    it = m_mapConnections.find(sThreadId);
    if (it == m_mapConnections.end()) {
        pDatabase = mysql_init(NULL);
        if (!mysql_real_connect(pDatabase, 
                this->m_sDatabaseHost.c_str(),
                this->m_sDatabaseUser.c_str(),
                this->m_sDatabasePass.c_str(),
                this->m_sDatabaseName.c_str(), 0, NULL, 0)) {
            Log::err(TAG, std::string(mysql_error(pDatabase)));
            Log::err(TAG, "Failed to connect.");
            return NULL;
        }
        m_mapConnections[sThreadId] = pDatabase;
    }else{
        pDatabase = it->second;
    }
    return pDatabase; 
}

// ----------------------------------------------------------------------

void MySqlStorage::addLiveFlag(const ModelTeamConf &teamConf, const ModelServiceConf &serviceConf, const ModelFlag &flag){
    MYSQL *pConn = getDatabaseConnection();
    // TODO check connection with NULL

    std::string sQuery = "INSERT INTO flags_live(serviceid, flag_id, flag, teamid, "
        "   date_start, date_end, team_stole) VALUES("
        + std::to_string(serviceConf.num()) + ", "
        + "'" + flag.id() + "', "
        + "'" + flag.value() + "', "
        + std::to_string(teamConf.num()) + ", "
        + std::to_string(flag.timeStart()) + ", "
        + std::to_string(flag.timeEnd()) + ", "
        + "0"
        + ");";

    if (mysql_query(pConn, sQuery.c_str())) {
        Log::err(TAG, "Error insert: " + std::string(mysql_error(pConn)));
        return;
    }

    MYSQL_RES *pRes = mysql_use_result(pConn);
    mysql_free_result(pRes);
}

// ----------------------------------------------------------------------

std::vector<ModelFlag> MySqlStorage::endedFlags(const ModelTeamConf &teamConf, const ModelServiceConf &serviceConf){
    MYSQL *pConn = getDatabaseConnection();

    long nCurrentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::string sQuery = 
        "SELECT flag_id, flag, date_start, date_end, team_stole "
        "FROM flags_live "
        "WHERE serviceid = " + std::to_string(serviceConf.num()) + " "
        "   AND teamid = " + std::to_string(teamConf.num()) + " "
        "   AND date_end < " + std::to_string(nCurrentTime) + " "
        "   AND date_start > " + std::to_string(long(m_nGameStartUTCInSec)*1000) + " "
        "   AND date_end < " + std::to_string(long(m_nGameEndUTCInSec)*1000) + " "
        ";";

    std::vector<ModelFlag> vResult;
    if (mysql_query(pConn, sQuery.c_str())) {
        Log::err(TAG, "Error select (endedFlags): " + std::string(mysql_error(pConn)));
    } else {
        MYSQL_RES *pRes = mysql_use_result(pConn);
        MYSQL_ROW row;
        // output table name
        while ((row = mysql_fetch_row(pRes)) != NULL) {
            ModelFlag flag;
            flag.setServiceNum(serviceConf.num());
            flag.setTeamNum(teamConf.num());
            flag.setId(std::string(row[0]));
            flag.setValue(std::string(row[1]));
            flag.setTimeStart(std::stol(std::string(row[2])));
            flag.setTimeEnd(std::stol(std::string(row[3])));
            flag.setTeamStole(std::stoi(std::string(row[4])));
            vResult.push_back(flag);
        }
        mysql_free_result(pRes);
    }
    return vResult;
}

// ----------------------------------------------------------------------

void MySqlStorage::updateFlag(const ModelTeamConf &team, const ModelServiceConf &serviceConf, const ModelFlag &sFlag){
    // TODO
}

// ----------------------------------------------------------------------

void MySqlStorage::updateScoreboard(const ModelTeamConf &teamConf, const ModelServiceConf &serviceConf) {
    MYSQL *pConn = getDatabaseConnection();
    // TODO check connection with NULL

    long nCurrentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    // TODO check NULL
    int nDefence = 0;
    int nAttack = 0;
    double nSLA = 100.0;

    // calculate defence
    {
        std::string sQuery = 
            "SELECT COUNT(*) as defence FROM flags "
            "WHERE serviceid = " + std::to_string(serviceConf.num()) + " "
            "   AND teamid = " + std::to_string(teamConf.num()) + " "
            "   AND team_stole = 0 "
            "   AND date_start > " + std::to_string(long(m_nGameStartUTCInSec)*1000) + " "
            "   AND date_end < " + std::to_string(long(m_nGameEndUTCInSec)*1000) + " "
            ";";

        if (mysql_query(pConn, sQuery.c_str())) {
            Log::err(TAG, "Error select (updateScoreboard - calculate defence): " + std::string(mysql_error(pConn)));
        } else {
            MYSQL_RES *pRes = mysql_use_result(pConn);
            MYSQL_ROW row;
            // output table name
            while ((row = mysql_fetch_row(pRes)) != NULL) {
                nDefence += std::stoi(std::string(row[0]));
            }
            mysql_free_result(pRes);
        }
    }

    // calculate attack
    {
        std::string sQuery = 
            "SELECT SUM(attack) as attack FROM ("
            "   SELECT COUNT(*) as attack FROM flags_live "
            "       WHERE serviceid = " + std::to_string(serviceConf.num()) + " "
            "           AND team_stole = " + std::to_string(teamConf.num()) + " "
            "           AND date_start > " + std::to_string(long(m_nGameStartUTCInSec)*1000) + " "
            "           AND date_end < " + std::to_string(long(m_nGameEndUTCInSec)*1000) + " "
            "   UNION ALL"
            "   SELECT COUNT(*) as attack FROM flags "
            "       WHERE serviceid = " + std::to_string(serviceConf.num()) + " "
            "           AND team_stole = " + std::to_string(teamConf.num()) + " "
            "           AND date_start > " + std::to_string(long(m_nGameStartUTCInSec)*1000) + " "
            "           AND date_end < " + std::to_string(long(m_nGameEndUTCInSec)*1000) + " "
            ") t;";

        if (mysql_query(pConn, sQuery.c_str())) {
            Log::err(TAG, "Error select (updateScoreboard - calculate attack): " + std::string(mysql_error(pConn)));
        } else {
            MYSQL_RES *pRes = mysql_use_result(pConn);
            MYSQL_ROW row;
            // output table name
            while ((row = mysql_fetch_row(pRes)) != NULL) {
                nAttack += std::stoi(std::string(row[0]));
            }
            mysql_free_result(pRes);
        }
    }

    // calculate SLA
    int nFlagsSuccess = 0;
    {
        std::string sQuery = 
            "SELECT SUM(sla) as sla FROM ("
            "   SELECT COUNT(*) as sla FROM flags_live "
            "       WHERE serviceid = " + std::to_string(serviceConf.num()) + " "
            "           AND teamid = " + std::to_string(teamConf.num()) + " "
            "           AND date_start > " + std::to_string(long(m_nGameStartUTCInSec)*1000) + " "
            "           AND date_end < " + std::to_string(long(m_nGameEndUTCInSec)*1000) + " "
            "   UNION ALL"
            "   SELECT COUNT(*) as sla FROM flags "
            "       WHERE serviceid = " + std::to_string(serviceConf.num()) + " "
            "           AND teamid = " + std::to_string(teamConf.num()) + " "
            "           AND date_start > " + std::to_string(long(m_nGameStartUTCInSec)*1000) + " "
            "           AND date_end < " + std::to_string(long(m_nGameEndUTCInSec)*1000) + " "
            ") t;";

        // std::cout << sQuery << "\n";
        if (mysql_query(pConn, sQuery.c_str())) {
            Log::err(TAG, "Error select (updateScoreboard - calculate SLA): " + std::string(mysql_error(pConn)));
        } else {
            MYSQL_RES *pRes = mysql_use_result(pConn);
            MYSQL_ROW row;
            // output table name
            while ((row = mysql_fetch_row(pRes)) != NULL) {
                nFlagsSuccess += std::stoi(std::string(row[0]));
            }
            mysql_free_result(pRes);
        }
    }

    // sla
    nSLA = m_pScoreboard->calculateSLA(nFlagsSuccess, serviceConf);

    m_pScoreboard->setServiceScore(teamConf.num(), serviceConf.num(), nDefence, nAttack, nSLA);
}

// ----------------------------------------------------------------------

bool MySqlStorage::findFlagByValue(const std::string &sFlag, ModelFlag &resultFlag) {
    MYSQL *pConn = getDatabaseConnection();
    // TODO check on null

    std::string sQuery = 
        "SELECT teamid, serviceid, flag_id, flag, date_start, date_end, team_stole "
        "FROM flags_live "
        "WHERE "
        "   flag = '" + sFlag + "' "
        "   AND date_start > " + std::to_string(long(m_nGameStartUTCInSec)*1000) + " "
        "   AND date_end < " + std::to_string(long(m_nGameEndUTCInSec)*1000) + " "
        ";";

    ModelFlag flag;

    if (mysql_query(pConn, sQuery.c_str())) {
        Log::err(TAG, "Error select (findFlagByValue): " + std::string(mysql_error(pConn)));
        return false;
    } else {
        MYSQL_RES *pRes = mysql_use_result(pConn);
        MYSQL_ROW row;
        // output table name
        if ((row = mysql_fetch_row(pRes)) != NULL) {
            resultFlag.setTeamNum(std::stoi(std::string(row[0])));
            resultFlag.setId(std::string(row[2]));
            resultFlag.setServiceNum(std::stoi(std::string(row[1])));
            resultFlag.setValue(std::string(row[3]));
            resultFlag.setTimeStart(std::stol(std::string(row[4])));
            resultFlag.setTimeEnd(std::stol(std::string(row[5])));
            resultFlag.setTeamStole(std::stoi(std::string(row[6])));
            mysql_free_result(pRes);
            return true;
        }
        mysql_free_result(pRes);
    }
    return false;
}

// ----------------------------------------------------------------------

bool MySqlStorage::updateTeamStole(const std::string &sFlag, int nTeamNum) {
    MYSQL *pConn = getDatabaseConnection();
    // TODO check on null

    std::string sQuery = ""
        "UPDATE flags_live SET team_stole = " + std::to_string(nTeamNum) + " "
        "WHERE flag = '" + sFlag + "'  "
        "  AND team_stole = 0 "
        "  AND date_start > " + std::to_string(long(m_nGameStartUTCInSec)*1000) + " "
        "  AND date_end < " + std::to_string(long(m_nGameEndUTCInSec)*1000) + " "
        ";";


    if (mysql_query(pConn, sQuery.c_str())) {
        Log::err(TAG, "Error select (updateTeamStole): " + std::string(mysql_error(pConn)));
        return false;
    }

    ModelFlag resultFlag;

    if (!this->findFlagByValue(sFlag, resultFlag)) {
        return false;
    }
    return resultFlag.teamStole() == nTeamNum;
}

// ----------------------------------------------------------------------

void MySqlStorage::removeFlag(ModelFlag &flag) {
    MYSQL *pConn = getDatabaseConnection();
    // TODO check on null

    std::string sQuery = "DELETE FROM flags_live WHERE flag = '" + flag.value() + "';";
    if (mysql_query(pConn, sQuery.c_str())) {
        Log::err(TAG, "Error delete: " + std::string(mysql_error(pConn)));
        return;
    }
    MYSQL_RES *pRes = mysql_use_result(pConn);
    mysql_free_result(pRes);
    
}

// ----------------------------------------------------------------------

void MySqlStorage::moveToArchive(ModelFlag &flag) {
    MYSQL *pConn = getDatabaseConnection();
    // TODO check on null

    std::string sQuery = "INSERT INTO flags(serviceid, flag_id, flag, teamid, "
        "   date_start, date_end, team_stole) VALUES("
        + std::to_string(flag.serviceNum()) + ", "
        + "'" + flag.id() + "', "
        + "'" + flag.value() + "', "
        + std::to_string(flag.teamNum()) + ", "
        + std::to_string(flag.timeStart()) + ", "
        + std::to_string(flag.timeEnd()) + ", "
        + std::to_string(flag.teamStole())
        + ");";

    if (mysql_query(pConn, sQuery.c_str())) {
        Log::err(TAG, "Error insert: " + std::string(mysql_error(pConn)));
        return;
    }

    MYSQL_RES *pRes = mysql_use_result(pConn);
    mysql_free_result(pRes);

    removeFlag(flag);
    
}

// ----------------------------------------------------------------------
