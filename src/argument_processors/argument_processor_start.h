#ifndef ARGUMENT_PROCESSOR_START_H
#define ARGUMENT_PROCESSOR_START_H

#include <wsjcpp_arguments.h>
#include <service_checker_thread.h>

class ArgumentProcessorStart : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorStart();

        virtual bool applyParameterArgument(const std::string &sProgramName, const std::string &sArgumentName, const std::string &sValue);
        virtual bool applySingleArgument(const std::string &sProgramName, const std::string &sArgumentName);
        virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams);

    private:
        std::vector<ServiceCheckerThread *> m_vThreads;
};

#endif // ARGUMENT_PROCESSOR_START_H
