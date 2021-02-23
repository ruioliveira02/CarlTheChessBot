#include "movegeneration.h"
#include "magicmoves.h"

#include <iostream>
#include <vector>

using namespace std;

vector<pair<Position,int> > tests;

void setupTests()
{
    vector<Square> pieces[6][2];


    bool castling [2][2];


    //rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8
    pieces[Piece::Pawn][Color::White] = vector<Square>({8,9,10,51,14,15});
    pieces[Piece::Knight][Color::White] = vector<Square>({1,12});
    pieces[Piece::Bishop][Color::White] = vector<Square>({2,26});
    pieces[Piece::Rook][Color::White] = vector<Square>({0,7});
    pieces[Piece::Queen][Color::White] = vector<Square>({3});
    pieces[Piece::King][Color::White] = vector<Square>({4});

    pieces[Piece::Pawn][Color::Black] = vector<Square>({48,49,42,53,54,55});
    pieces[Piece::Knight][Color::Black] = vector<Square>({57,13});
    pieces[Piece::Bishop][Color::Black] = vector<Square>({58,52});
    pieces[Piece::Rook][Color::Black] = vector<Square>({56,63});
    pieces[Piece::Queen][Color::Black] = vector<Square>({59});
    pieces[Piece::King][Color::Black] = vector<Square>({61});

    castling[0][0] = castling[1][0] = true;
    castling[0][1] = castling[1][1] = false;

    tests.push_back(make_pair(Position(pieces,2251799880849311ULL,12678481774024597504ULL,Color::White, castling,(short)(-1),(short)1,(short)8),44));

}

void runTests()
{
    initializeAllBitBoards();
    initmagicmoves();

    for(int i = 0; i < tests.size(); i++)
    {
        cout << "Running test " << i << endl;

        auto answer = generateAllMoves(tests[i].first);

        for(int j = 0; j < 43; j++)
            cout << answer[j].origin << " " << answer[j].destiny << endl;

        cout << answer.size() << endl;

        if(answer.size() == tests[i].second)
            cout << "Test passed" << endl;
        else
            cout << "Task failed successfully" << endl;
    }
}


int main()
{
    setupTests();

    runTests();

    return 0;
}
