#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <thread>

using namespace std;

class IOutputter {
public:
    virtual ~IOutputter(){};
    virtual void Output(vector<string> commands, size_t timestamp) = 0;
};

class CommandBlockConsoleOutputter : public IOutputter {
private:
public:
    void Output(vector<string> commands, size_t timestamp)
    {
        cout << "bulk:";
        char c = ' ';
        for (const auto& el:commands)
        {
            if (el == "{" || el == "}")
                continue;
            cout << c << el;
            c = ',';
        }
        cout << endl;
    }
};

class CommandBlockFileOutputter : public IOutputter {
private:
public:
    void Output(vector<string> commands, size_t timestamp)
    {
        ofstream f;
        string s = "bulk" + std::to_string(timestamp) + ".log";
        f.open(s);

        f << "bulk:";
        char c = ' ';
        for (const auto& el:commands)
        {
            if (el == "{" || el == "}")
                continue;
            f << c << el;
            c = ',';
        }
        f << endl;
        f.close();
    }
};

class CommandMemoryManager
{
private:
    size_t mCurBlockStartTimestamp;
    size_t mCurCommandIdx; //number of command that is being processed (does not contain brackets)
    size_t mN;
    int mEmbedBracers = 0;
    vector<string> mCommands;
    bool mAmDynBlock = false;

public:
    CommandMemoryManager(size_t N) : mN(N)
    {
        Refresh();
    }

    bool Interpret(string command)
    {
        if (mCurCommandIdx == 0) {
            const auto p1 = std::chrono::system_clock::now();
            mCurBlockStartTimestamp = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
        }

        if (command != "}" && command != "{") mCurCommandIdx++;
        mCommands.push_back(command); // does contain brackets

        if (command == "}") {
            if (--mEmbedBracers == 0) // that is a dyn block finish
                return true;
            return false;
        }

        if (command == "{") { // this is an emergency exit for the block
            mAmDynBlock = true;
            if ((++mEmbedBracers == 1)  && (mCurCommandIdx > 0))
                return true;
            return false;
        }

        if  ((mCurCommandIdx == mN) && (mEmbedBracers == 0)) // block is finished
        {
            return true;
        }
        return false;
    }

    void Refresh()
    {
        mCurCommandIdx = 0;
        mEmbedBracers = 0;
        if (mCommands.size() > 0 && mCommands[mCommands.size() - 1] == "{") {
            mEmbedBracers++;
            mAmDynBlock = true;
        }
        mCommands.clear();
        return;
    }

    size_t GetBlockStartTimestamp()
    {
        return mCurBlockStartTimestamp;
    }

    vector<string> GetCurrentBlock()
    {
        return mCommands;
    }

    bool IsDynBlock()
    {
        return mAmDynBlock;
    }

};

class InputCommandParser
{
public:
    InputCommandParser() = default;
    void InterpretInputs(std::ifstream& in, CommandMemoryManager& M)
    {
        std::cin.rdbuf(in.rdbuf());
        string command;

        for(std::string command; std::getline(in, command);)
        {
            bool is_complete = M.Interpret(command);
            if (is_complete)
            {
                //O.Output(M.GetCurrentBlock(), M.GetBlockStartTimestamp());
                NotifyOutputters(M);
                M.Refresh(); // empty buffers
            }
            std::this_thread::sleep_for(1s);
        }

        if (!M.IsDynBlock()) NotifyOutputters(M);
        return;
    }

    void Attach(IOutputter *observer) {
        mListOfSubscribers.push_back(observer);
    }
/*
    void Detach(IOutputter *observer) override {
        mListOfSubscribers.erase(observer);
    }
*/
    void NotifyOutputters(CommandMemoryManager& M) {
        for (const auto& o:mListOfSubscribers) {
            o->Output(M.GetCurrentBlock(), M.GetBlockStartTimestamp());
        }
    }

private:
    std::vector<IOutputter*> mListOfSubscribers;
};

