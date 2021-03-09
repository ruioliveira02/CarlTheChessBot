#include "game.h"
#include "movegeneration.h"
#include "magicmoves.h"
#include "evaluate.h"
#include <algorithm>



void game::initialize()
{
    initializeAllBitBoards();
    initmagicmoves();
}



game game::makeMove(Move move) const
{
    game g(*this);

    g.updateBitBoards(move);
    g.assessCapture(move);


    if(move.type == MoveType::Promotion)
    {
        g.promote(move);
    }
    else if(move.type == MoveType::EnPassant)
    {
        g.captureEnpassant(move);
    }
    else if(move.type == MoveType::Castling)
    {
        g.castle(move);
    }
    else
    {
        g.updatePieces(move);
        g.updateCastlingRights(move);
    }

    g.position.ToMove = oppositeColor(g.position.ToMove);
    return g;
}


bool game::exists(Piece piece) const
{
    return position.PieceBitBoards[piece][Color::White] != 0ULL || position.PieceBitBoards[piece][Color::Black] != 0ULL;
}

bool game::isDraw() const
{
	//material insuficiente
    if (!exists(Piece::Queen) && !exists(Piece::Rook) && !exists(Piece::Pawn))
    {
        //TODO: verificar material

    }

    //TODO: detetar dead position (?)

    //TODO: 50 moves rule

    //TODO: empate por repeticao...

    return false;
}

void game::updateBitBoards(Move move)
{
    if(position.ToMove == Color::White)
    {
        position.WhiteOccupancy = position.WhiteOccupancy & ~(1ULL << move.origin);
        position.WhiteOccupancy |= (1ULL << move.destiny);
    }
    else
    {
        position.BlackOccupancy = position.BlackOccupancy & ~(1ULL << move.origin);
        position.BlackOccupancy |= (1ULL << move.destiny);
    }
}


void game::assessCapture(Move move)
{
    Color color = oppositeColor(position.ToMove);

    for(int i = 0; i < 6; i++)
    {
        //there is a piece of type i in the destination of the move
        if ((1ULL << move.destiny) & position.PieceBitBoards[i][color])
        {
            BitBoard eraseAux = ~(1ULL << move.destiny);
            position.PieceBitBoards[i][color] &= eraseAux;

            if(position.ToMove == Color::White)
            {
                position.BlackOccupancy &= eraseAux;
                position.PieceBitBoards[i][Color::Black] &= eraseAux;
            }
            else
            {
                position.WhiteOccupancy &= eraseAux;
                position.PieceBitBoards[i][Color::White] &= eraseAux;
            }

            break;
        }
    }
}


void game::promote(Move move)
{
    position.PieceBitBoards[Piece::Pawn][position.ToMove] = position.PieceBitBoards[Piece::Pawn][position.ToMove] & ~(1ULL << move.origin);
    position.PieceBitBoards[move.piece][position.ToMove] |= (1ULL << move.destiny);
}



void game::captureEnpassant(Move move)
{
    position.PieceBitBoards[Piece::Pawn][position.ToMove] = position.PieceBitBoards[Piece::Pawn][position.ToMove] & ~(1ULL << move.origin);
    position.PieceBitBoards[Piece::Pawn][position.ToMove] |= (1ULL << move.destiny);

    int enpassant = (position.ToMove == Color::White) ? (move.destiny - 8) : (move.destiny + 8);

    if(position.ToMove == Color::White)
    {
        position.BlackOccupancy = position.BlackOccupancy & ~(1ULL << enpassant);
    }
    else
    {
        position.WhiteOccupancy = position.WhiteOccupancy & ~(1ULL << enpassant);
    }

    position.PieceBitBoards[Piece::Pawn][oppositeColor(position.ToMove)] = position.PieceBitBoards[Piece::Pawn][oppositeColor(position.ToMove)] & (1ULL << enpassant);
}


void game::castle(Move move)
{
    position.Castling[0][position.ToMove] = position.Castling[1][position.ToMove] = false;

    int rookOrigin = 0, rookDestiny = 0;

    if(move.destiny == 2)
    {
        rookOrigin = 0;
        rookDestiny = 3;
    }
    else if(move.destiny == 6)
    {
        rookOrigin = 7;
        rookDestiny = 5;
    }
    else if(move.destiny == 62)
    {
        rookOrigin = 63;
        rookDestiny = 61;
    }
    else
    {
        rookOrigin = 56;
        rookDestiny = 59;
    }

    if(position.ToMove == Color::White)
    {
        position.WhiteOccupancy |= (1ULL << rookDestiny);
        position.WhiteOccupancy = position.WhiteOccupancy & ~(1ULL << rookOrigin);
    }
    else
    {
        position.BlackOccupancy |= (1ULL << rookDestiny);
        position.BlackOccupancy = position.BlackOccupancy & ~(1ULL << rookOrigin);
    }

    position.PieceBitBoards[Piece::Rook][position.ToMove] |= (1ULL << rookDestiny);
    position.PieceBitBoards[Piece::Rook][position.ToMove] &= ~(1ULL << rookOrigin);

    updatePieces(move);
}


void game::updatePieces(Move move)
{
    Color color = oppositeColor(position.ToMove);
    BitBoard dest_bitboard = (1ULL << move.destiny);

    //inventar alguma coisa com XOR para ser mais rápido?
    position.PieceBitBoards[move.piece][position.ToMove] &= ~(1ULL << move.origin);
    position.PieceBitBoards[move.piece][position.ToMove] |= dest_bitboard;

    dest_bitboard = ~dest_bitboard;
}


void game::updateCastlingRights(Move move)
{
    if(move.piece == Piece::Rook)
    {
        if(move.origin == 0)
        {
            position.Castling[1][0] = false;
        }
        else if(move.origin == 7)
        {
            position.Castling[0][0] = false;
        }
        else if(move.origin == 56)
        {
            position.Castling[1][1] = false;
        }
        else if(move.origin == 63)
        {
            position.Castling[0][1] = false;
        }
    }
    else if(move.piece == Piece::King)
    {
        position.Castling[0][position.ToMove] = position.Castling[1][position.ToMove] = false;
    }
}



evaluation game::evaluate() const
{
   return evaluatePosition(position);
}
