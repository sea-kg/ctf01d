#ifndef ARGUMENT_PROCESSOR_VERSION_H
#define ARGUMENT_PROCESSOR_VERSION_H

#include <wsjcpp_arguments.h>

class ArgumentProcessorVersion : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorVersion();

        virtual bool applyParameterArgument(const std::string &sProgramName, const std::string &sArgumentName, const std::string &sValue);
        virtual bool applySingleArgument(const std::string &sProgramName, const std::string &sArgumentName);
        virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams);
};

#endif // ARGUMENT_PROCESSOR_VERSION_H
