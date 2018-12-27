#ifndef CREATE_DEFAULTS_H
#define CREATE_DEFAULTS_H

#include <string>
#include <stdio.h>
#include <iostream>

class CreateDefaults {

    public:
        static bool make(const std::string &sWorkspace);
        static bool createFolders(const std::string &sWorkspace);
        static bool createFiles(const std::string &sWorkspace);

    private:

        static std::string g_sContent_game_conf;
        static std::string g_sContent_scoreboard_conf;
        static std::string g_sContent_mysql_storage_conf;
        static std::string g_sContent_server_conf;
};

#endif // CREATE_DEFAULTS_H
