#ifndef UNIT_TEST_SERVICE_COSTS_AND_STATISTICS_H
#define UNIT_TEST_SERVICE_COSTS_AND_STATISTICS_H

#include <wsjcpp_unit_tests.h>

// Description: TODO
class UnitTestServiceCostsAndStatistics : public WsjcppUnitTestBase {
    public:
        UnitTestServiceCostsAndStatistics();
        virtual bool doBeforeTest() override;
        virtual void executeTest() override;
        virtual bool doAfterTest() override;
};

#endif // UNIT_TEST_SERVICE_COSTS_AND_STATISTICS_H

