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

Position makeMove(Move move)
{
    Position p(*this);
    
    switch (move.type)
    {
        //TODO
    }

    p.ToMove = (p.ToMove + 1) % 2;
    return p;

}

bool exists(Piece piece)
{
    return PieceBitBoards[piece][Color::White] != 0 || PieceBitBoards[piece][Color::Black] != 0;
}

evaluation Position::evaluate()
{
    //material insuficiente
    if (!exists(Piece::Queen) && !exists(Piece::Rook) && !exists(Piece::Pawn))
    {
        //TODO: verificar material

    }

    //TODO: detetar dead position (?)

    //mate
    if (!hasLegalMoves(*this))
    {
        if (inCheck(*this, ToMove))
            return evaluation(0, (ToMove + 1) % 2); //checkmate

        return evaluation();                        //stalemate
    }

    //TODO: 50 moves rule (eu vi na wiki q há uma dos 75 moves tmb, qual é a diferença?)

    //TODO: empate por repeticao...

    //TODO: avaliação por score
    return evaluation(42.0);
}
