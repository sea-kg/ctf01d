#ifndef ARGUMENT_PROCESSOR_SERVICES_H
#define ARGUMENT_PROCESSOR_SERVICES_H

#include <wsjcpp_arguments.h>

class ArgumentProcessorServices : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorServices();
};

class ArgumentProcessorServicesList : public WsjcppArgumentProcessor {
    public:
        ArgumentProcessorServicesList();

        virtual int exec(const std::vector<std::string> &vRoutes, const std::vector<std::string> &vSubParams) override;
};

#endif // ARGUMENT_PROCESSOR_SERVICES_H
