#include "evaluate.h"

#include <bits/stdc++.h>


evaluation evaluatePosition(const Position& position)
{
    return pieceLocations(position, Color::White) + pieceLocations(position, Color::Black);
}

/*
double evaluateSide(const Position& position, Color color)
{
    return material(position, color) + pieceLocations(position, color);
}


double material(const Position& position, Color color)
{
    double res = countBits(position.PieceBitBoards[Piece::Pawn][color]) + 3 * countBits(position.PieceBitBoards[Piece::Knight][color])
        + 3.25 * countBits(position.PieceBitBoards[Piece::Bishop][color]) + 5 * countBits(position.PieceBitBoards[Piece::Rook][color])
        + 9 * countBits(position.PieceBitBoards[Piece::Queen][color]);

    return res;
}
*/

double pieceLocations(const Position& position, Color color)
{
    double res = 0.0;

    for (int i = 0; i < 6; i++)
        FORBIT(pos, position.PieceBitBoards[i][color])
            res += value((Piece)i, color, pos);

    return res;
}

double value(Piece piece, Color color, Square position)
{
    if (color == Color::White)
        return pieceSquaresTable[piece][(7 - (position / 8)) * 8 + (position % 8)];
    else
        return -pieceSquaresTable[piece][position];
}
