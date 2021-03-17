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
        return destiny % 8 == 6 ? "0-0" : "0-0-0";

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

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            Castling[i][j] = false;

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

std::string Position::toFEN() const
{
    std::string ans = "";

    for (int y = 7; y >= 0; y--)
    {
        int skips = 0;

        for (int x = 0; x < 8; x++)
        {
            for (int p = 0; p < 6; p++)
            {
                for (int c = 0; c < 2; c++)
                {
                    if (PieceBitBoards[p][c] & (1ULL << (8 * y + x)))
                    {
                        if (skips == 0)
                            ans += pieceChar((Piece)p, (Color)c);
                        else
                            ans += std::to_string(skips) + pieceChar((Piece)p, (Color)c);

                        skips = 0;
                        goto next;
                    }
                }
            }

            skips++;
            next:
            continue;
        }

        if (y != 0)
            ans += skips == 0 ? "/" : std::to_string(skips) + "/";
        else if (skips != 0)
            ans += std::to_string(skips);
    }

    ans += (ToMove == Color::White ? " w " : " b ");
    bool any = false;

    if (Castling[0][Color::White])
    {
        ans += 'K';
        any = true;
    }

    if (Castling[1][Color::White])
    {
        ans += 'Q';
        any = true;
    }

    if (Castling[0][Color::Black])
    {
        ans += 'k';
        any = true;
    }

    if (Castling[1][Color::Black])
    {
        ans += 'q';
        any = true;
    }

    if (!any)
        ans += "- ";
    else
        ans += ' ';

    if (EnPassant == -1)
        ans += "- ";
    else
        ans += "" + ('a' + (EnPassant % 8)) + ('1' + (EnPassant / 8)) + ' ';

    ans += std::to_string(HalfMoves) + ' ' + std::to_string(TotalMoves);
    return ans;
}


Move Position::moveFromString(std::string str)
{
    Square origin = squareFromString(str.substr(0, 2));
    Square destiny = squareFromString(str.substr(2, 2));

    //Promotion
    if(str.size() == 5)
    {
        return Move(MoveType::Promotion, origin, destiny, pieceFromChar(str[4]));
    }

    Piece moved = pieceOnSquare(origin);

    if(moved == Piece::King && abs(origin - destiny) == 2)
    {
        return Move(MoveType::Castling, origin, destiny, moved);
    }
    if(moved == Piece::Pawn)
    {
        if(ToMove == Color::White && origin / 8 == 4)
        {
            if(destiny == origin + 7 && (PieceBitBoards[Piece::Pawn][Color::Black] & (1ULL << (origin - 1))) &&
                !(BlackOccupancy & (1ULL << (origin + 7))))
            {
                return Move(MoveType::EnPassant, origin, destiny, Piece::Pawn);
            }
            if(destiny == origin + 9 && (PieceBitBoards[Piece::Pawn][Color::Black] & (1ULL << (origin + 1))) &&
                !(BlackOccupancy & (1ULL << (origin + 9))))
            {
                return Move(MoveType::EnPassant, origin, destiny, Piece::Pawn);
            }
        }
        if(ToMove == Color::White && origin / 8 == 3)
        {
            if(destiny == origin - 7 && (PieceBitBoards[Piece::Pawn][Color::White] & (1ULL << (origin + 1))) &&
                !(WhiteOccupancy & (1ULL << (origin - 7))))
            {
                return Move(MoveType::EnPassant, origin, destiny, Piece::Pawn);
            }
            if(destiny == origin - 9 && (PieceBitBoards[Piece::Pawn][Color::White] & (1ULL << (origin - 1))) &&
                !(WhiteOccupancy & (1ULL << (origin - 9))))
            {
                return Move(MoveType::EnPassant, origin, destiny, Piece::Pawn);
            }
        }
    }

    return Move(MoveType::Normal, origin, destiny, moved);
}


Square Position::squareFromString(std::string str)
{
    return (str[0] - 'a') * 8 + (str[1] - '1');
}



Piece Position::pieceFromChar(char ch)
{
    switch(ch)
    {
        case 'q':
            return Piece::Queen;
        case 'k':
            return Piece::King;
        case 'r':
            return Piece::Rook;
        case 'b':
            return Piece::Bishop;
        case 'n':
            return Piece::Knight;
        default:
            return Piece::Pawn;
    }
}




std::string Move::toString()
{
    std::string suffix = "";
    if(type==MoveType::Promotion)
        suffix = std::string(1, pieceChar(piece, Color::Black));

    return squareToString(origin) + squareToString(destiny) + suffix;
}


std::string Move::squareToString(Square s)
{
    int i = s / 8, j = s % 8;

    return std::string(1,(char)(j + 'a')) + std::string(1,(char)(i + '1'));
}


Piece Position::pieceOnSquare(Square s)
{
    for(int i = 0; i < 6; i++)
        for(int j = 0; j < 2; j++)
            if(PieceBitBoards[i][j] & (1ULL << s))
                return (Piece)i;

    return Piece::King;
}
