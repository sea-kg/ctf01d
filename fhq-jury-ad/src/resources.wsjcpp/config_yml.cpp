// automaticly generated by update_resources.py

#include <config_yml.h>

REGISTRY_WSJCPP_RESOURCE_FILE(RES_config_yml)

const std::string &RES_config_yml::getFilename() {
    static const std::string s = "config.yml";
    return s;
}

const int RES_config_yml::getBufferSize() {
    return 1062;
}

const char *RES_config_yml::getBuffer() {
    static const std::string sRet =  // size: 1062
        "## Combined config for fhq-jury-ad\n"
        "# use 2 spaces for tab\n"
        "\n"
        "game:\n"
        "  id: \"test\" # uniq gameid must be regexp [a-z0-9]+\n"
        "  name: \"Test First Game\" # visible game name in scoreboard\n"
        "  start: \"2019-10-04 10:30:00\" # start time of game (UTC)\n"
        "  end: \"2030-09-06 13:00:00\" # end time of game (UTC)\n"
        "  coffee_break_start: \"2020-10-04 10:30:00\" # start time of game coffee break (UTC), but it will be ignored if period more (or less) then start and end\n"
        "  coffee_break_end: \"2021-01-04 10:30:00\" # end time of game coffee break (UTC), but it will be ignored if period more (or less) then start and end\n"
        "  flag_timelive_in_min: 1 # you can change flag time live (in minutes)\n"
        "  basic_costs_stolen_flag_in_points: 10 # basic costs stolen (attack) flag in points for adaptive scoreboard\n"
        "\n"
        "server:\n"
        "  # use storage which storage will be used, now possible values:\n"
        "  # mysql - use mysql database\n"
        "  # ram - (!not implemented yet) use just memory\n"
        "  # local - (!not implemented yet) use just memory\n"
        "  # postgres - (!not implemented yet) will be used postgre database\n"
        "  use_storage: mysql\n"
    ;
    return sRet.c_str();
} //::buffer() 

