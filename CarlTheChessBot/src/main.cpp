#include <iostream>
#include "position.h"
#include "movegeneration.h"
#include "magicmoves.h"
#include "search.h"

using namespace std;

int main()
{
    //r3k2r/1p1pppbp/8/pPp3pP/4Q3/2P1N1P1/P2PPP2/R3K2R w KQkq a6 0 1
    std::vector<Square> pieces[6][2];
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

    bool castling[2][2];

    castling[0][0] = castling[1][0] = true;
    castling[0][1] = castling[1][1] = true;


    Position pos = Position(pieces,558619703697ULL,11671641680636805120ULL,Color::White, castling,(short)(40),(short)0,(short)1);

    game g = game();
    g.position = pos;

    auto res = search(g);

    return 0;
}
