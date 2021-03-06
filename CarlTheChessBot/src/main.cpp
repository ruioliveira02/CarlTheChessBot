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

Move readMove(game g)
{
    begin:

    Move m;
    string s;
    cin >> s;

    if (s == "O-O" || s == "O-O-O")
    {
        m.type = MoveType::Castling;
        m.piece = Piece::King;
        m.origin = g.position.ToMove == Color::White ? 4 : 59;
        m.destiny = m.origin + (s == "O-O" ? 2 : (-2));
    }
    else if (s[2] == '=')
    {
        pair<Piece, Color> p;

        if (!readPiece(s[3], p) || p.second != g.position.ToMove)
            goto begin;

        m.type = MoveType::Promotion;
        m.destiny = (s[1] - '1') * 8 + (s[0] - 'a');
        m.origin = m.destiny - 8;
        m.piece = p.first;
    }
    else if (s.size() >= 5)
    {
        pair<Piece, Color> p;

        if (!readPiece(s[2], p) || p.second != g.position.ToMove)
            goto begin;

        m.piece = p.first;
        m.origin = (s[1] - '1') * 8 + (s[0] - 'a');
        m.destiny = (s[4] - '1') * 8 + (s[3] - 'a');

        //TODO
        //if (game.position.getPiece(m.origin) != m.piece)
        //    goto begin;

        //TODO: verificar se o move é legal

        if (m.piece == Piece::Pawn && m.origin % 8 != m.destiny % 8
                && (g.position.ToMove == Color::White ? g.position.WhiteOccupancy : g.position.BlackOccupancy)
                    & (1 << m.destiny) == 0)
            m.type = MoveType::EnPassant;
        else
            m.type = MoveType::Normal;
    }
    else
        goto begin;

    return m;
}


int main()
{
    game::initialize();

    Position pos("r3k2r/1p1pppbp/8/pPp3pP/4Q3/2P1N1P1/P2PPP2/R3K2R w KQkq a6 0 1");
    game g = game();
    g.position = pos;

    cout << pos.ToMove << endl;

    while (1)
    {
        cout << "\n";
        printPosition(g.position);
        cout << "\n";

        cout << "White: ";
        Move m = readMove(g);
        g = g.makeMove(m);

        cout << "\n";
        printPosition(g.position);
        cout << "\n";

        auto res = search(g);
        cout << "Black: " << res.first.toString(Color::Black) << endl;
        g = g.makeMove(res.first);
    }

    return 0;
}
