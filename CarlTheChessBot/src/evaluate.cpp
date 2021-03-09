#include "evaluate.h"

#include <bits/stdc++.h>

evaluation evaluatePosition(const Position& position)
{
    return evaluation(evaluateSide(position, Color::White) - evaluateSide(position, Color::Black));
}


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


double pieceLocations(const Position& position, Color color)
{
    double res = 0.0;

    for (int i = 0; i < 6; i++)
    {
        FORBIT(pos, position.PieceBitBoards[i][color])
        {
            if(color == Color::White)
                res += pieceSquaresTable[i][pos];
            else
                res += pieceSquaresTable[i][(7 - (pos / 8)) * 8 + (pos % 8)];
        }
    }

    return (res / 100.0);
}
