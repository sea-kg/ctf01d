#include "unit_test_service_costs_and_statistics.h"
#include <vector>
#include <wsjcpp_core.h>
#include <employ_scoreboard.h>

REGISTRY_WSJCPP_UNIT_TEST(UnitTestServiceCostsAndStatistics)

UnitTestServiceCostsAndStatistics::UnitTestServiceCostsAndStatistics()
    : WsjcppUnitTestBase("UnitTestServiceCostsAndStatistics") {
}

// ---------------------------------------------------------------------

void UnitTestServiceCostsAndStatistics::init() {
    // nothing
}

// ---------------------------------------------------------------------

bool UnitTestServiceCostsAndStatistics::run() {
    bool bTestSuccess = true;

    int nStolenPoints = 10; // basic costs of stolen flag
    int nServicesSize = 2;
    int nDefencePoints = nStolenPoints * nServicesSize;

    Ctf01dServiceCostsAndStatistics test1("service1");
    Ctf01dServiceCostsAndStatistics test2("service2");

    // stollen flags
    compareN(bTestSuccess, "all stollen flags 0", test1.allStolenFlagsForService(), 0);
   
    test1.incrementStolenFlagsForService();
    compareN(bTestSuccess, "all stollen flags 1", test1.allStolenFlagsForService(), 1);

    test1.setStolenFlagsForService(10);
    compareN(bTestSuccess, "all stollen flags 10", test1.allStolenFlagsForService(), 10);

    test1.setStolenFlagsForService(0);
    double s0 = test1.updateProportionalStolenFlagsForService(10);
    compareD(bTestSuccess, "proportional 10", s0, 10.0);
    
    int nStolenFlagsForService1 = 10;
    int nStolenFlagsForService2 = 10;
    int nAllStolenFlags = nStolenFlagsForService1 + nStolenFlagsForService2;
    

    test1.setStolenFlagsForService(nStolenFlagsForService1);
    s0 = test1.updateProportionalStolenFlagsForService(nAllStolenFlags);
    compareD(bTestSuccess, "proportional 20 / 10", s0, 2.0);

    double nSumOfReverseProportionalStolenFlags = nAllStolenFlags / nStolenFlagsForService1 + nAllStolenFlags / nStolenFlagsForService2; // 4 
    double cost0 = test1.updateCostStolenFlagForService(nStolenPoints, nSumOfReverseProportionalStolenFlags);
    compareD(bTestSuccess, "cost stollen flag", cost0, 5.0);
    compareD(bTestSuccess, "cost stollen flag", test1.costStolenFlag(), 5.0);

    nStolenFlagsForService1 = 5;
    nStolenFlagsForService2 = 15;
    nAllStolenFlags = nStolenFlagsForService1 + nStolenFlagsForService2;

    test2.setStolenFlagsForService(nStolenFlagsForService2);
    s0 = test2.updateProportionalStolenFlagsForService(nAllStolenFlags);
    compareD(bTestSuccess, "proportional 20 / 15", s0, 20.0 / 15.0);

    nSumOfReverseProportionalStolenFlags = nAllStolenFlags / nStolenFlagsForService1 + nAllStolenFlags / nStolenFlagsForService2; // 4 
    double cost2 = test2.updateCostStolenFlagForService(nStolenPoints, nSumOfReverseProportionalStolenFlags);
    compareS(bTestSuccess, "cost stollen flag", std::to_string(cost2), std::to_string(2.666667));
    compareD(bTestSuccess, "cost stollen flag", test2.costStolenFlag(), cost2);

    // defence flags
    compareN(bTestSuccess, "all defence flags 0", test1.allDefenceFlagsForService(), 0);
   
    test1.incrementDefenceFlagsForService();
    compareN(bTestSuccess, "all defence flags 1", test1.allDefenceFlagsForService(), 1);

    test1.setDefenceFlagsForService(10);
    compareN(bTestSuccess, "all defence flags 10", test1.allDefenceFlagsForService(), 10);

    test1.setDefenceFlagsForService(0);
    double d0 = test1.updateProportionalDefenceFlagsForService(10);
    compareD(bTestSuccess, "proportional 10", d0, 10.0);

    int nDefenceFlagsForService1 = 10;
    int nDefenceFlagsForService2 = 10;
    int nAllDefenceFlags = nDefenceFlagsForService1 + nDefenceFlagsForService2;
    
    
    test1.setDefenceFlagsForService(nDefenceFlagsForService1);
    s0 = test1.updateProportionalStolenFlagsForService(nAllDefenceFlags);
    compareD(bTestSuccess, "proportional 20 / 10", s0, 2.0);

    double nSumOfReverseProportionalDefenceFlags = nAllDefenceFlags / nDefenceFlagsForService1 + nAllDefenceFlags / nDefenceFlagsForService2; // 4 
    cost0 = test1.updateCostDefenceFlagForService(nDefencePoints, nSumOfReverseProportionalDefenceFlags);
    compareD(bTestSuccess, "cost stollen flag", cost0, 50.0);
    compareD(bTestSuccess, "cost stollen flag", test1.costDefenceFlag(), 50.0);

    std::string sFirdtBloodTeamId = "team0";
    test1.setFirstBloodTeamId("team0");
    compareS(bTestSuccess, "first blood team id", test1.getFirstBloodTeamId(), sFirdtBloodTeamId);

    return bTestSuccess;
}

