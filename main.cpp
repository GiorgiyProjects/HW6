#include "CommandBlock.h"

int main(int argc, char** argv)
{
    std::ifstream in("../input/1.txt");
    size_t N = (argc > 1) ? atoi(argv[1]) : 3;
    CommandMemoryManager M(N);
    CommandBlockOutputter O;
    InputCommandParser I(in, M, O);
    return 0;
}