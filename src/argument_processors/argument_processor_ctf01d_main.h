#ifndef ARGUMENT_PROCESSOR_CTF01D_MAIN_H
#define ARGUMENT_PROCESSOR_CTF01D_MAIN_H

#include <wsjcpp_arguments.h>

class ArgumentProcessorCtf01dMain : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorCtf01dMain();

        virtual bool applyParameterArgument(const std::string &sProgramName, const std::string &sArgumentName, const std::string &sValue) override;
};

#endif // ARGUMENT_PROCESSOR_CTF01D_MAIN_H
