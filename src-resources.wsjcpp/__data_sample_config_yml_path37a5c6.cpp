// automaticly generated by wsjcpp:v0.5.1

#include "__data_sample_config_yml_path37a5c6.h"

REGISTRY_WSJCPP_RESOURCE_FILE(RES___data_sample_config_yml_path37a5c6)

const std::string &RES___data_sample_config_yml_path37a5c6::getFilename() const {
    static const std::string s = "./data_sample/config.yml";
    return s;
}
// ---------------------------------------------------------------------

const std::string &RES___data_sample_config_yml_path37a5c6::getPackAs() const {
    static const std::string s = "text";
    return s;
}
// ---------------------------------------------------------------------

int RES___data_sample_config_yml_path37a5c6::getBufferSize() const {
    return 4937;
}

// ---------------------------------------------------------------------

const char *RES___data_sample_config_yml_path37a5c6::getBuffer() const {
    static const std::string sRet = "" // size: 4937
        "## Combined config for ctf01d\n"
        "# use 2 spaces for tab\n"
        "\n"
        "game:\n"
        "  id: \"test\" # uniq gameid must be regexp [a-z0-9]+\n"
        "  name: \"Test First Game\" # visible game name in scoreboard\n"
        "  start: \"2023-11-12 16:00:00\" # start time of game (UTC)\n"
        "  end: \"2030-11-12 22:00:00\" # end time of game (UTC)\n"
        "  coffee_break_start: \"2023-11-12 20:00:00\" # start time of game coffee break (UTC), but it will be ignored if period more (or less) then start and end\n"
        "  coffee_break_end: \"2023-11-12 21:00:00\" # end time of game coffee break (UTC), but it will be ignored if period more (or less) then start and end\n"
        "  flag_timelive_in_min: 1 # you can change flag time live (in minutes)\n"
        "  basic_costs_stolen_flag_in_points: 1 # basic costs stolen (attack) flag in points for adaptive scoreboard\n"
        "  cost_defence_flag_in_points: 1.0 # cost defences flag in points\n"
        "\n"
        "scoreboard:\n"
        "  port: 8080 # http port for scoreboard\n"
        "  htmlfolder: \"./html\" # web page for scoreboard see index-template.html\n"
        "  random: no # If yes - will be random values in scoreboard\n"
        "\n"
        "checkers:\n"
        "  - id: \"example_service1\" # work directory will be checker_example_service4\n"
        "    service_name: \"Service1\"\n"
        "    enabled: yes\n"
        "    # host: 127.0.0.1\n"
        "    # port: 10001\n"
        "    script_path: \"./checker.py\"\n"
        "    script_wait_in_sec: 5 # max time for running script\n"
        "    time_sleep_between_run_scripts_in_sec: 15 # like a round for service\n"
        "  - id: \"example_service2\" # work directory will be checker_example_service4\n"
        "    service_name: \"Service2\"\n"
        "    enabled: yes\n"
        "    script_path: \"./checker.py\"\n"
        "    script_wait_in_sec: 5\n"
        "    time_sleep_between_run_scripts_in_sec: 15\n"
        "  - id: \"example_service3\" # work directory will be checker_example_service4\n"
        "    service_name: \"Service3\"\n"
        "    enabled: yes\n"
        "    script_path: \"./checker.py\"\n"
        "    script_wait_in_sec: 5\n"
        "    time_sleep_between_run_scripts_in_sec: 15\n"
        "  - id: \"example_service4\" # work directory will be checker_example_service4\n"
        "    service_name: \"Service4\"\n"
        "    enabled: yes\n"
        "    script_path: \"./checker.py\"\n"
        "    script_wait_in_sec: 5\n"
        "    time_sleep_between_run_scripts_in_sec: 15\n"
        "\n"
        "teams:\n"
        "  - id: \"t01\" # must be uniq\n"
        "    name: \"Team #1\"\n"
        "    active: yes\n"
        "    logo: \"./html/images/teams/team01.png\"\n"
        "    ip_address: \"127.0.1.1\" # address to vulnserver\n"
        "  - id: \"t02\" # must be uniq\n"
        "    name: \"Team #2\"\n"
        "    active: yes\n"
        "    logo: \"./html/images/teams/team02.png\"\n"
        "    ip_address: \"127.0.2.1\" # address to vulnserver\n"
        "  - id: \"t03\" # must be uniq\n"
        "    name: \"Team #3\"\n"
        "    active: yes\n"
        "    logo: \"./html/images/teams/team03.png\"\n"
        "    ip_address: \"127.0.3.1\" # address to vulnserver\n"
        "  - id: \"t04\" # must be uniq\n"
        "    name: \"Team #4\"\n"
        "    active: yes\n"
        "    logo: \"./html/images/teams/team04.png\"\n"
        "    ip_address: \"127.0.3.1\" # address to vulnserver\n"
        "  - id: \"t05\" # must be uniq\n"
        "    name: \"Team #5\"\n"
        "    active: yes\n"
        "    logo: \"./html/images/teams/team05.png\"\n"
        "    ip_address: \"127.0.5.1\" # address to vulnserver\n"
        "  - id: \"t06\" # must be uniq\n"
        "    name: \"Team #6\"\n"
        "    active: yes\n"
        "    logo: \"./html/images/teams/team06.png\"\n"
        "    ip_address: \"127.0.6.1\" # address to vulnserver\n"
        "  - id: \"t07\" # must be uniq\n"
        "    name: \"Team #7\"\n"
        "    active: yes\n"
        "    logo: \"./html/images/teams/team07.png\"\n"
        "    ip_address: \"127.0.7.1\" # address to vulnserver\n"
        "  - id: \"t08\" # must be uniq\n"
        "    name: \"Team #8\"\n"
        "    active: yes\n"
        "    logo: \"./html/images/teams/team08.png\"\n"
        "    ip_address: \"127.0.8.1\" # address to vulnserver\n"
        "  - id: \"t09\" # must be uniq\n"
        "    name: \"Team #9\"\n"
        "    active: yes\n"
        "    logo: \"./html/images/teams/team09.png\"\n"
        "    ip_address: \"127.0.9.1\" # address to vulnserver\n"
        "  - id: \"t10\" # must be uniq\n"
        "    name: \"Team #10\"\n"
        "    active: yes\n"
        "    logo: \"./html/images/teams/team10.png\"\n"
        "    ip_address: \"127.0.10.1\" # address to vulnserver\n"
        "  - id: \"t11\" # must be uniq\n"
        "    name: \"Team #11\"\n"
        "    active: yes\n"
        "    logo: \"./html/images/teams/team11.png\"\n"
        "    ip_address: \"127.0.11.1\" # address to vulnserver\n"
        "  - id: \"t12\" # must be uniq\n"
        "    name: \"Team #12\"\n"
        "    active: yes\n"
        "    logo: \"./html/images/teams/team12.png\"\n"
        "    ip_address: \"127.0.12.1\" # address to vulnserver\n"
        "  - id: \"t13\" # must be uniq\n"
        "    name: \"Team #13\"\n"
        "    active: yes\n"
        "    logo: \"./html/images/teams/team13.png\"\n"
        "    ip_address: \"127.0.13.1\" # address to vulnserver\n"
        "  - id: \"t14\" # must be uniq\n"
        "    name: \"Team #14\"\n"
        "    active: yes\n"
        "    logo: \"./html/images/teams/team14.png\"\n"
        "    ip_address: \"127.0.14.1\" # address to vulnserver\n"
        "  - id: \"t15\" # must be uniq\n"
        "    name: \"Team #15\"\n"
        "    active: yes\n"
        "    logo: \"./html/images/teams/team15.png\"\n"
        "    ip_address: \"127.0.15.1\" # address to vulnserver\n"
        "  - id: \"t16\" # must be uniq\n"
        "    name: \"Team #16\"\n"
        "    active: yes\n"
        "    logo: \"./html/images/teams/team16.png\"\n"
        "    ip_address: \"127.0.16.1\" # address to vulnserver\n"
        "  - id: \"t17\" # must be uniq\n"
        "    name: \"Team #17\"\n"
        "    active: yes\n"
        "    logo: \"./html/images/teams/team17.png\"\n"
        "    ip_address: \"127.0.17.1\" # address to vulnserver\n"
        "  - id: \"t18\" # must be uniq\n"
        "    name: \"Team #18\"\n"
        "    active: yes\n"
        "    logo: \"./html/images/teams/team18.png\"\n"
        "    ip_address: \"127.0.18.1\" # address to vulnserver\n"
        ""
    ;
    return sRet.c_str();
} //::buffer() 


