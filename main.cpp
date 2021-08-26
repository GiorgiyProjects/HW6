#include "CommandBlock.h"

int main(int argc, char** argv)
{
    std::ifstream in("../input/1.txt");
    size_t N = (argc > 1) ? atoi(argv[1]) : 3;
    CommandMemoryManager M(N);
    InputCommandParser I;
    CommandBlockConsoleOutputter* O1 = new CommandBlockConsoleOutputter();
    I.Attach(O1);
    CommandBlockFileOutputter* O2 = new CommandBlockFileOutputter();
    I.Attach(O2);
    I.InterpretInputs(in, M);
    return 0;
}