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



    //r3k2r/1p1pppbp/8/pPp3pP/4Q3/2P1N1P1/P2PPP2/R3K2R w KQkq a6 0 1
    pieces[Piece::Pawn][Color::White] = vector<Square>({8,33,18,11,12,13,22,39});
    pieces[Piece::Knight][Color::White] = vector<Square>({20});
    pieces[Piece::Bishop][Color::White] = vector<Square>();
    pieces[Piece::Rook][Color::White] = vector<Square>({0,7});
    pieces[Piece::Queen][Color::White] = vector<Square>({28});
    pieces[Piece::King][Color::White] = vector<Square>({4});

    pieces[Piece::Pawn][Color::Black] = vector<Square>({32,49,34,51,52,53,38,55});
    pieces[Piece::Knight][Color::Black] = vector<Square>();
    pieces[Piece::Bishop][Color::Black] = vector<Square>({54});
    pieces[Piece::Rook][Color::Black] = vector<Square>({56,63});
    pieces[Piece::Queen][Color::Black] = vector<Square>();
    pieces[Piece::King][Color::Black] = vector<Square>({61});

    castling[0][0] = castling[1][0] = true;
    castling[0][1] = castling[1][1] = true;

    tests.push_back(make_pair(Position(pieces,558619703697ULL,11671641680636805120ULL,Color::White, castling,(short)(40),(short)0,(short)1),52));

    //4R3/2Q5/6R1/1K1B1N2/3B4/3N4/P6P/1k6 w - - 0 1
    pieces[Piece::Pawn][Color::White] = vector<Square>({8,15});
    pieces[Piece::Knight][Color::White] = vector<Square>({19,37});
    pieces[Piece::Bishop][Color::White] = vector<Square>({27,35});
    pieces[Piece::Rook][Color::White] = vector<Square>({46,60});
    pieces[Piece::Queen][Color::White] = vector<Square>({50});
    pieces[Piece::King][Color::White] = vector<Square>({33});

    pieces[Piece::Pawn][Color::Black] = vector<Square>();
    pieces[Piece::Knight][Color::Black] = vector<Square>();
    pieces[Piece::Bishop][Color::Black] = vector<Square>();
    pieces[Piece::Rook][Color::Black] = vector<Square>();
    pieces[Piece::Queen][Color::Black] = vector<Square>();
    pieces[Piece::King][Color::Black] = vector<Square>({1});

    castling[0][0] = castling[1][0] = false;
    castling[0][1] = castling[1][1] = false;

    tests.push_back(make_pair(Position(pieces,1154117953781268736ULL,2ULL,Color::White, castling,(short)(-1),(short)0,(short)1),102));
}

void runTests()
{
    initializeAllBitBoards();
    initmagicmoves();

    for(int i = 0; i < tests.size(); i++)
    {
        cout << "Running test " << i << endl;

        auto answer = generateAllMoves(tests[i].first);

        for(int j = 0; j < answer.size(); j++)
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
