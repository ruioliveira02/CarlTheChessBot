#include "movegeneration.h"

std::vector<Move> generateAllMoves(Position position)
{
    Color color = position.ToMove;

    std::vector<Move> kingMoves = generateAllPieceMoves(position, Piece::King, color);
    std::vector<Move> queenMoves = generateAllPieceMoves(position, Piece::Queen, color);
    std::vector<Move> rookMoves = generateAllPieceMoves(position, Piece::Rook, color);
    std::vector<Move> bishopMoves = generateAllPieceMoves(position, Piece::Bishop, color);
    std::vector<Move> knightMoves = generateAllPieceMoves(position, Piece::Knight, color);
    std::vector<Move> pawnMoves = generateAllPieceMoves(position, Piece::Pawn, color);

    pawnMoves.insert(pawnMoves.end(), knightMoves.begin(), knightMoves.end());
    pawnMoves.insert(pawnMoves.end(), bishopMoves.begin(), bishopMoves.end());
    pawnMoves.insert(pawnMoves.end(), rookMoves.begin(), rookMoves.end());
    pawnMoves.insert(pawnMoves.end(), queenMoves.begin(), queenMoves.end());
    pawnMoves.insert(pawnMoves.end(), kingMoves.begin(), kingMoves.end());

    return pawnMoves;
}

std::vector<Move> generateAllPieceMoves(Position position, Piece piece, Color color)
{
    std::vector<Move> moves = std::vector<Move>();
    for(int i = 0; i < position.PieceLocations[piece][color].size(); i++)
    {
        Square square = position.PieceLocations[piece][color][i];
        std::vector<Move> temp;
        switch(piece)
        {
            case Piece::Pawn:
                temp = generatePawnMoves(position, square);
                break;
            case Piece::Knight:
                temp = generateKnightMoves(position, square);
                break;
            case Piece::Bishop:
                temp = generateBishopMoves(position, square);
                break;
            case Piece::Rook:
                temp = generateRookMoves(position, square);
                break;
            case Piece::Queen:
                temp = generateQueenMoves(position, square);
                break;
            case Piece::King:
                temp = generateKingMoves(position, square);
                break;
        }

        moves.insert(moves.end(), temp.begin(), temp.end());
    }

    return moves;
}
