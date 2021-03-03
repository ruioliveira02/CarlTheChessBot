#include "movegeneration.h"
#include "magicmoves.h"

#include <iostream>
#include <vector>

using namespace std;

vector<pair<Position,pair<bool, bool> > > checkTests; //(position, is white in check, is black in check)


void setupCheckTests()
{
    bool castling[2][2];
    vector<Square> pieces[6][2];


    //7k/8/7Q/8/8/3q4/8/4K3 w - - 0 1
    for(int i = 0 ; i < 2; i++)
        for(int j = 0; j < 2; j++)
            castling[i][j] = false;

    pieces[Piece::Pawn][Color::White] = vector<Square>();
    pieces[Piece::Knight][Color::White] = vector<Square>();
    pieces[Piece::Bishop][Color::White] = vector<Square>();
    pieces[Piece::Rook][Color::White] = vector<Square>();
    pieces[Piece::Queen][Color::White] = vector<Square>({47});
    pieces[Piece::King][Color::White] = vector<Square>({4});

    pieces[Piece::Pawn][Color::Black] = vector<Square>();
    pieces[Piece::Knight][Color::Black] = vector<Square>();
    pieces[Piece::Bishop][Color::Black] = vector<Square>();
    pieces[Piece::Rook][Color::Black] = vector<Square>();
    pieces[Piece::Queen][Color::Black] = vector<Square>({19});
    pieces[Piece::King][Color::Black] = vector<Square>({63});

    checkTests.push_back({Position(pieces, 140737488355344ULL, 9223372036855300096ULL, Color::Black, castling, 0, 0, 0), {false, true}});


    //8/8/8/8/8/8/1p4N1/K6k w - - 0 1
    pieces[Piece::Pawn][Color::White] = vector<Square>();
    pieces[Piece::Knight][Color::White] = vector<Square>({14});
    pieces[Piece::Bishop][Color::White] = vector<Square>();
    pieces[Piece::Rook][Color::White] = vector<Square>();
    pieces[Piece::Queen][Color::White] = vector<Square>();
    pieces[Piece::King][Color::White] = vector<Square>({0});

    pieces[Piece::Pawn][Color::Black] = vector<Square>({9});
    pieces[Piece::Knight][Color::Black] = vector<Square>();
    pieces[Piece::Bishop][Color::Black] = vector<Square>();
    pieces[Piece::Rook][Color::Black] = vector<Square>();
    pieces[Piece::Queen][Color::Black] = vector<Square>();
    pieces[Piece::King][Color::Black] = vector<Square>({7});

    checkTests.push_back({Position(pieces, 16385ULL, 640ULL, Color::White, castling, 0, 0, 0), {true, false}});


    //rn2kb1r/1b3ppp/p2p1N2/qp6/3NP3/8/PPP2PPP/R2QKB1R w KQkq - 0 1
    for(int i = 0 ; i < 2; i++)
        for(int j = 0; j < 2; j++)
            castling[i][j] = true;

    pieces[Piece::Pawn][Color::White] = vector<Square>({8,9,10,28,13,14,15});
    pieces[Piece::Knight][Color::White] = vector<Square>({27,45});
    pieces[Piece::Bishop][Color::White] = vector<Square>({5});
    pieces[Piece::Rook][Color::White] = vector<Square>({0,7});
    pieces[Piece::Queen][Color::White] = vector<Square>({3});
    pieces[Piece::King][Color::White] = vector<Square>({4});

    pieces[Piece::Pawn][Color::Black] = vector<Square>({9});
    pieces[Piece::Knight][Color::Black] = vector<Square>({57});
    pieces[Piece::Bishop][Color::Black] = vector<Square>({49,61});
    pieces[Piece::Rook][Color::Black] = vector<Square>({56,63});
    pieces[Piece::Queen][Color::Black] = vector<Square>({32});
    pieces[Piece::King][Color::Black] = vector<Square>({60});

    checkTests.push_back({Position(pieces, 35184774801337ULL, 12898872287037489664ULL, Color::White, castling, 0, 0, 0), {true, true}});
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


int main()
{
    initmagicmoves();
    initializeAllBitBoards();

    setupCheckTests();
    runCheckTests();
    return 0;
}
