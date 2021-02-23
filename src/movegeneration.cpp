#include "movegeneration.h"
#include "MagicMoves.hpp"

BitBoard knightMoves[64];
BitBoard kingMoves[64];

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

std::vector<Move> generateQueenMoves(Position position, Square square)
{
    std::vector<Move> rookMoves = generateRookMoves(position, square);
    std::vector<Move> bishopMoves = generateBishopMoves(position, square);

    rookMoves.insert(rookMoves.end(), bishopMoves.begin(), bishopMoves.end());

    return rookMoves;
}

std::vector<Move> generateRookMoves(Position position, Square square)
{
    BitBoard occupancy = position.WhiteOccupancy | position.BlackOccupancy;

    BitBoard ownPieces = (position.ToMove == Color::White) ? position.WhiteOccupancy : position.BlackOccupancy;

    BitBoard result = Rmagic(square,occupancy) & ~ownPieces;

    return convertBitBoardToMoves(result, square, Piece::Rook);
}

std::vector<Move> generateBishopMoves(Position position, Square square)
{
    BitBoard occupancy = position.WhiteOccupancy | position.BlackOccupancy;

    BitBoard ownPieces = (position.ToMove == Color::White) ? position.WhiteOccupancy : position.BlackOccupancy;

    BitBoard result = Bmagic(square,occupancy) & !ownPieces;

    return convertBitBoardToMoves(result, square, Piece::Bishop);
}


std::vector<Move> generatePawnMoves(Position position, Square square)
{
    return std::vector<Move>();
}

std::vector<Move> generateKnightMoves(Position position, Square square)
{
    BitBoard ownPieces = (position.ToMove == Color::White) ? position.WhiteOccupancy : position.BlackOccupancy;
    BitBoard result = knightMoves[square] & ~ownPieces;

    return convertBitBoardToMoves(result, square, Piece::Knight);
}

std::vector<Move> generateKingMoves(Position position, Square square)
{
    return std::vector<Move>();
}

std::vector<Move> convertBitBoardToMoves(BitBoard bitboard, Square square, Piece piece)
{
    std::vector<Move> answer = std::vector<Move>();

    for(int i = 0; i < 64; i++)
    {
        //Check if i-th bit is set
        if(bitboard & (1ULL << i))
        {
            answer.push_back(Move(MoveType::Normal, square, i, -1, piece));
        }
    }

    return answer;
}


void initializeAllBitBoards()
{
    initializeKingBitBoard();
    initializeKnightBitBoard();
}

void initializeKnightBitBoard()
{
    for(int i = 0; i < 64; i++)
    {
        knightMoves[i] = 0ULL;

        int rank = i / 8;
        int file = i % 8;

        if(rank + 2 <= 7 && file + 1 <= 7)
            knightMoves[i] |= (1 << (i + 17));
        if(rank + 2 <= 7 && file - 1 >= 0)
            knightMoves[i] |= (1 << (i + 15));
        if(rank - 2 >= 0 && file + 1 <= 7)
            knightMoves[i] |= (1 << (i - 15));
        if(rank - 2 >= 0 && file - 1 >= 0)
            knightMoves[i] |= (1 << (i - 17));
        if(rank + 1 <= 7 && file + 2 <= 7)
            knightMoves[i] |= (1 << (i + 10));
        if(rank + 1 <= 7 && file - 2 >= 0)
            knightMoves[i] |= (1 << (i + 6));
        if(rank - 1 >= 0 && file + 2 <= 7)
            knightMoves[i] |= (1 << (i - 6));
        if(rank - 1 >= 0 && file - 2 >= 0)
            knightMoves[i] |= (1 << (i - 10));
    }
}


void initializeKingBitBoard()
{
    for(int i = 0; i < 64; i++)
    {
        kingMoves[i] = 0ULL;

        int rank = i / 8;
        int file = i % 8;

        if(rank + 1 <= 7 && file + 1 <= 7)
            kingMoves[i] |= (1 << (i + 9));
        if(rank + 1 <= 7)
            kingMoves[i] |= (1 << (i + 8));
        if(rank + 1 <= 7 && file - 1 >= 0)
            kingMoves[i] |= (1 << (i - 7));
        if(file - 1 >= 0)
            kingMoves[i] |= (1 << (i - 1));
        if(file + 1 <= 7)
            kingMoves[i] |= (1 << (i + 1));
        if(rank - 1 >= 0 && file - 1 >= 0)
            kingMoves[i] |= (1 << (i - 9));
        if(rank - 1 >= 0 && file + 1 <= 7)
            kingMoves[i] |= (1 << (i - 7));
        if(rank - 1 >= 0)
            kingMoves[i] |= (1 << (i - 8));
    }
}
