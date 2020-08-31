
#include "employ_scoreboard.h"
#include <wsjcpp_core.h>

// ---------------------------------------------------------------------
// EmployScoreboard

REGISTRY_WJSCPP_SERVICE_LOCATOR(EmployScoreboard)

EmployScoreboard::EmployScoreboard() 
: WsjcppEmployBase(EmployScoreboard::name(), {}) {
    TAG = EmployScoreboard::name();
}

// ---------------------------------------------------------------------

bool EmployScoreboard::init() {
    WsjcppLog::info(TAG, "init");
    return true;
}

// ---------------------------------------------------------------------

bool EmployScoreboard::deinit() {
    WsjcppLog::info(TAG, "deinit");
    return true;
}


