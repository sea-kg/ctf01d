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
        static bool dirExists(const std::string &sDirname);
        static bool makeDir(const std::string &sDirname);

        static bool fileExists(const std::string &sFilename);
        static bool writeFile(const std::string &sFilename, const std::string &sContent);

        static std::string g_sContent_game_conf;
        static std::string g_sContent_scoreboard_conf;
        static std::string g_sContent_mysql_storage_conf;
        static std::string g_sContent_server_conf;

        static std::string g_sContent_html_index_html;
        static std::string g_sContent_html_css_scoreboard_css;
        static std::string g_sContent_html_js_scoreboard_js;
        
        static std::string g_sContent_html_images_states_mumble_svg;
        static std::string g_sContent_html_images_states_down_svg;
        static std::string g_sContent_html_images_states_up_svg;
        static std::string g_sContent_html_images_states_shit_svg;
        static std::string g_sContent_html_images_states_wait_svg;
        static std::string g_sContent_html_images_states_corrupt_svg;

        static std::string g_sContent_html_images_teams_unknown_svg;
};

#endif // CREATE_DEFAULTS_H