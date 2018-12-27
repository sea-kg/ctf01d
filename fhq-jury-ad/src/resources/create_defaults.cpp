#include "create_defaults.h"
#include <unistd.h>
#include <iostream>
#include <vector>
#include <fs.h>
#include <resources_manager.h>

// ---------------------------------------------------------------------

bool CreateDefaults::make(const std::string &sWorkspace) {
    if (!CreateDefaults::createFolders(sWorkspace)) {
        return false;
    }
    return CreateDefaults::createFiles(sWorkspace);
}

// ---------------------------------------------------------------------

bool CreateDefaults::createFolders(const std::string &sWorkspace){
    
    // prepare folders
    std::vector<std::string> vCreateDirs;
    vCreateDirs.push_back(sWorkspace + "/logs");
    vCreateDirs.push_back(sWorkspace + "/teams");
    vCreateDirs.push_back(sWorkspace + "/checkers");
    vCreateDirs.push_back(sWorkspace + "/html");
    vCreateDirs.push_back(sWorkspace + "/html/css");
    vCreateDirs.push_back(sWorkspace + "/html/js");
    vCreateDirs.push_back(sWorkspace + "/html/images");
    vCreateDirs.push_back(sWorkspace + "/html/images/teams");
    vCreateDirs.push_back(sWorkspace + "/html/images/states");

    for(int i = 0; i < vCreateDirs.size(); i++) {
        std::string sPath = vCreateDirs[i];
        // check dir existing
        if (!FS::dirExists(sPath)) {
            // try make dir
            if (!FS::makeDir(sPath)) {
                return false;
            } else {
                std::cout << "Created folder " << sPath << std::endl;
            }
        }
    }
    return true;
}

// ---------------------------------------------------------------------

bool CreateDefaults::createFiles(const std::string &sWorkspace) {

    // prepare default files
    struct LocalFileContent {
        LocalFileContent(const std::string &sPath, const std::string &sContent) {
            path = sPath;
            content = &sContent;
        };
        std::string path;
        const std::string *content;
    };

    std::vector<LocalFileContent> vCreateFiles;

    vCreateFiles.push_back(LocalFileContent(
        sWorkspace + "/game.conf", 
        CreateDefaults::g_sContent_game_conf
    ));

    vCreateFiles.push_back(LocalFileContent(
        sWorkspace + "/scoreboard.conf", 
        CreateDefaults::g_sContent_scoreboard_conf
    ));

    vCreateFiles.push_back(LocalFileContent(
        sWorkspace + "/mysql_storage.conf", 
        CreateDefaults::g_sContent_mysql_storage_conf
    ));

    vCreateFiles.push_back(LocalFileContent(
        sWorkspace + "/server.conf", 
        CreateDefaults::g_sContent_server_conf
    ));

    // write content of files
    for(int i = 0; i < vCreateFiles.size(); i++) {
        LocalFileContent lfc = vCreateFiles[i];
        // check file existing
        if (!FS::fileExists(lfc.path)) {
            if (!FS::writeFile(lfc.path, *lfc.content)) {
                return false;
            } else {
                std::cout << "Created file " << lfc.path << std::endl;
            }
        }
    }

    // TODO mkdir -p for files
    const std::vector<ResourceFile*> list = ResourceManager::list();
    for(int i = 0; i < list.size(); i++) {
        std::string sFilename = sWorkspace + "/html/" + list[i]->filename();
        if (!FS::fileExists(sFilename)) {
            if (!FS::writeFile(sFilename, list[i]->buffer(), list[i]->bufferSize())) {
                return false;
            } else {
                std::cout << "Created file " << sFilename << std::endl;
            }
        }
    }

	return true;
}

// ---------------------------------------------------------------------

std::string CreateDefaults::g_sContent_game_conf
    = ""
    "# uniq gameid must be regexp [a-z0-9]+\n"
    "game.id = test\n"
    "\n"
    "# visible game name in scoreboard\n"
    "game.name = Test First Game\n"
    "\n"
    "# start time of game (UTC)\n"
    "game.start = 2018-09-06 12:00:00\n"
    "\n"
    "# end time of game (UTC)\n"
    "game.end = 2018-09-06 13:00:00\n"
    "\n"
    "# you can change flag time live (in minutes)\n"
    "game.flag_timelive_in_min = 10\n"
    "";

// ---------------------------------------------------------------------

std::string CreateDefaults::g_sContent_scoreboard_conf
    = ""
    "# http port for scoreboard\n"
    "scoreboard.port = 8080\n"
    "\n"
    "# web page for scoreboard see index-template.html\n"
    "scoreboard.htmlfolder = ./html\n"
    "\n"
    "# If yes - will be random values in scoreboard\n"
    "scoreboard.random = no\n"
    ;

// ---------------------------------------------------------------------

std::string CreateDefaults::g_sContent_mysql_storage_conf
    = ""
    "# if server.use_storage is mysql\n"
    "mysql_storage.dbhost = localhost\n"
    "mysql_storage.dbport = 3306\n"
    "mysql_storage.dbname = fhqjuryad\n"
    "mysql_storage.dbuser = fhqjuryad\n"
    "mysql_storage.dbpass = fhqjuryad\n"
    "";

// ---------------------------------------------------------------------

std::string CreateDefaults::g_sContent_server_conf
    = ""
    "# use storage which storage will be used, now possible values:\n"
    "# mysql - will be used mysql database\n"
    "# ram - (not implemented yet) use just memory\n"
    "# postgres - (not implemented yet) will be used postgre database\n"
    "server.use_storage = mysql\n"
    ;

// ---------------------------------------------------------------------
