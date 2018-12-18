#include <mysql_storage.h>
#include <utils_logger.h>
#include <mysql/mysql.h>
#include <utils_parse_config.h>

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

std::string MySqlStorage::type() {
    return "mysql";
}

// ----------------------------------------------------------------------

bool MySqlStorage::applyConfigFromFile(const std::string &sConfigFile, 
            std::vector<ModelTeamConf> &vTeams, 
            std::vector<ModelServiceConf> &vServicesConf) {
    Log::info(TAG, "Reading config: " + sConfigFile);
    
    if (!Log::fileExists(sConfigFile)) {
        Log::err(TAG, "File " + sConfigFile + " does not exists ");
        return false;
    }

    // game.conf - will be override configs from conf.ini
    UtilsParseConfig mysqlStorageConf = UtilsParseConfig(sConfigFile);
    if (!mysqlStorageConf.parseConfig()) {
        Log::err(TAG, "Could not parse " + sConfigFile);
        return false;
    }

    m_sDatabaseHost = mysqlStorageConf.getStringValueFromConfig("mysql_storage.dbhost", m_sDatabaseHost);
    Log::info(TAG, "mysql_storage.dbhost: " + m_sDatabaseHost);

    m_nDatabasePort = mysqlStorageConf.getIntValueFromConfig("mysql_storage.dbport", m_nDatabasePort);
    Log::info(TAG, "mysql_storage.dbport: " + std::to_string(m_nDatabasePort));

    m_sDatabaseName = mysqlStorageConf.getStringValueFromConfig("mysql_storage.dbname", m_sDatabaseName);
    Log::info(TAG, "mysql_storage.dbname: " + m_sDatabaseName);


    m_sDatabaseUser = mysqlStorageConf.getStringValueFromConfig("mysql_storage.dbuser", m_sDatabaseUser);
    Log::info(TAG, "mysql_storage.dbuser: " + m_sDatabaseUser);

    m_sDatabasePass = mysqlStorageConf.getStringValueFromConfig("mysql_storage.dbpass", m_sDatabasePass);
    // Log::info(TAG, "mysql_storage.dbpass: " + m_sDatabasePass);

    if (this->m_sDatabaseHost == "") {
        Log::err(TAG, sConfigFile + ": mysql_storage.dbhost - not found");
		return false;
	}

    if (this->m_sDatabaseName == "") {
		Log::err(TAG, sConfigFile + ": mysql_storage.dbname - not found");
		return false;
	}

	if (this->m_sDatabaseUser == "") {
		Log::err(TAG, sConfigFile + ": mysql_storage.dbuser - not found");
		return false;
	}

    if (this->m_sDatabasePass == "") {
		Log::err(TAG, sConfigFile + ": mysql_storage.dbpass - not found");
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

    vUpdates.push_back(MySQLDBUpdate(11,  // don't change if after commit
        "ALTER TABLE `flags_live` MODIFY `team_stole` VARCHAR(50) NOT NULL DEFAULT '';"
    ));

    vUpdates.push_back(MySQLDBUpdate(12,  // don't change if after commit
        "ALTER TABLE `flags_live` MODIFY `teamid` VARCHAR(50) NOT NULL DEFAULT '';"
    ));

    vUpdates.push_back(MySQLDBUpdate(13,  // don't change if after commit
        "ALTER TABLE `flags_live` MODIFY `serviceid` VARCHAR(50) NOT NULL DEFAULT '';"
    ));

    vUpdates.push_back(MySQLDBUpdate(14,  // don't change if after commit
        "ALTER TABLE `flags` MODIFY `team_stole` VARCHAR(50) NOT NULL DEFAULT '';"
    ));

    vUpdates.push_back(MySQLDBUpdate(15,  // don't change if after commit
        "ALTER TABLE `flags` MODIFY `teamid` VARCHAR(50) NOT NULL DEFAULT '';"
    ));

    vUpdates.push_back(MySQLDBUpdate(16,  // don't change if after commit
        "ALTER TABLE `flags` MODIFY `serviceid` VARCHAR(50) NOT NULL DEFAULT '';"
    ));

    vUpdates.push_back(MySQLDBUpdate(17,  // don't change if after commit
        "ALTER TABLE `flags_attempts` MODIFY `teamid` VARCHAR(50) NOT NULL DEFAULT '';"
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
        "'" + serviceConf.id() + "', "
        + "'" + flag.id() + "', "
        + "'" + flag.value() + "', "
        + "'" + teamConf.id() + "', "
        + std::to_string(flag.timeStart()) + ", "
        + std::to_string(flag.timeEnd()) + ", "
        + "''"
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
        "WHERE serviceid = '" + serviceConf.id() + "' "
        "   AND teamid = '" + teamConf.id() + "' "
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
            flag.setServiceId(serviceConf.id());
            flag.setTeamId(teamConf.id());
            flag.setId(std::string(row[0]));
            flag.setValue(std::string(row[1]));
            flag.setTimeStart(std::stol(std::string(row[2])));
            flag.setTimeEnd(std::stol(std::string(row[3])));
            flag.setTeamStole(std::string(row[4]));
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
            "WHERE serviceid = '" + serviceConf.id() + "' "
            "   AND teamid = '" + teamConf.id() + "' "
            "   AND team_stole = '' "
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
            "       WHERE serviceid = '" + serviceConf.id() + "' "
            "           AND team_stole = '" + teamConf.id() + "' "
            "           AND date_start > " + std::to_string(long(m_nGameStartUTCInSec)*1000) + " "
            "           AND date_end < " + std::to_string(long(m_nGameEndUTCInSec)*1000) + " "
            "   UNION ALL"
            "   SELECT COUNT(*) as attack FROM flags "
            "       WHERE serviceid = '" + serviceConf.id() + "' "
            "           AND team_stole = '" + teamConf.id() + "' "
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
            "       WHERE serviceid = '" + serviceConf.id() + "' "
            "           AND teamid = '" + teamConf.id() + "' "
            "           AND date_start > " + std::to_string(long(m_nGameStartUTCInSec)*1000) + " "
            "           AND date_end < " + std::to_string(long(m_nGameEndUTCInSec)*1000) + " "
            "   UNION ALL"
            "   SELECT COUNT(*) as sla FROM flags "
            "       WHERE serviceid = '" + serviceConf.id() + "' "
            "           AND teamid = '" + teamConf.id() + "' "
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

    m_pScoreboard->setServiceScore(teamConf.id(), serviceConf.id(), nDefence, nAttack, nSLA);
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
            resultFlag.setTeamId(std::string(row[0]));
            resultFlag.setId(std::string(row[2]));
            resultFlag.setServiceId(std::string(row[1]));
            resultFlag.setValue(std::string(row[3]));
            resultFlag.setTimeStart(std::stol(std::string(row[4])));
            resultFlag.setTimeEnd(std::stol(std::string(row[5])));
            resultFlag.setTeamStole(std::string(row[6]));
            mysql_free_result(pRes);
            return true;
        }
        mysql_free_result(pRes);
    }
    return false;
}

// ----------------------------------------------------------------------

bool MySqlStorage::updateTeamStole(const std::string &sFlag, const std::string &sTeamId) {
    MYSQL *pConn = getDatabaseConnection();
    // TODO check on null

    std::string sQuery = ""
        "UPDATE flags_live SET team_stole = '" + sTeamId + "' "
        "WHERE flag = '" + sFlag + "'  "
        "  AND team_stole = '' "
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
    return resultFlag.teamStole() == sTeamId;
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
        "'" + flag.serviceId() + "', "
        + "'" + flag.id() + "', "
        + "'" + flag.value() + "', "
        + "'" + flag.teamId() + "', "
        + std::to_string(flag.timeStart()) + ", "
        + std::to_string(flag.timeEnd()) + ", "
        + "'" + flag.teamStole() + "'"
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