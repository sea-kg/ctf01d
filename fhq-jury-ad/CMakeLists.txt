cmake_minimum_required(VERSION 3.0)

project(fhq-jury-ad)

include(${CMAKE_CURRENT_SOURCE_DIR}/src.wsjcpp/CMakeLists.txt)

#### BEGIN_WSJCPP_APPEND
#### END_WSJCPP_APPEND

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${fhq-jury-ad_SOURCE_DIR})
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Sources

# include header dirs
list (APPEND WSJCPP_INCLUDE_DIRS "src")
list (APPEND WSJCPP_INCLUDE_DIRS "src/checker")
list (APPEND WSJCPP_INCLUDE_DIRS "src/config")
list (APPEND WSJCPP_INCLUDE_DIRS "src/objects")
list (APPEND WSJCPP_INCLUDE_DIRS "src/resources")
list (APPEND WSJCPP_INCLUDE_DIRS "src/resources.wsjcpp")
list (APPEND WSJCPP_INCLUDE_DIRS "src/scoreboard")
list (APPEND WSJCPP_INCLUDE_DIRS "src/utils")
list (APPEND WSJCPP_INCLUDE_DIRS "src/interfaces")
list (APPEND WSJCPP_INCLUDE_DIRS "src/storages")
list (APPEND WSJCPP_INCLUDE_DIRS "src/storages/file")
list (APPEND WSJCPP_INCLUDE_DIRS "src/web-server")
list (APPEND WSJCPP_INCLUDE_DIRS "src/web-server/core")

# checker
list (APPEND WSJCPP_SOURCES "src/checker/dorunchecker.h")
list (APPEND WSJCPP_SOURCES "src/checker/dorunchecker.cpp")
list (APPEND WSJCPP_SOURCES "src/checker/service_checker_thread.h")
list (APPEND WSJCPP_SOURCES "src/checker/service_checker_thread.cpp")

# config
list (APPEND WSJCPP_SOURCES "src/config/conf_file_parser.h")
list (APPEND WSJCPP_SOURCES "src/config/conf_file_parser.cpp")
list (APPEND WSJCPP_SOURCES "src/config/config.h")
list (APPEND WSJCPP_SOURCES "src/config/config.cpp")
list (APPEND WSJCPP_SOURCES "src/config/read_teams_conf.h")
list (APPEND WSJCPP_SOURCES "src/config/read_teams_conf.cpp")
list (APPEND WSJCPP_SOURCES "src/config/service.h")
list (APPEND WSJCPP_SOURCES "src/config/service.cpp")
list (APPEND WSJCPP_SOURCES "src/config/team.h")
list (APPEND WSJCPP_SOURCES "src/config/team.cpp")


# web-server
list (APPEND WSJCPP_SOURCES "src/web-server/http_handler_api_v1.h")
list (APPEND WSJCPP_SOURCES "src/web-server/http_handler_api_v1.cpp")
list (APPEND WSJCPP_SOURCES "src/web-server/http_handler_web_folder.h")
list (APPEND WSJCPP_SOURCES "src/web-server/http_handler_web_folder.cpp")

# objects
list (APPEND WSJCPP_SOURCES "src/objects/flag.h")
list (APPEND WSJCPP_SOURCES "src/objects/flag.cpp")

# resources

#[begin-list-of-resources]

list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_favicon_16x16_png.h")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_favicon_16x16_png.cpp")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_favicon_ico.h")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_favicon_ico.cpp")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_index_html.h")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_index_html.cpp")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_favicon_32x32_png.h")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_favicon_32x32_png.cpp")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_css_scoreboard_css.h")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_css_scoreboard_css.cpp")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_defence_svg.h")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_defence_svg.cpp")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_logo_png.h")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_logo_png.cpp")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_first_blood_svg.h")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_first_blood_svg.cpp")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_attack_svg.h")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_attack_svg.cpp")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_activity_svg.h")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_activity_svg.cpp")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_uptime_svg.h")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_uptime_svg.cpp")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_points_svg.h")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_points_svg.cpp")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_states_down_svg.h")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_states_down_svg.cpp")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_states_up_svg.h")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_states_up_svg.cpp")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_states_mumble_svg.h")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_states_mumble_svg.cpp")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_states_shit_svg.h")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_states_shit_svg.cpp")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_states_wait_svg.h")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_states_wait_svg.cpp")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_states_corrupt_svg.h")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_states_corrupt_svg.cpp")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_states_coffeebreak_svg.h")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_states_coffeebreak_svg.cpp")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_teams_unknown_svg.h")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_images_teams_unknown_svg.cpp")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_js_scoreboard_js.h")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/html_js_scoreboard_js.cpp")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/config_yml.h")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/config_yml.cpp")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/mysql_storage_conf.h")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/mysql_storage_conf.cpp")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/scoreboard_conf.h")
list (APPEND WSJCPP_SOURCES "./src/resources.wsjcpp/scoreboard_conf.cpp")

#[end-list-of-resources]

list (APPEND WSJCPP_SOURCES "src/resources/resources_manager.h")
list (APPEND WSJCPP_SOURCES "src/resources/resources_manager.cpp")

# scoreboard
list (APPEND WSJCPP_SOURCES "src/scoreboard/scoreboard.cpp")
list (APPEND WSJCPP_SOURCES "src/scoreboard/scoreboard.h")
list (APPEND WSJCPP_SOURCES "src/scoreboard/service_costs_and_statistics.h")
list (APPEND WSJCPP_SOURCES "src/scoreboard/service_costs_and_statistics.cpp")
list (APPEND WSJCPP_SOURCES "src/scoreboard/service_status_cell.h")
list (APPEND WSJCPP_SOURCES "src/scoreboard/service_status_cell.cpp")
list (APPEND WSJCPP_SOURCES "src/scoreboard/team_status_row.h")
list (APPEND WSJCPP_SOURCES "src/scoreboard/team_status_row.cpp")

# utils
list (APPEND WSJCPP_SOURCES "src/utils/utils_help_parse_args.h")
list (APPEND WSJCPP_SOURCES "src/utils/utils_help_parse_args.cpp")

# storages
list (APPEND WSJCPP_SOURCES "src/storages/storages.h")
list (APPEND WSJCPP_SOURCES "src/storages/storages.cpp")

# file storage
# list (APPEND WSJCPP_SOURCES "src/storages/file/file_storage.h")
# list (APPEND WSJCPP_SOURCES "src/storages/file/file_storage.cpp")

# main
list (APPEND WSJCPP_SOURCES "src/main.cpp")


############################
##### mysql storage
list (APPEND WSJCPP_INCLUDE_DIRS "src/storages/mysql")
list (APPEND WSJCPP_SOURCES "src/storages/mysql/mysql_storage.h")
list (APPEND WSJCPP_SOURCES "src/storages/mysql/mysql_storage.cpp")

# Find and make sure the system have the header file
find_path(MYSQL_HEADER mysql/mysql.h)
if(MYSQL_HEADER STREQUAL "MYSQL_HEADER-NOTFOUND")
    message(FATAL_ERROR "Could not find the mysql/mysql.h header file: apt install libmysqlclient-dev")
endif()

include(FindPkgConfig)
pkg_check_modules(LIBMYSQLCLIENT REQUIRED mysqlclient)

foreach(FLAG ${LIBMYSQLCLIENT_CFLAGS_OTHER})
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAG}")
endforeach()

list (APPEND WSJCPP_INCLUDE_DIRS ${LIBMYSQLCLIENT_LIBRARY_DIRS})
list (APPEND WSJCPP_LIBRARIES ${LIBMYSQLCLIENT_LIBRARIES})

#####
############################

# ZLIB
find_package( ZLIB REQUIRED )
if ( ZLIB_FOUND )
    list (APPEND WSJCPP_INCLUDE_DIRS ${ZLIB_INCLUDE_DIRS})
    include_directories( ${ZLIB_INCLUDE_DIRS} )
    list (APPEND WSJCPP_LIBRARIES ${ZLIB_LIBRARIES})
endif( ZLIB_FOUND )

# LIBPNG
find_package(PNG REQUIRED)
if (NOT PNG_FOUND)
    message(FATAL_ERROR "You don't seem to have libpng development libraries installed (apt isntall libpng-dev)")
else ()
    list (APPEND WSJCPP_INCLUDE_DIRS ${PNG_INCLUDE_DIR})
    list (APPEND WSJCPP_LIBRARIES ${PNG_LIBRARY})
endif ()

# CURL
FIND_PACKAGE(CURL)
IF(CURL_FOUND)
  list (APPEND WSJCPP_INCLUDE_DIRS ${CURL_INCLUDE_DIR})
  list (APPEND WSJCPP_LIBRARIES ${CURL_LIBRARIES})
ELSE(CURL_FOUND)
  MESSAGE(FATAL_ERROR "Could not find the CURL library and development files.")
ENDIF(CURL_FOUND)

include_directories(${WSJCPP_INCLUDE_DIRS})

add_executable (fhq-jury-ad ${WSJCPP_SOURCES})

target_link_libraries(fhq-jury-ad -lpthread ${WSJCPP_LIBRARIES} )

install(
    TARGETS
        fhq-jury-ad
    RUNTIME DESTINATION
        ${CMAKE_INSTALL_PREFIX}/bin
)
