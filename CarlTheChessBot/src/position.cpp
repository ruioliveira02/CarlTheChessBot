#include "position.h"

#include <vector>
#include <iostream>

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


Position::Position(std::vector<Square> pieces [6][2],BitBoard white, BitBoard black ,Color color,bool castling[2][2], short enpassant,short halfMoves,short totalMoves)
{
    EnPassant = enpassant;
    ToMove = color;
    HalfMoves = halfMoves;
    TotalMoves = totalMoves;

    WhiteOccupancy = white;
    BlackOccupancy = black;

    for(int i = 0; i < 2; i++)
        for(int j = 0; j < 2; j++)
            Castling[i][j] = castling[i][j];

    for(int i = 0; i < 6; i++)
        for(int j = 0; j < 2; j++)
            PieceLocations[i][j] = pieces[i][j];

    for(int i = 0; i < 6; i++)
        for(int j = 0; j < 2; j++)
            PieceBitBoards[i][j] = 0ULL;



    for(int i = 0; i < 6; i++)
        for(int j = 0; j < 2; j++)
            for(int k = 0; k < PieceLocations[i][j].size(); k++)
                PieceBitBoards[i][j] |= (1ULL << (PieceLocations[i][j][k]));
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

    for(int i = 0; i < 6; i++)
        for(int j = 0; j < 2; j++)
            PieceLocations[i][j] = pos.PieceLocations[i][j];

    for(int i = 0; i < 6; i++)
        for(int j = 0; j < 2; j++)
            PieceBitBoards[i][j] = pos.PieceBitBoards[i][j];
}


Position::~Position()
{
    //dtor
}
