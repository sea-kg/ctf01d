#include <argument_processor_ctf01d_main.h>

// ---------------------------------------------------------------------

int main(int argc, const char* argv[]) {
    ArgumentProcessorCtf01dMain *pMain = new ArgumentProcessorCtf01dMain();
    WsjcppArguments prog(argc, argv, pMain);
    return prog.exec();
}
