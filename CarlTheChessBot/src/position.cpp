#include "position.h"

#include <vector>
#include <iostream>
#include <sstream>
#include <cstring>

char pieceChar(Piece piece, Color color)
{
    char c;

    switch (piece)
    {
        case Piece::Pawn:       c = 'p';    break;
        case Piece::Knight:     c = 'n';    break;
        case Piece::Bishop:     c = 'b';    break;
        case Piece::Rook:       c = 'r';    break;
        case Piece::Queen:      c = 'q';    break;
        case Piece::King:       c = 'k';    break;
        default:                c = '?';    break;
    }

    if (color == Color::White)
        c += 'A' - 'a';

    return c;
}

bool readPiece(char c, std::pair<Piece, Color>& r)
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
        case 'P':   r = std::make_pair(Piece::Pawn, color);      return true;
        case 'N':   r = std::make_pair(Piece::Knight, color);    return true;
        case 'B':   r = std::make_pair(Piece::Bishop, color);    return true;
        case 'R':   r = std::make_pair(Piece::Rook, color);      return true;
        case 'Q':   r = std::make_pair(Piece::Queen, color);     return true;
        case 'K':   r = std::make_pair(Piece::King, color);      return true;
        default:    return false;
    }
}

std::string Move::toString(Color color)
{
    switch (type)
    {

        case MoveType::Normal:
        case MoveType::EnPassant:
        return std::string() + (char)(origin % 8 + 'a') + (char)(origin / 8 + '1') + pieceChar(piece, color)
                  + (char)(destiny % 8 + 'a') + (char)(destiny / 8 + '1');

        case MoveType::Castling:
        return destiny % 8 == 6 ? "O-O" : "O-O-O";

        case MoveType::Promotion:
        return std::string() + (char)(origin % 8 + 'a') + (char)(origin / 8 + '1') + '=' + pieceChar(piece, color);

        default: return "^_^";
    }
}

Position::Position()
{
}


Position::Position(const char* fen)
{
    std::pair<Piece, Color> p;
    int y = 7;
    int x = 0;

    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 2; j++)
            PieceBitBoards[i][j] = 0ULL;

    while (1)
    {
        if (readPiece(*fen, p))
        {
            PieceBitBoards[p.first][p.second] |= (1ULL << (8 * y + x));
            x++;
        }
        else if ('1' <= *fen && *fen <= '8')
            x += *fen - '0';
        else //if (*fen == '/' || *fen == ' ')
        {
            y--;
            x = 0;
        }

        fen++;

        if (y < 0)
            break;
    }

    ToMove = *fen == 'w' ? Color::White : Color::Black;
    fen += 2;

    if (*fen == 'K')
    {
        Castling[0][Color::White] = true;
        fen++;
    }

    if (*fen == 'Q')
    {
        Castling[1][Color::White] = true;
        fen++;
    }

    if (*fen == 'k')
    {
        Castling[0][Color::Black] = true;
        fen++;
    }

    if (*fen == 'q')
    {
        Castling[1][Color::Black] = true;
        fen++;
    }

    fen += *fen == '-' ? 2 : 1;

    if (*fen == '-')
    {
        EnPassant = -1;
        fen += 2;
    }
    else
    {
        EnPassant = (*(fen + 1) - '1') * 8 + (*fen - 'a');
        fen += 3;
    }

    std::stringstream stream;
    stream << fen;

    stream >> HalfMoves;
    stream >> TotalMoves;

    WhiteOccupancy = 0ULL;
    BlackOccupancy = 0ULL;

    for (int i = 0; i < 6; i++)
    {
        WhiteOccupancy |= PieceBitBoards[i][Color::White];
        BlackOccupancy |= PieceBitBoards[i][Color::Black];
    }
}


Position::Position(std::vector<Square> pieces [6][2],Color color,bool castling[2][2], short enpassant,short halfMoves,short totalMoves)
{
    EnPassant = enpassant;
    ToMove = color;
    HalfMoves = halfMoves;
    TotalMoves = totalMoves;

    for(int i = 0; i < 2; i++)
        for(int j = 0; j < 2; j++)
            Castling[i][j] = castling[i][j];

    for(int i = 0; i < 6; i++)
        for(int j = 0; j < 2; j++)
            PieceBitBoards[i][j] = 0ULL;

    for(int i = 0; i < 6; i++)
        for(int j = 0; j < 2; j++)
            for(int k = 0; k < pieces[i][j].size(); k++)
                PieceBitBoards[i][j] |= (1ULL << (pieces[i][j][k]));

    WhiteOccupancy = 0ULL;
    BlackOccupancy = 0ULL;

    for (int i = 0; i < 6; i++)
    {
        WhiteOccupancy |= PieceBitBoards[i][Color::White];
        BlackOccupancy |= PieceBitBoards[i][Color::Black];
    }
}

Position::Position(const Position& pos)
{
    EnPassant = pos.EnPassant;
    ToMove = pos.ToMove;
    HalfMoves = pos.HalfMoves;
    TotalMoves = pos.TotalMoves;

    WhiteOccupancy = pos.WhiteOccupancy;
    BlackOccupancy = pos.BlackOccupancy;

    for(int i = 0; i < 2; i++)
        for(int j = 0; j < 2; j++)
            Castling[i][j] = pos.Castling[i][j];

    Castling[0][0] = pos.Castling[0][0];
    Castling[0][1] = pos.Castling[0][1];
    Castling[1][0] = pos.Castling[1][0];
    Castling[1][1] = pos.Castling[1][1];

    for(int i = 0; i < 6; i++)
        for(int j = 0; j < 2; j++)
            PieceBitBoards[i][j] = pos.PieceBitBoards[i][j];
}


Position::~Position()
{
    //dtor
}
