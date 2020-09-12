#ifndef ARGUMENT_PROCESSOR_CHECK_H
#define ARGUMENT_PROCESSOR_CHECK_H

#include <wsjcpp_arguments.h>

class ArgumentProcessorCheck : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorCheck();

        virtual bool applyParameterArgument(const std::string &sProgramName, const std::string &sArgumentName, const std::string &sValue);
        virtual bool applySingleArgument(const std::string &sProgramName, const std::string &sArgumentName);
        virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams);
};

#endif // ARGUMENT_PROCESSOR_CHECK_H
