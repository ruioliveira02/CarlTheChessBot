#include "movegeneration.h"
#include "magicmoves.h"

#include <bits/stdc++.h>

BitBoard knightMoves[64];
BitBoard kingMoves[64];

std::pair<Move*, int> generateAllMoves(const Position& position)
{
    Color color = position.ToMove;
    int moveCount = 0;

    BitBoard* kingMoves = generateAllPieceMoves(position, Piece::King, color);
    BitBoard* queenMoves = generateAllPieceMoves(position, Piece::Queen, color);
    BitBoard* rookMoves = generateAllPieceMoves(position, Piece::Rook, color);
    BitBoard* bishopMoves = generateAllPieceMoves(position, Piece::Bishop, color);
    BitBoard* knightMoves = generateAllPieceMoves(position, Piece::Knight, color);
    BitBoard* pawnMoves = generateAllPieceMoves(position, Piece::Pawn, color);
    BitBoard enPassant = generateEnPassant(position);
    Move* castling = generateCastling(position, position.PieceLocations[Piece::King][color][0]);

    moveCount += __builtin_popcountll(*kingMoves);

    for (int i = 0; i < 10 && queenMoves[i] != 0; i++)
        moveCount += __builtin_popcountll(queenMoves[i]);

    for (int i = 0; i < 10 && rookMoves[i] != 0; i++)
        moveCount += __builtin_popcountll(rookMoves[i]);

    for (int i = 0; i < 10 && bishopMoves[i] != 0; i++)
        moveCount += __builtin_popcountll(bishopMoves[i]);

    for (int i = 0; i < 10 && knightMoves[i] != 0; i++)
        moveCount += __builtin_popcountll(knightMoves[i]);

    for (int i = 0; i < 10 && pawnMoves[i] != 0; i++)
    {
        moveCount += __builtin_popcountll(pawnMoves[i] & 72057594037927680ULL);
        moveCount += 4 * __builtin_popcountll(pawnMoves[i] & 18374686479671623935ULL);
    }

    moveCount += __builtin_popcountll(enPassant);

    //castling
    for (int i = 0; i < 2; i++)
        if (castling[i].type == MoveType::Castling)
            moveCount++;

    Move* moves = new Move[moveCount];
    Move* it = moves;
    int j;

    convertBitBoardToMoves(*kingMoves, position.PieceLocations[Piece::King][color][0], Piece::King, it);

    for (int i = 0; i < 10 && queenMoves[i] != 0; i++)
        convertBitBoardToMoves(queenMoves[i], position.PieceLocations[Piece::Queen][color][i], Piece::Queen, it);

    for (int i = 0; i < 10 && rookMoves[i] != 0; i++)
        convertBitBoardToMoves(rookMoves[i], position.PieceLocations[Piece::Rook][color][i], Piece::Rook, it);

    for (int i = 0; i < 10 && bishopMoves[i] != 0; i++)
        convertBitBoardToMoves(bishopMoves[i], position.PieceLocations[Piece::Bishop][color][i], Piece::Bishop, it);

    for (int i = 0; i < 10 && knightMoves[i] != 0; i++)
        convertBitBoardToMoves(knightMoves[i], position.PieceLocations[Piece::Knight][color][i], Piece::Knight, it);

    for (int i = 0; i < 10 && pawnMoves[i] != 0; i++)
    {
        convertBitBoardToMoves(pawnMoves[i] & 72057594037927680ULL, position.PieceLocations[Piece::Pawn][color][i], Piece::Pawn, it);
        BitBoard promotions = pawnMoves[i] & 18374686479671623935ULL;

        while ((j = ffsll(promotions) - 1) != -1)
        {
            *(it++) = Move(MoveType::Promotion, position.PieceLocations[Piece::Pawn][color][i], j, Piece::Queen);
            *(it++) = Move(MoveType::Promotion, position.PieceLocations[Piece::Pawn][color][i], j, Piece::Rook);
            *(it++) = Move(MoveType::Promotion, position.PieceLocations[Piece::Pawn][color][i], j, Piece::Knight);
            *(it++) = Move(MoveType::Promotion, position.PieceLocations[Piece::Pawn][color][i], j, Piece::Bishop);
            enPassant &= ~(1ULL << j);
        }
    }

    //en passant
    while ((j = ffsll(enPassant) - 1) != -1)
    {
        *(it++) = Move(MoveType::EnPassant, j, position.EnPassant, Piece::Pawn);
        enPassant &= ~(1ULL << j);
    }

    for (int i = 0; i < 2; i++)
        if (castling[i].type == MoveType::Castling)
            *(it++) = castling[i];

    delete[] kingMoves;
    delete[] queenMoves;
    delete[] rookMoves;
    delete[] bishopMoves;
    delete[] knightMoves;
    delete[] pawnMoves;
    delete[] castling;

    return make_pair(moves, moveCount);
}

BitBoard* generateAllPieceMoves(const Position& position, Piece piece, Color color)
{
    BitBoard* moves = new BitBoard[10];

    for(int i = 0; i < position.PieceLocations[piece][color].size(); i++)
    {
        Square square = position.PieceLocations[piece][color][i];

        switch(piece)
        {
            case Piece::Pawn:
                moves[i] = generatePawnMoves(position, square);
                break;
            case Piece::Knight:
                moves[i] = generateKnightMoves(position, square);
                break;
            case Piece::Bishop:
                moves[i] = generateBishopMoves(position, square);
                break;
            case Piece::Rook:
                moves[i] = generateRookMoves(position, square);
                break;
            case Piece::Queen:
                moves[i] = generateQueenMoves(position, square);
                break;
            case Piece::King:
                moves[i] = generateKingMoves(position, square);
                break;
        }
    }

    return moves;
}

void convertBitBoardToMoves(BitBoard bitboard, Square square, Piece piece, Move*& it)
{
    int i;

    //find rightmost set bit
    while ((i = ffsll(bitboard) - 1) != -1)
    {
        *(it++) = Move(MoveType::Normal, square, i, piece);
        bitboard &= ~(1ULL << i);
    }
}


BitBoard generateQueenMoves(const Position& position, Square square)
{
    return generateRookMoves(position, square) | generateBishopMoves(position, square);
}

BitBoard generateRookMoves(const Position& position, Square square)
{
    BitBoard occupancy = position.WhiteOccupancy | position.BlackOccupancy;

    BitBoard ownPieces = (position.ToMove == Color::White) ? position.WhiteOccupancy : position.BlackOccupancy;

    return Rmagic(square,occupancy) & ~ownPieces;
}

BitBoard generateBishopMoves(const Position& position, Square square)
{
    BitBoard occupancy = position.WhiteOccupancy | position.BlackOccupancy;

    BitBoard ownPieces = (position.ToMove == Color::White) ? position.WhiteOccupancy : position.BlackOccupancy;

    return Bmagic(square,occupancy) & ~ownPieces;
}


//excluindo enpassant
BitBoard generatePawnMoves(const Position& position, Square square)
{
    BitBoard occupancy = position.WhiteOccupancy | position.BlackOccupancy;
    BitBoard oppoenentOccupancy = position.ToMove == Color::White ? position.BlackOccupancy : position.WhiteOccupancy;

    if (position.ToMove == Color::White)
    {
        BitBoard push = (1ULL << (square + 8)) & ~occupancy;
        BitBoard doublePush = 4278190080ULL & (push << 8) & ~occupancy;
        BitBoard captures = (5ULL << (square + 7)) & (255ULL << ((square / 8 + 1) * 8)) & oppoenentOccupancy;
        return push | doublePush | captures;
    }
    else
    {
        BitBoard push = (1ULL << (square - 8)) & ~occupancy;
        BitBoard doublePush = 1095216660480ULL & (push >> 8) & ~occupancy;
        BitBoard captures = (5ULL << (square - 9)) & (255ULL << ((square / 8 - 1) * 8)) & oppoenentOccupancy;
        return push | doublePush | captures;
    }
}

BitBoard generateKnightMoves(const Position& position, Square square)
{
    BitBoard ownPieces = (position.ToMove == Color::White) ? position.WhiteOccupancy : position.BlackOccupancy;
    return knightMoves[square] & ~ownPieces;
}

BitBoard generateKingMoves(const Position& position, Square square)
{
    BitBoard ownPieces = (position.ToMove == Color::White) ? position.WhiteOccupancy : position.BlackOccupancy;
    return kingMoves[square] & ~ownPieces;
}

//retorna uma array de tamanho 2
Move* generateCastling(const Position& position, Square square)
{
    Move *answer = new Move[2];
    answer[0].type = MoveType::Normal;
    answer[1].type = MoveType::Normal;
    Move *it = answer;

    if(inCheck(position, position.ToMove, square))
        return answer;

    bool kingside = (position.ToMove == Color::White) ? position.Castling[0][0] : position.Castling[0][1];
    bool queenside = (position.ToMove == Color::White) ? position.Castling[1][0] : position.Castling[1][1];

    BitBoard occupancy = position.WhiteOccupancy | position.BlackOccupancy;

    if(kingside && !(occupancy & (1ULL << (square + 1))) && !(occupancy & (1ULL << (square + 2)))
        && !inCheck(position, position.ToMove, square + 1) && !inCheck(position, position.ToMove, square + 2))
    {
        *(it++) = Move(MoveType::Castling, square, square + 2, Piece::King);
    }

    if(queenside && !(occupancy & (1ULL << (square - 1))) && !(occupancy & (1ULL << (square - 2))) && !(occupancy & (1ULL << (square - 3)))
        && !inCheck(position, position.ToMove, square - 1) && !inCheck(position, position.ToMove, square - 2))
    {
        *(it++) = Move(MoveType::Castling, square, square - 2, Piece::King);
    }

    return answer;
}


//returns the positions of the pawns that can capture en passant, not their destinations
BitBoard generateEnPassant(const Position& position)
{
    if (position.EnPassant == -1)
        return 0;

    if (position.ToMove == Color::White)
        return position.PieceBitBoards[Piece::Pawn][position.ToMove] & (5ULL << (position.EnPassant - 9)) & 4278190080ULL;
    else
        return position.PieceBitBoards[Piece::Pawn][position.ToMove] & (5ULL << (position.EnPassant + 7)) & 1095216660480ULL;
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
            knightMoves[i] |= (1ULL << (i + 17));
        if(rank + 2 <= 7 && file - 1 >= 0)
            knightMoves[i] |= (1ULL << (i + 15));
        if(rank - 2 >= 0 && file + 1 <= 7)
            knightMoves[i] |= (1ULL << (i - 15));
        if(rank - 2 >= 0 && file - 1 >= 0)
            knightMoves[i] |= (1ULL << (i - 17));
        if(rank + 1 <= 7 && file + 2 <= 7)
            knightMoves[i] |= (1ULL << (i + 10));
        if(rank + 1 <= 7 && file - 2 >= 0)
            knightMoves[i] |= (1ULL << (i + 6));
        if(rank - 1 >= 0 && file + 2 <= 7)
            knightMoves[i] |= (1ULL << (i - 6));
        if(rank - 1 >= 0 && file - 2 >= 0)
            knightMoves[i] |= (1ULL << (i - 10));
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
            kingMoves[i] |= (1ULL << (i + 9));
        if(rank + 1 <= 7)
            kingMoves[i] |= (1ULL << (i + 8));
        if(rank + 1 <= 7 && file - 1 >= 0)
            kingMoves[i] |= (1ULL << (i + 7));
        if(file - 1 >= 0)
            kingMoves[i] |= (1ULL << (i - 1));
        if(file + 1 <= 7)
            kingMoves[i] |= (1ULL << (i + 1));
        if(rank - 1 >= 0 && file - 1 >= 0)
            kingMoves[i] |= (1ULL << (i - 9));
        if(rank - 1 >= 0 && file + 1 <= 7)
            kingMoves[i] |= (1ULL << (i - 7));
        if(rank - 1 >= 0)
            kingMoves[i] |= (1ULL << (i - 8));
    }
}


bool inCheck(const Position& position, Color color)
{
    return inCheck(position, color, position.PieceLocations[Piece::King][color][0]);
}

bool inCheck(const Position& position, Color color, Square square)
{
    BitBoard ownPieces = (color == Color::White) ? position.WhiteOccupancy : position.BlackOccupancy;
    BitBoard occupancy = position.WhiteOccupancy | position.BlackOccupancy;

    int opponentIndex = oppositeColor(color);

    if((knightMoves[square] & ~ownPieces) & position.PieceBitBoards[Piece::Knight][opponentIndex])
        return true;

     if((Rmagic(square, occupancy) & ~ownPieces) & (position.PieceBitBoards[Piece::Rook][opponentIndex] | position.PieceBitBoards[Piece::Queen][opponentIndex]))
        return true;


    if((Bmagic(square, occupancy) & ~ownPieces) & (position.PieceBitBoards[Piece::Bishop][opponentIndex] | position.PieceBitBoards[Piece::Queen][opponentIndex]))
        return true;


    BitBoard pawnChecks = (color == Color::White) ? ((1ULL << (square + 7)) | (1ULL << (square + 9))) : ((1ULL << (square - 7)) | (1ULL << (square - 9)));

    if(pawnChecks & position.PieceBitBoards[Piece::Pawn][oppositeColor(color)])
        return true;

    if(kingMoves[square] & position.PieceBitBoards[Piece::King][oppositeColor(color)])
        return true;

    return false;
}
