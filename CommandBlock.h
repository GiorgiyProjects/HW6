#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <thread>

using namespace std;

template <class T>
class IOManager
{
private:
    size_t mCurBlockStartTimestamp;
    size_t mCurCommandIdx; //number of command that is being processed now
    size_t mN;
    bool mExpectClosingBracket;
    int mEmbedBracers = 0;

public:
    IOManager(size_t N) : mN(N), mCurCommandIdx(0), mExpectClosingBracket(false)
    {
    }

    void Interpret(string command, T& block)
    {
        if (mCurCommandIdx == 0) {
            const auto p1 = std::chrono::system_clock::now();
            mCurBlockStartTimestamp = std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
        }
        if (command == "}") {
            if (--mEmbedBracers == 0)
                mExpectClosingBracket = false;
            return;
        }
        if (command == "{") {
            if (++mEmbedBracers > 1) //embedded bracket
                return;
            Output(block); // new block start
            OutputToFile(block);
            Refresh();
            block.clear();
            mExpectClosingBracket = true;
            return;
        }

        if  ((mCurCommandIdx == mN) && (mEmbedBracers == 0)) // block is finished and
        {
            Output(block);
            OutputToFile(block);
            Refresh();
            block.clear();
        }

        mCurCommandIdx++;
        block.push_back(command);
        return;
    }

    void Refresh()
    {
        mCurCommandIdx = 0;
        mExpectClosingBracket = false;
    }

    void Output(const T& block)
    {
        if (!mExpectClosingBracket)
            std::cout << block;
    }

    void OutputToFile(const T& block)
    {
        if (mExpectClosingBracket) return;
        ofstream f;
        string s = "bulk" + std::to_string(mCurBlockStartTimestamp) + ".log";
        f.open(s);
        f << block;
        f.close();
    }
};

class CommandBlock
{
public:
    vector<string> mCommands;
public:
    CommandBlock() = default;
    void push_back(std::string command)
    {
        mCommands.push_back(command);
    }

    void clear()
    {
        mCommands.clear();
    }
};

ostream& operator<<(ostream& os, const CommandBlock& block)
{
    os << "bulk:";
    char c = ' ';
    for (const auto& el:block.mCommands)
    {
        os << c << el;
        c = ',';
    }
    cout << endl;
}