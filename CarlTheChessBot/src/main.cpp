#include <iostream>
#include "position.h"
#include "movegeneration.h"
#include "magicmoves.h"
#include "search.h"
#include "UCI.h"

using namespace std;

void printPosition(const Position& position)
{
    for (int y = 7; y >= 0; y--)
    {
        for (int x = 0; x < 8; x++)
            cout << ((x + y) % 2 == 0 ? "      " : "██████");

        cout << '\n';

        for (int x = 0; x < 8; x++)
        {
            cout << ((x + y) % 2 == 0 ? "   " : "███");

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

            cout << ((x + y) % 2 == 0 ? " " : "█");
            endstep:

            cout << ((x + y) % 2 == 0 ? "  " : "██");
            continue;
        }

        cout << '\n';

        for (int x = 0; x < 8; x++)
            cout << ((x + y) % 2 == 0 ? "      " : "██████");

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

    if (s == "0-0" || s == "0-0-0")
    {
        m.type = MoveType::Castling;
        m.piece = Piece::King;
        m.origin = g.position.ToMove == Color::White ? 4 : 59;
        m.destiny = m.origin + (s == "0-0" ? 2 : (-2));
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
{/*
    //std::ofstream out("out.txt");
    //std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!

    game::initialize();
    cout << "Loaded!\n" << endl;

    game g = game();
    g.position = Position("8/4k3/6n1/8/8/P2P3P/2B1K3/8 w - - 0 1");

    //auto a = search(g);
    //game g2 = g.makeMove(a.first);
    //search(g2);
    //return 0;

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

        auto res = search(g, 10000, 2);
        cout << "Black: " << res.first.toString(Color::Black) << endl;
        g = g.makeMove(res.first);
    }*/

    UCI uci = UCI();
    uci.run();

    return 0;
}
