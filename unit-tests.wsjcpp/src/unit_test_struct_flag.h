#ifndef UNIT_TEST_STRUCT_FLAG_H
#define UNIT_TEST_STRUCT_FLAG_H

#include <wsjcpp_unit_tests.h>

// Description: TODO
class UnitTestStructFlag : public WsjcppUnitTestBase {
    public:
        UnitTestStructFlag();
        virtual bool doBeforeTest() override;
        virtual void executeTest() override;
        virtual bool doAfterTest() override;
};

#endif // UNIT_TEST_STRUCT_FLAG_H

