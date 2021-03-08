#include "movegeneration.h"
#include "magicmoves.h"

#include <iostream>
#include <vector>

using namespace std;

vector<pair<Position,pair<bool, bool> > > checkTests; //(position, is white in check, is black in check)


void setupCheckTests()
{
    checkTests.push_back({Position("7k/8/7Q/8/8/3q4/8/4K3 w - - 0 1"), {false, true}});
    checkTests.push_back({Position("8/8/8/8/8/8/1p4N1/K6k w - - 0 1"), {true, false}});
    checkTests.push_back({Position("rn2kb1r/1b3ppp/p2p1N2/qp6/3NP3/8/PPP2PPP/R2QKB1R w KQkq - 0 1"), {true, true}});
}


void runCheckTests()
{
    for(int i = 0; i < checkTests.size(); i++)
    {
        cout << "Running test " << i << endl;

        bool white = inCheck(checkTests[i].first, Color::White);
        bool black = inCheck(checkTests[i].first, Color::Black);

        if(white == checkTests[i].second.first)
            cout << "White correct: " << white << endl;
        else
            cout << "White wrong. Expected " << checkTests[i].second.first << endl;

        if(black == checkTests[i].second.second)
            cout << "Black correct: " << black << endl;
        else
            cout << "Black wrong. Expected " << checkTests[i].second.second << endl;
    }
}


int testChecks()
{
    initmagicmoves();
    initializeAllBitBoards();

    setupCheckTests();
    runCheckTests();
    return 0;
}
