#include "game.h"
#include <algorithm>




game game::makeMove(Move move)
{
    game g(*this);


    if(g.position.ToMove == Color::White)
    {
        g.position.WhiteOccupancy = g.position.WhiteOccupancy & ~(1ULL << move.origin);
        g.position.WhiteOccupancy |= (1ULL << move.destiny);
    }
    else
    {
        g.position.BlackOccupancy = g.position.BlackOccupancy & ~(1ULL << move.origin);
        g.position.BlackOccupancy |= (1ULL << move.destiny);
    }

    for(int i = 0; i < 6; i++)
    {
        if(std::find(g.position.PieceLocations[i][oppositeColor(g.position.ToMove)].begin(), g.position.PieceLocations[i][oppositeColor(g.position.ToMove)].end(),move.destiny)
            != g.position.PieceLocations[i][oppositeColor(g.position.ToMove)].end())
        {
            g.position.PieceLocations[i][oppositeColor(g.position.ToMove)].erase(std::remove(g.position.PieceLocations[i][oppositeColor(g.position.ToMove)].begin(),
                g.position.PieceLocations[i][oppositeColor(g.position.ToMove)].end(), move.destiny),g.position.PieceLocations[i][oppositeColor(g.position.ToMove)].end());

            if(g.position.ToMove == Color::White)
            {
                g.position.BlackOccupancy = g.position.BlackOccupancy & ~(1ULL << move.destiny);
                g.position.PieceBitBoards[i][Color::Black] = g.position.PieceBitBoards[i][Color::Black] & ~(1ULL << move.destiny);
            }
            else
            {
                g.position.WhiteOccupancy = g.position.WhiteOccupancy & ~(1ULL << move.destiny);
                g.position.PieceBitBoards[i][Color::White] = g.position.PieceBitBoards[i][Color::White] & ~(1ULL << move.destiny);
            }
        }
    }

    if(move.type == MoveType::Promotion)
    {
        g.position.PieceBitBoards[Piece::Pawn][g.position.ToMove] = g.position.PieceBitBoards[Piece::Pawn][g.position.ToMove] & ~(1ULL << move.origin);
        g.position.PieceLocations[Piece::Pawn][g.position.ToMove].erase(std::remove(g.position.PieceLocations[Piece::Pawn][g.position.ToMove].begin(),
            g.position.PieceLocations[Piece::Pawn][g.position.ToMove].end(), move.origin), g.position.PieceLocations[Piece::Pawn][g.position.ToMove].end());

        g.position.PieceBitBoards[move.piece][g.position.ToMove] |= (1ULL << move.destiny);
        g.position.PieceLocations[move.piece][g.position.ToMove].push_back(move.destiny);
    }
    else if(move.type == MoveType::EnPassant)
    {
        g.position.PieceBitBoards[Piece::Pawn][g.position.ToMove] = g.position.PieceBitBoards[Piece::Pawn][g.position.ToMove] & ~(1ULL << move.origin);
        g.position.PieceBitBoards[Piece::Pawn][g.position.ToMove] |= (1ULL << move.destiny);
        std::replace(g.position.PieceLocations[Piece::Pawn][g.position.ToMove].begin(), g.position.PieceLocations[Piece::Pawn][g.position.ToMove].end(), move.origin, move.destiny);

        int enpassant = (g.position.ToMove == Color::White) ? (move.destiny - 8) : (move.destiny + 8);

        if(g.position.ToMove == Color::White)
        {
            g.position.BlackOccupancy = g.position.BlackOccupancy & ~(1ULL << enpassant);
        }
        else
        {
            g.position.WhiteOccupancy = g.position.WhiteOccupancy & ~(1ULL << enpassant);
        }

        g.position.PieceBitBoards[Piece::Pawn][oppositeColor(g.position.ToMove)] = g.position.PieceBitBoards[Piece::Pawn][oppositeColor(g.position.ToMove)] & (1ULL << enpassant);
        g.position.PieceLocations[Piece::Pawn][oppositeColor(g.position.ToMove)].erase(std::remove(g.position.PieceLocations[Piece::Pawn][oppositeColor(g.position.ToMove)].begin(),
            g.position.PieceLocations[Piece::Pawn][oppositeColor(g.position.ToMove)].end(), enpassant), g.position.PieceLocations[Piece::Pawn][oppositeColor(g.position.ToMove)].end());
    }
    else if(move.type == MoveType::Castling)
    {
        g.position.Castling[0][g.position.ToMove] = g.position.Castling[1][g.position.ToMove] = false;

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

        if(g.position.ToMove == Color::White)
        {
            g.position.WhiteOccupancy |= (1ULL << rookDestiny);
            g.position.WhiteOccupancy = g.position.WhiteOccupancy & ~(1ULL << rookOrigin);
        }
        else
        {
            g.position.BlackOccupancy |= (1ULL << rookDestiny);
            g.position.BlackOccupancy = g.position.BlackOccupancy & ~(1ULL << rookOrigin);
        }

        std::replace(g.position.PieceLocations[Piece::Rook][g.position.ToMove].begin(), g.position.PieceLocations[Piece::Rook][g.position.ToMove].end(), rookOrigin, rookDestiny);
        g.position.PieceBitBoards[Piece::Rook][g.position.ToMove] |= (1ULL << rookDestiny);
        g.position.PieceBitBoards[Piece::Rook][g.position.ToMove] = g.position.PieceBitBoards[Piece::Rook][g.position.ToMove] & ~(1ULL << rookOrigin);
    }
    else
    {
        std::replace(g.position.PieceLocations[move.piece][g.position.ToMove].begin(), g.position.PieceLocations[move.piece][g.position.ToMove].end(), move.origin, move.destiny);
        g.position.PieceBitBoards[move.piece][g.position.ToMove] |= (1ULL << move.destiny);
        g.position.PieceBitBoards[move.piece][g.position.ToMove] = g.position.PieceBitBoards[move.piece][g.position.ToMove] & ~(1ULL << move.origin);

        if(move.piece == Piece::Rook)
        {
            if(move.origin == 0)
            {
                g.position.Castling[1][0] = false;
            }
            else if(move.origin == 7)
            {
                g.position.Castling[0][0] = false;
            }
            else if(move.origin == 56)
            {
                g.position.Castling[1][1] = false;
            }
            else if(move.origin == 63)
            {
                g.position.Castling[0][1] = false;
            }
        }
        else if(move.piece == Piece::King)
        {
            g.position.Castling[0][g.position.ToMove] = g.position.Castling[1][g.position.ToMove] = false;
        }
    }

    g.position.ToMove = oppositeColor(g.position.ToMove);
    return g;
}


bool game::exists(Piece piece)
{
    return position.PieceBitBoards[piece][Color::White] != 0ULL || position.PieceBitBoards[piece][Color::Black] != 0ULL;
}

bool game::isDraw()
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

evaluation game::evaluate()
{
    return evaluation(42.0);
}
