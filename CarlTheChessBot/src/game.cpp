#include "game.h"







game game::makeMove(Move move)
{
    Game g(*this);
    
    switch (move.type)
    {
        //TODO
    }

    g.position.ToMove = (g.position.ToMove + 1) % 2;
    return g;
}

bool exists(Piece piece)
{
    return PieceBitBoards[piece][Color::White] != 0 || PieceBitBoards[piece][Color::Black] != 0;
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