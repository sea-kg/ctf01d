#ifndef UTILS_MAKE_FOLDERS_AND_FILES_H
#define UTILS_MAKE_FOLDERS_AND_FILES_H

#include <config.h>
#include <string>
#include <stdio.h>
#include <iostream>

class UtilsMakeFoldersAndFiles {

    public:
        static bool make(const std::string &sWorkspace);
        
        
    private:
        static bool dirExists(const std::string &sDirname);
        static bool makeDir(const std::string &sDirname);

        static bool fileExists(const std::string &sFilename);
        static bool writeFile(const std::string &sFilename, const std::string &sContent);
        static std::string g_sContent_servicehub_d_urls;
        static std::string g_sContent_conf_d_conf_ini;
        static std::string g_sContent_conf_d_conf_ini_sample;
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

#endif // UTILS_MAKE_FOLDERS_AND_FILES_H
