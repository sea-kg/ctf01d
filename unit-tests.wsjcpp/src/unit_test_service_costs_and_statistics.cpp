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

    int nBacisCostsStolenFlagInPoints = 10; // basic costs of stolen flag
    int nServicesSize = 2;
    int nTeamsSize = 2;
    int nStolenFlagsForService1 = 10;
    int nStolenFlagsForService2 = 10;
    int nAllStolenFlags = nStolenFlagsForService1 + nStolenFlagsForService2;

    Ctf01dServiceCostsAndStatistics test1("service1", nBacisCostsStolenFlagInPoints, nServicesSize, nTeamsSize);
    Ctf01dServiceCostsAndStatistics test2("service2", nBacisCostsStolenFlagInPoints, nServicesSize, nTeamsSize);

    // stollen flags
    compareN(bTestSuccess, "all stollen flags 0", test1.getAllStolenFlagsForService(), 0);
    compareD(bTestSuccess, "cost stollen flag", test1.getCostStolenFlag(), 10.0);

    test1.doIncrementStolenFlagsForService(1);
    compareN(bTestSuccess, "all stollen flags 1", test1.getAllStolenFlagsForService(), 1);

    test1.updateProportionalStolenFlagsForService(nStolenFlagsForService1, nAllStolenFlags);
    compareN(bTestSuccess, "all stollen flags 10", test1.getAllStolenFlagsForService(), 10);

    double s0 = test1.updateProportionalStolenFlagsForService(0, 10);
    compareD(bTestSuccess, "proportional 10", s0, 10.0);

    s0 = test1.updateProportionalStolenFlagsForService(nStolenFlagsForService1, nAllStolenFlags);
    compareD(bTestSuccess, "proportional 20 / 10", s0, 2.0);

    double nSumOfReverseProportionalStolenFlags = nAllStolenFlags / nStolenFlagsForService1 + nAllStolenFlags / nStolenFlagsForService2; // 4 
    double cost0 = test1.updateCostStolenFlag(nSumOfReverseProportionalStolenFlags);
    compareD(bTestSuccess, "cost stollen flag", cost0, 5.0);
    compareD(bTestSuccess, "cost stollen flag", test1.getCostStolenFlag(), 5.0);

    nStolenFlagsForService1 = 5;
    nStolenFlagsForService2 = 15;
    nAllStolenFlags = nStolenFlagsForService1 + nStolenFlagsForService2;

    s0 = test2.updateProportionalStolenFlagsForService(nStolenFlagsForService2, nAllStolenFlags);
    compareD(bTestSuccess, "proportional 20 / 15", s0, 20.0 / 15.0);

    nSumOfReverseProportionalStolenFlags = nAllStolenFlags / nStolenFlagsForService1 + nAllStolenFlags / nStolenFlagsForService2; // 4 
    double cost2 = test2.updateCostStolenFlag(nSumOfReverseProportionalStolenFlags);
    compareS(bTestSuccess, "cost stollen flag", std::to_string(cost2), std::to_string(2.666667));
    compareD(bTestSuccess, "cost stollen flag", test2.getCostStolenFlag(), cost2);

    // defence flags
    compareN(bTestSuccess, "all defence flags 0", test1.getAllDefenceFlagsForService(), 0);
    compareD(bTestSuccess, "cost stollen flag", test1.getCostDefenceFlag(), 20.0);

    test1.doIncrementDefenceFlagsForService(1);
    compareN(bTestSuccess, "all defence flags 1", test1.getAllDefenceFlagsForService(), 1);

    test1.updateProportionalDefenceFlagsForService(10, 10);
    compareN(bTestSuccess, "all defence flags 10", test1.getAllDefenceFlagsForService(), 10);

    double d0 = test1.updateProportionalDefenceFlagsForService(0, 10);
    compareD(bTestSuccess, "proportional 10", d0, 10.0);

    int nDefenceFlagsForService1 = 20;
    int nDefenceFlagsForService2 = 20;
    int nAllDefenceFlags = nDefenceFlagsForService1 + nDefenceFlagsForService2;
    
    s0 = test1.updateProportionalDefenceFlagsForService(nDefenceFlagsForService2, nAllDefenceFlags);
    compareD(bTestSuccess, "proportional 20 / 10", s0, 2.0);
    
    double nSumOfReverseProportionalDefenceFlags = nAllDefenceFlags / nDefenceFlagsForService1 + nAllDefenceFlags / nDefenceFlagsForService2; // 4 

    cost0 = test1.updateCostDefenceFlag(nSumOfReverseProportionalDefenceFlags);
    compareD(bTestSuccess, "cost defence flag 1", cost0, 10.0);
    compareD(bTestSuccess, "cost defence flag 2 ", test1.getCostDefenceFlag(), cost0);

    std::string sFirdtBloodTeamId = "team0";
    test1.setFirstBloodTeamId("team0");
    compareS(bTestSuccess, "first blood team id", test1.getFirstBloodTeamId(), sFirdtBloodTeamId);

    nlohmann::json jsonCosts;
    test1.updateJsonCosts(jsonCosts);

    std::string sJsonFirstBlood = jsonCosts["first_blood"];
    double nJsonCostAtt = jsonCosts["cost_att"];
    double nJsonCostDef = jsonCosts["cost_def"];
    int nJsonAfAtt = jsonCosts["af_att"];
    int nJsonAfDef = jsonCosts["af_def"];

    compareS(bTestSuccess, "first blood team id (2)", sJsonFirstBlood, sFirdtBloodTeamId);
    compareD(bTestSuccess, "cost defence flag 1 (2)", nJsonCostDef, 10.0);
    compareD(bTestSuccess, "cost stollen flag 1 (2)", nJsonCostAtt, 5.0);
    compareN(bTestSuccess, "all flags stollen (2)", nJsonAfAtt, 10);
    compareN(bTestSuccess, "all flags defence (2)", nJsonAfDef, 20);

    return bTestSuccess;
}

