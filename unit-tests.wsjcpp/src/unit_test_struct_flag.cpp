#include "unit_test_struct_flag.h"
#include <vector>
#include <wsjcpp_core.h>
#include <employ_flags.h>

REGISTRY_WSJCPP_UNIT_TEST(UnitTestStructFlag)

UnitTestStructFlag::UnitTestStructFlag()
    : WsjcppUnitTestBase("UnitTestStructFlag") {
}

// ---------------------------------------------------------------------

bool UnitTestStructFlag::doBeforeTest() {
    // nothing
    return true;
}

// ---------------------------------------------------------------------

void UnitTestStructFlag::executeTest() {

    int nTimeFlagLifeInMin = 1;
    std::string sTeamId = "team1";
    std::string sServiceId = "service1";

    Ctf01dFlag flag;
    flag.generateRandomFlag(nTimeFlagLifeInMin, sTeamId, sServiceId);

    compare("teamid 1", flag.getTeamId(), sTeamId);
    compare("serviceid 1", flag.getServiceId(), sServiceId);
    long nFlagLifeTimeInMs = flag.getTimeEndInMs() - flag.getTimeStartInMs();
    compare("flag life time 1", nFlagLifeTimeInMs, nTimeFlagLifeInMin*60*1000);

    sTeamId = "team2";
    flag.setTeamId(sTeamId);
    compare("teamid 2", flag.getTeamId(), sTeamId);

    sServiceId = "service2";
    flag.setServiceId(sServiceId);
    compare("serviceid 2", flag.getServiceId(), sServiceId);

    long nStartTimeInMs = 100567622;
    flag.setTimeStartInMs(nStartTimeInMs);
    compare("start time in ms 2", nStartTimeInMs, flag.getTimeStartInMs());

    long nEndTimeInMs = 1005667621;
    flag.setTimeEndInMs(nEndTimeInMs);
    compare("end time in ms 2", nEndTimeInMs, flag.getTimeEndInMs());

    std::string sOldId = flag.getId();
    flag.generateId();
    compare("generateId 2", flag.getId() == sOldId, false);

    std::string sFlagId = "QWHzYEKuTX";
    flag.setId(sFlagId);
    compare("flag id 2", flag.getId(), sFlagId);
    
    std::string sOldValue = flag.getValue();
    flag.generateValue();
    compare("generateValue 2", flag.getValue() == sOldValue, false);

    std::string sFlagValue = "239ebbac-bb0f-a8b7-02fe-92887a064bf6";
    flag.setValue(sFlagValue);
    compare("flag value 2", flag.getValue() , sFlagValue);

    Ctf01dFlag flag3;
    flag3.copyFrom(flag);

    compare("flag id 3", flag3.getId() , sFlagId);
    compare("flag value 3", flag3.getValue() , sFlagValue);
    compare("teamid 3", flag3.getTeamId(), sTeamId);
    compare("serviceid 3", flag3.getServiceId(), sServiceId);
    compare("start time in ms 3", nStartTimeInMs, flag3.getTimeStartInMs());
    compare("end time in ms 3", nEndTimeInMs, flag3.getTimeEndInMs());

}

// ---------------------------------------------------------------------

bool UnitTestStructFlag::doAfterTest() {
    // nothing
    return true;
}