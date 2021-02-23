#include "position.h"

#include <vector>

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
            for(int k = 0; k < PieceLocations[i][k].size(); k++)
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
}


Position::~Position()
{
    //dtor
}

