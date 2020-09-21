cmake_minimum_required(VERSION 3.0)

project(ctf01d)

include(${CMAKE_CURRENT_SOURCE_DIR}/src.wsjcpp/CMakeLists.txt)

#### BEGIN_WSJCPP_APPEND
#### END_WSJCPP_APPEND

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${ctf01d_SOURCE_DIR})
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Sources

# include header dirs
list (APPEND WSJCPP_INCLUDE_DIRS "src")
list (APPEND WSJCPP_INCLUDE_DIRS "src/argument_processors")
list (APPEND WSJCPP_INCLUDE_DIRS "src/checker")
list (APPEND WSJCPP_INCLUDE_DIRS "src/scoreboard")
list (APPEND WSJCPP_INCLUDE_DIRS "src/storages")
list (APPEND WSJCPP_INCLUDE_DIRS "src/storages/file")
list (APPEND WSJCPP_INCLUDE_DIRS "src/service_locator")
list (APPEND WSJCPP_INCLUDE_DIRS "src/http_handlers")

# argument processors
list (APPEND WSJCPP_SOURCES "./src/argument_processors/argument_processor_check.h")
list (APPEND WSJCPP_SOURCES "./src/argument_processors/argument_processor_check.cpp")
list (APPEND WSJCPP_SOURCES "./src/argument_processors/argument_processor_clean.h")
list (APPEND WSJCPP_SOURCES "./src/argument_processors/argument_processor_clean.cpp")
list (APPEND WSJCPP_SOURCES "./src/argument_processors/argument_processor_ctf01d_main.h")
list (APPEND WSJCPP_SOURCES "./src/argument_processors/argument_processor_ctf01d_main.cpp")
list (APPEND WSJCPP_SOURCES "./src/argument_processors/argument_processor_start.h")
list (APPEND WSJCPP_SOURCES "./src/argument_processors/argument_processor_start.cpp")
list (APPEND WSJCPP_SOURCES "./src/argument_processors/argument_processor_teams.h")
list (APPEND WSJCPP_SOURCES "./src/argument_processors/argument_processor_teams.cpp")
list (APPEND WSJCPP_SOURCES "./src/argument_processors/argument_processor_version.h")
list (APPEND WSJCPP_SOURCES "./src/argument_processors/argument_processor_version.cpp")
list (APPEND WSJCPP_SOURCES "./src/argument_processors/argument_processor_web_test.h")
list (APPEND WSJCPP_SOURCES "./src/argument_processors/argument_processor_web_test.cpp")

# employees
list (APPEND WSJCPP_SOURCES "./src/service_locator/employ_team_logos.h")
list (APPEND WSJCPP_SOURCES "./src/service_locator/employ_team_logos.cpp")
list (APPEND WSJCPP_SOURCES "./src/service_locator/employ_flags.h")
list (APPEND WSJCPP_SOURCES "./src/service_locator/employ_flags.cpp")
list (APPEND WSJCPP_SOURCES "./src/service_locator/employ_scoreboard.h")
list (APPEND WSJCPP_SOURCES "./src/service_locator/employ_scoreboard.cpp")
list (APPEND WSJCPP_SOURCES "./src/service_locator/employ_config.h")
list (APPEND WSJCPP_SOURCES "./src/service_locator/employ_config.cpp")

# checker
list (APPEND WSJCPP_SOURCES "src/checker/dorunchecker.h")
list (APPEND WSJCPP_SOURCES "src/checker/dorunchecker.cpp")
list (APPEND WSJCPP_SOURCES "src/checker/service_checker_thread.h")
list (APPEND WSJCPP_SOURCES "src/checker/service_checker_thread.cpp")

# http_handlers
list (APPEND WSJCPP_SOURCES "src/http_handlers/http_handler_api_v1.h")
list (APPEND WSJCPP_SOURCES "src/http_handlers/http_handler_api_v1.cpp")
list (APPEND WSJCPP_SOURCES "src/http_handlers/http_handler_web_folder.h")
list (APPEND WSJCPP_SOURCES "src/http_handlers/http_handler_web_folder.cpp")
list (APPEND WSJCPP_SOURCES "./src/http_handlers/light_web_http_handler_team_logo.h")
list (APPEND WSJCPP_SOURCES "./src/http_handlers/light_web_http_handler_team_logo.cpp")

# scoreboard
list (APPEND WSJCPP_SOURCES "src/scoreboard/scoreboard.cpp")
list (APPEND WSJCPP_SOURCES "src/scoreboard/scoreboard.h")

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

add_executable (ctf01d ${WSJCPP_SOURCES})

target_link_libraries(ctf01d -lpthread ${WSJCPP_LIBRARIES} )

install(
    TARGETS
        ctf01d
    RUNTIME DESTINATION
        ${CMAKE_INSTALL_PREFIX}/bin
)


