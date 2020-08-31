#include "unit_test_struct_flag.h"
#include <vector>
#include <wsjcpp_core.h>
#include <employ_flags.h>

REGISTRY_WSJCPP_UNIT_TEST(UnitTestStructFlag)

UnitTestStructFlag::UnitTestStructFlag()
    : WsjcppUnitTestBase("UnitTestStructFlag") {
}

// ---------------------------------------------------------------------

void UnitTestStructFlag::init() {
    // nothing
}

// ---------------------------------------------------------------------

bool UnitTestStructFlag::run() {
    bool bTestSuccess = true;


    int nTimeFlagLifeInMin = 1;
    std::string sTeamId = "team1";
    std::string sServiceId = "service1";

    Ctf01dFlag flag;
    flag.generateRandomFlag(nTimeFlagLifeInMin, sTeamId, sServiceId);

    compareS(bTestSuccess, "teamid 1", flag.getTeamId(), sTeamId);
    compareS(bTestSuccess, "serviceid 1", flag.getServiceId(), sServiceId);
    long nFlagLifeTimeInMs = flag.getTimeEndInMs() - flag.getTimeStartInMs();
    compareN(bTestSuccess, "flag life time 1", nFlagLifeTimeInMs, nTimeFlagLifeInMin*60*1000);

    sTeamId = "team2";
    flag.setTeamId(sTeamId);
    compareS(bTestSuccess, "teamid 2", flag.getTeamId(), sTeamId);

    sServiceId = "service2";
    flag.setServiceId(sServiceId);
    compareS(bTestSuccess, "serviceid 2", flag.getServiceId(), sServiceId);

    long nStartTimeInMs = 100567622;
    flag.setTimeStartInMs(nStartTimeInMs);
    compareN(bTestSuccess, "start time in ms 2", nStartTimeInMs, flag.getTimeStartInMs());

    long nEndTimeInMs = 1005667621;
    flag.setTimeEndInMs(nEndTimeInMs);
    compareN(bTestSuccess, "end time in ms 2", nEndTimeInMs, flag.getTimeEndInMs());

    std::string sOldId = flag.getId();
    flag.generateId();
    compareB(bTestSuccess, "generateId 2", flag.getId() == sOldId, false);

    std::string sFlagId = "QWHzYEKuTX";
    flag.setId(sFlagId);
    compareS(bTestSuccess, "flag id 2", flag.getId(), sFlagId);
    
    std::string sOldValue = flag.getValue();
    flag.generateValue();
    compareB(bTestSuccess, "generateValue 2", flag.getValue() == sOldValue, false);

    std::string sFlagValue = "239ebbac-bb0f-a8b7-02fe-92887a064bf6";
    flag.setValue(sFlagValue);
    compareS(bTestSuccess, "flag value 2", flag.getValue() , sFlagValue);

    Ctf01dFlag flag3;
    flag3.copyFrom(flag);

    compareS(bTestSuccess, "flag id 3", flag3.getId() , sFlagId);
    compareS(bTestSuccess, "flag value 3", flag3.getValue() , sFlagValue);
    compareS(bTestSuccess, "teamid 3", flag3.getTeamId(), sTeamId);
    compareS(bTestSuccess, "serviceid 3", flag3.getServiceId(), sServiceId);
    compareN(bTestSuccess, "start time in ms 3", nStartTimeInMs, flag3.getTimeStartInMs());
    compareN(bTestSuccess, "end time in ms 3", nEndTimeInMs, flag3.getTimeEndInMs());

    return bTestSuccess;
}

