#include <iostream>
#include "position.h"
#include "movegeneration.h"
#include "magicmoves.h"
#include "search.h"

using namespace std;

void printPosition(const Position& position)
{
    for (int y = 7; y >= 0; y--)
    {
        for (int x = 0; x < 8; x++)
        {
            for (int p = 0; p < 6; p++)
            {
                for (int c = 0; c < 2; c++)
                {
                    if (position.PieceBitBoards[p][c] & (1ULL << (8 * y + x)))
                    {
                        cout << pieceChar((Piece)p, (Color)c);
                        goto endstep;
                    }
                }
            }

            cout << ' ';
            endstep:
            continue;
        }

        cout << '\n';
    }

    cout.flush();
}

int main()
{
    //r3k2r/1p1pppbp/8/pPp3pP/4Q3/2P1N1P1/P2PPP2/R3K2R w KQkq a6 0 1
    std::vector<Square> pieces[6][2];
    pieces[Piece::Pawn][Color::White] = vector<Square>();
    pieces[Piece::Knight][Color::White] = vector<Square>();
    pieces[Piece::Bishop][Color::White] = vector<Square>();
    pieces[Piece::Rook][Color::White] = vector<Square>();
    pieces[Piece::Queen][Color::White] = vector<Square>({55});
    pieces[Piece::King][Color::White] = vector<Square>({44});

    pieces[Piece::Pawn][Color::Black] = vector<Square>();
    pieces[Piece::Knight][Color::Black] = vector<Square>();
    pieces[Piece::Bishop][Color::Black] = vector<Square>();
    pieces[Piece::Rook][Color::Black] = vector<Square>();
    pieces[Piece::Queen][Color::Black] = vector<Square>();
    pieces[Piece::King][Color::Black] = vector<Square>({58});

    bool castling[2][2];

    castling[0][0] = castling[1][0] = false;
    castling[0][1] = castling[1][1] = false;

    Position pos = Position(pieces,36046389205008384ULL, 288230376151711744ULL,Color::White, castling,(short)(40),(short)0,(short)1);


    game g = game();
    g.initialize();
    g.position = pos;

    printPosition(pos);
    cout << "\n\n";

    auto res = search(g);
    cout << endl;

    return 0;
}
