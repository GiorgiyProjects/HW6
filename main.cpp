#include "CommandBlock.h"

int main(int argc, char** argv)
{
    /*
    std::ifstream in("../input/1.txt");
    std::cin.rdbuf(in.rdbuf());
    */

    size_t N = (argc > 1) ? atoi(argv[1]) : 3;

    CommandBlock a;
    IOManager<CommandBlock> manager(N);
    string command;

    for(std::string command; std::getline(cin, command);)
    {
        manager.Interpret(command, a);
        std::this_thread::sleep_for(1s);
    }
    manager.Output(a);
    manager.OutputToFile(a);
    return 0;
}