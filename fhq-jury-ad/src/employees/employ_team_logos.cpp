
#include "employ_team_logos.h"
#include <wsjcpp_core.h>

// ---------------------------------------------------------------------
// EmployTeamLogos

REGISTRY_WJSCPP_EMPLOY(EmployTeamLogos)

EmployTeamLogos::EmployTeamLogos() 
: WsjcppEmployBase(EmployTeamLogos::name(), {}) {
    TAG = EmployTeamLogos::name();
}

// ---------------------------------------------------------------------

bool EmployTeamLogos::init() {
    WsjcppLog::info(TAG, "init");
    return true;
}

// ---------------------------------------------------------------------

bool EmployTeamLogos::deinit() {
    WsjcppLog::info(TAG, "deinit");
    return true;
}


