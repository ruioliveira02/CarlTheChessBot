#include "movegeneration.h"
#include "magicmoves.h"

#include <iostream>
#include <vector>

using namespace std;

vector<pair<Position,int> > tests;

void setupTests()
{
    tests.push_back(make_pair(Position("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"),44));
    tests.push_back(make_pair(Position("r3k2r/1p1pppbp/8/pPp3pP/4Q3/2P1N1P1/P2PPP2/R3K2R w KQkq a6 0 1"),52));
    tests.push_back(make_pair(Position("4R3/2Q5/6R1/1K1B1N2/3B4/3N4/P6P/1k6 w - - 0 1"),102));
    tests.push_back(make_pair(Position("k7/8/8/6pP/7P/8/P6P/7K w - g6 0 1"), 9));
}

void runMoveGenerationTests()
{
    initializeAllBitBoards();
    initmagicmoves();

    for(int i = 0; i < tests.size(); i++)
    {
        cout << "Running test " << i << endl;

        auto answer = generateAllMoves(tests[i].first);

        for(int j = 0; j < answer.second; j++)
            cout << answer.first[j].origin << " " << answer.first[j].destiny << endl;

        cout << answer.second << endl;

        if(answer.second == tests[i].second)
            cout << "Test passed" << endl;
        else
            cout << "Task failed successfully" << endl;

        delete[] answer.first;
    }
}


int main()//testMoveGeneration()
{
    setupTests();

    runMoveGenerationTests();

    return 0;
}
