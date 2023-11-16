#include "unit_test_service_costs_and_statistics.h"
#include <vector>
#include <wsjcpp_core.h>
#include <employ_scoreboard.h>

REGISTRY_WSJCPP_UNIT_TEST(UnitTestServiceCostsAndStatistics)

UnitTestServiceCostsAndStatistics::UnitTestServiceCostsAndStatistics()
    : WsjcppUnitTestBase("UnitTestServiceCostsAndStatistics") {
}

// ---------------------------------------------------------------------

bool UnitTestServiceCostsAndStatistics::doBeforeTest() {
    // nothing
    return true;
}

// ---------------------------------------------------------------------

void UnitTestServiceCostsAndStatistics::executeTest() {
    int nBacisCostsStolenFlagInPoints = 10; // basic costs of stolen flag
    int nServicesSize = 2;
    int nTeamsSize = 2;
    int nStolenFlagsForService1 = 10;
    int nStolenFlagsForService2 = 10;
    int nAllStolenFlags = nStolenFlagsForService1 + nStolenFlagsForService2;

    Ctf01dServiceCostsAndStatistics test1("service1", nBacisCostsStolenFlagInPoints, nServicesSize, nTeamsSize);
    Ctf01dServiceCostsAndStatistics test2("service2", nBacisCostsStolenFlagInPoints, nServicesSize, nTeamsSize);

    // stollen flags
    compare("all stollen flags 0", test1.getAllStolenFlagsForService(), 0);
    compareD("cost stollen flag", test1.getCostStolenFlag(), 10.0);

    test1.doIncrementStolenFlagsForService(1);
    compare("all stollen flags 1", test1.getAllStolenFlagsForService(), 1);

    // defence flags
    compare("all defence flags 0", test1.getAllDefenceFlagsForService(), 0);

    int nDefenceFlagsForService1 = 20;
    int nDefenceFlagsForService2 = 20;
    int nAllDefenceFlags = nDefenceFlagsForService1 + nDefenceFlagsForService2;

    double nSumOfReverseProportionalDefenceFlags = nAllDefenceFlags / nDefenceFlagsForService1 + nAllDefenceFlags / nDefenceFlagsForService2; // 4 

    std::string sFirdtBloodTeamId = "team0";
    test1.setFirstBloodTeamId("team0");
    compare("first blood team id", test1.getFirstBloodTeamId(), sFirdtBloodTeamId);
}

// ---------------------------------------------------------------------

bool UnitTestServiceCostsAndStatistics::doAfterTest() {
    // nothing
    return true;
}