#include "evaluate.h"


evaluation evaluatePosition(Position position)
{
    return evaluation(evaluateSide(position, Color::White) - evaluateSide(position, Color::Black));
}


double evaluateSide(Position position, Color color)
{
    return material(position, color) + pieceLocations(position, color);
}


double material(Position position, Color color)
{
    double res = position.PieceLocations[Piece::Pawn][color].size() + 3 * position.PieceLocations[Piece::Knight][color].size() + 3.25 * position.PieceLocations[Piece::Bishop][color].size()
                + 5 * position.PieceLocations[Piece::Rook][color].size() + 9 * position.PieceLocations[Piece::Queen][color].size();

    return res;
}


double pieceLocations(Position position, Color color)
{
    double res = 0.0;

    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < position.PieceLocations[i][color].size(); j++)
        {
            if(color == Color::White)
                res += pieceSquaresTable[i][position.PieceLocations[i][color][j]];
            else
                res += pieceSquaresTable[i][63 - position.PieceLocations[i][color][j]];
        }
    }

    return (res / 100.0);
}
