#ifndef ARGUMENT_PROCESSOR_WEB_TEST_H
#define ARGUMENT_PROCESSOR_WEB_TEST_H

#include <wsjcpp_arguments.h>

class ArgumentProcessorWebTest : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorWebTest();

        virtual bool applyParameterArgument(const std::string &sProgramName, const std::string &sArgumentName, const std::string &sValue);
        virtual bool applySingleArgument(const std::string &sProgramName, const std::string &sArgumentName);
        virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams);
};

#endif // ARGUMENT_PROCESSOR_WEB_TEST_H
