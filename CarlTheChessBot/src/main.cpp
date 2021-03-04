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

bool readPiece(char c, pair<Piece, Color>& r)
{
    Color color;

    if (c >= 'a')
    {
        c -= 'a' - 'A';
        color = Color::Black;
    }
    else
        color = Color::White;

    switch (c)
    {
        case 'P':   r = make_pair(Piece::Pawn, color);      return true;
        case 'N':   r = make_pair(Piece::Knight, color);    return true;
        case 'B':   r = make_pair(Piece::Bishop, color);    return true;
        case 'R':   r = make_pair(Piece::Rook, color);      return true;
        case 'Q':   r = make_pair(Piece::Queen, color);     return true;
        case 'K':   r = make_pair(Piece::King, color);      return true;
        default:    return false;
    }
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

    //r3k2r/1p1pppbp/8/pPp3pP/4Q3/2P1N1P1/P2PPP2/R3K2R w KQkq a6 0 1
    std::vector<Square> pieces[6][2];
    pieces[Piece::Pawn][Color::White] = vector<Square>({8, 9, 10, 11, 12, 13, 14, 15});
    pieces[Piece::Knight][Color::White] = vector<Square>({1, 6});
    pieces[Piece::Bishop][Color::White] = vector<Square>({2, 5});
    pieces[Piece::Rook][Color::White] = vector<Square>({0, 7});
    pieces[Piece::Queen][Color::White] = vector<Square>({3});
    pieces[Piece::King][Color::White] = vector<Square>({4});

    pieces[Piece::Pawn][Color::Black] = vector<Square>({48, 49, 50, 51, 52, 53, 54, 55});
    pieces[Piece::Knight][Color::Black] = vector<Square>({57, 62});
    pieces[Piece::Bishop][Color::Black] = vector<Square>({58, 61});
    pieces[Piece::Rook][Color::Black] = vector<Square>({56, 63});
    pieces[Piece::Queen][Color::Black] = vector<Square>({59});
    pieces[Piece::King][Color::Black] = vector<Square>({60});

    bool castling[2][2];

    castling[0][0] = castling[1][0] = true;
    castling[0][1] = castling[1][1] = true;

    Position pos = Position(pieces,65535ULL, 18446462598732840960ULL,Color::White, castling,(short)(-1),(short)0,(short)1);
    game g = game();
    g.position = pos;


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
