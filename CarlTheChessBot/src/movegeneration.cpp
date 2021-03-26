#include "movegeneration.h"
#include "magicmoves.h"

BitBoard knightBitBoard[64];
BitBoard kingBitBoard[64];

BitBoard kingMoves[64];     //tecnicamente desnecessátio, mas prontos
BitBoard queenMoves[64];
BitBoard rookMoves[64];
BitBoard bishopMoves[64];
BitBoard knightMoves[64];
BitBoard pawnMoves[64];

//assume-se que a array it tem espaço suficiente para guardar todos os movimentos
int generateAllMoves(const Position& position, Move* it)
{
    Color color = position.ToMove;
    int moveCount = 0;

    generateAllPieceMoves(position, Piece::King, color);
    generateAllPieceMoves(position, Piece::Queen, color);
    generateAllPieceMoves(position, Piece::Rook, color);
    generateAllPieceMoves(position, Piece::Bishop, color);
    generateAllPieceMoves(position, Piece::Knight, color);
    generateAllPieceMoves(position, Piece::Pawn, color);

    int x = rightmostBit(position.PieceBitBoards[Piece::King][color]);
    int i = 0;

    if(x == -1)
        return 0;

    Move castling[2];
    generateCastling(position, castling, rightmostBit(position.PieceBitBoards[Piece::King][color]));
    BitBoard enPassant = generateEnPassant(position);

    for (int j = 0; j < 2; j++)
    {
        if (castling[j].type == MoveType::Castling)
        {
            *(it++) = castling[j];
            moveCount++;
        }
    }

    FORBIT(j, enPassant)
    {
        *(it++) = Move(MoveType::EnPassant, j, position.EnPassant, Piece::Pawn);
        moveCount++;
    }

    moveCount += convertBitBoardToMoves(kingMoves[0], x, Piece::King, it);

    i = 0;
    FORBIT(j, position.PieceBitBoards[Piece::Queen][color])
    {
        moveCount += convertBitBoardToMoves(queenMoves[i], j, Piece::Queen, it);
        i++;
    }

    i = 0;
    FORBIT(j, position.PieceBitBoards[Piece::Rook][color])
    {
        moveCount += convertBitBoardToMoves(rookMoves[i], j, Piece::Rook, it);
        i++;
    }

    i = 0;
    FORBIT(j, position.PieceBitBoards[Piece::Bishop][color])
    {
        moveCount += convertBitBoardToMoves(bishopMoves[i], j, Piece::Bishop, it);
        i++;
    }

    i = 0;
    FORBIT(j, position.PieceBitBoards[Piece::Knight][color])
    {
        moveCount += convertBitBoardToMoves(knightMoves[i], j, Piece::Knight, it);
        i++;
    }

    i = 0;
    FORBIT(j, position.PieceBitBoards[Piece::Pawn][color])
    {
        moveCount += convertBitBoardToMoves(pawnMoves[i] & 72057594037927680ULL, j, Piece::Pawn, it);

        FORBIT(k, pawnMoves[i] & 18374686479671623935ULL)
        {
            *(it++) = Move(MoveType::Promotion, j, k, Piece::Queen);
            *(it++) = Move(MoveType::Promotion, j, k, Piece::Rook);
            *(it++) = Move(MoveType::Promotion, j, k, Piece::Knight);
            *(it++) = Move(MoveType::Promotion, j, k, Piece::Bishop);
            moveCount += 4;
        }

        i++;
    }

    return moveCount;
}


void generateAllPieceMoves(const Position& position, Piece piece, Color color)
{
    int i = 0;

    FORBIT(square, position.PieceBitBoards[piece][color])
    {
        switch(piece)
        {
            case Piece::Pawn:
                pawnMoves[i] = generatePawnMoves(position, square);
                break;
            case Piece::Knight:
                knightMoves[i] = generateKnightMoves(position, square);
                break;
            case Piece::Bishop:
                bishopMoves[i] = generateBishopMoves(position, square);
                break;
            case Piece::Rook:
                rookMoves[i] = generateRookMoves(position, square);
                break;
            case Piece::Queen:
                queenMoves[i] = generateQueenMoves(position, square);
                break;
            case Piece::King:
                kingMoves[i] = generateKingMoves(position, square);
                break;
        }

        i++;
    }
}

int convertBitBoardToMoves(BitBoard bitboard, Square square, Piece piece, Move*& it)
{
    int count = 0;

    FORBIT(i, bitboard)
    {
        it->type = MoveType::Normal;
        it->origin = square;
        it->destiny = i;
        it->piece = piece;
        it++;
        count++;
    }

    return count;
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

    if (position.ToMove == Color::White)
    {
        BitBoard push = (1ULL << (square + 8)) & ~occupancy;
        BitBoard doublePush = 4278190080ULL & (push << 8) & ~occupancy;
        BitBoard captures = (5ULL << (square + 7)) & (255ULL << ((square / 8 + 1) * 8)) & position.BlackOccupancy;
        return push | doublePush | captures;
    }
    else
    {
        BitBoard push = (1ULL << (square - 8)) & ~occupancy;
        BitBoard doublePush = 1095216660480ULL & (push >> 8) & ~occupancy;
        BitBoard captures = (5ULL << (square - 9)) & (255ULL << ((square / 8 - 1) * 8)) & position.WhiteOccupancy;
        return push | doublePush | captures;
    }
}

BitBoard generateKnightMoves(const Position& position, Square square)
{
    BitBoard ownPieces = (position.ToMove == Color::White) ? position.WhiteOccupancy : position.BlackOccupancy;
    return knightBitBoard[square] & ~ownPieces;
}

BitBoard generateKingMoves(const Position& position, Square square)
{
    BitBoard ownPieces = (position.ToMove == Color::White) ? position.WhiteOccupancy : position.BlackOccupancy;
    return kingBitBoard[square] & ~ownPieces;
}

//answer tem tamanho 2
void generateCastling(const Position& position, Move* answer, Square square)
{
    answer[0].type = MoveType::Normal;
    answer[1].type = MoveType::Normal;

    if(inCheck(position, position.ToMove, square))
        return;

    bool kingside = (position.ToMove == Color::White) ? position.Castling[0][0] : position.Castling[0][1];
    bool queenside = (position.ToMove == Color::White) ? position.Castling[1][0] : position.Castling[1][1];

    BitBoard occupancy = position.WhiteOccupancy | position.BlackOccupancy;

    if(kingside && !(occupancy & (1ULL << (square + 1))) && !(occupancy & (1ULL << (square + 2)))
        && !inCheck(position, position.ToMove, square + 1) && !inCheck(position, position.ToMove, square + 2))
    {
        *(answer++) = Move(MoveType::Castling, square, square + 2, Piece::King);
    }

    if(queenside && !(occupancy & (1ULL << (square - 1))) && !(occupancy & (1ULL << (square - 2))) && !(occupancy & (1ULL << (square - 3)))
        && !inCheck(position, position.ToMove, square - 1) && !inCheck(position, position.ToMove, square - 2))
    {
        *(answer++) = Move(MoveType::Castling, square, square - 2, Piece::King);
    }
}


//returns the positions of the pawns that can capture en passant, not their destinations
BitBoard generateEnPassant(const Position& position)
{
    if (position.EnPassant == -1)
        return 0ULL;

    if (position.ToMove == Color::White)
        return position.PieceBitBoards[Piece::Pawn][position.ToMove] & (5ULL << (position.EnPassant - 9)) & 1095216660480ULL;
    else
        return position.PieceBitBoards[Piece::Pawn][position.ToMove] & (5ULL << (position.EnPassant + 7)) & 4278190080ULL;
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
        knightBitBoard[i] = 0ULL;

        int rank = i / 8;
        int file = i % 8;

        if(rank + 2 <= 7 && file + 1 <= 7)
            knightBitBoard[i] |= (1ULL << (i + 17));
        if(rank + 2 <= 7 && file - 1 >= 0)
            knightBitBoard[i] |= (1ULL << (i + 15));
        if(rank - 2 >= 0 && file + 1 <= 7)
            knightBitBoard[i] |= (1ULL << (i - 15));
        if(rank - 2 >= 0 && file - 1 >= 0)
            knightBitBoard[i] |= (1ULL << (i - 17));
        if(rank + 1 <= 7 && file + 2 <= 7)
            knightBitBoard[i] |= (1ULL << (i + 10));
        if(rank + 1 <= 7 && file - 2 >= 0)
            knightBitBoard[i] |= (1ULL << (i + 6));
        if(rank - 1 >= 0 && file + 2 <= 7)
            knightBitBoard[i] |= (1ULL << (i - 6));
        if(rank - 1 >= 0 && file - 2 >= 0)
            knightBitBoard[i] |= (1ULL << (i - 10));
    }
}


void initializeKingBitBoard()
{
    for(int i = 0; i < 64; i++)
    {
        kingBitBoard[i] = 0ULL;

        int rank = i / 8;
        int file = i % 8;

        if(rank + 1 <= 7 && file + 1 <= 7)
            kingBitBoard[i] |= (1ULL << (i + 9));
        if(rank + 1 <= 7)
            kingBitBoard[i] |= (1ULL << (i + 8));
        if(rank + 1 <= 7 && file - 1 >= 0)
            kingBitBoard[i] |= (1ULL << (i + 7));
        if(file - 1 >= 0)
            kingBitBoard[i] |= (1ULL << (i - 1));
        if(file + 1 <= 7)
            kingBitBoard[i] |= (1ULL << (i + 1));
        if(rank - 1 >= 0 && file - 1 >= 0)
            kingBitBoard[i] |= (1ULL << (i - 9));
        if(rank - 1 >= 0 && file + 1 <= 7)
            kingBitBoard[i] |= (1ULL << (i - 7));
        if(rank - 1 >= 0)
            kingBitBoard[i] |= (1ULL << (i - 8));
    }
}


bool inCheck(const Position& position, Color color)
{
    return inCheck(position, color, rightmostBit(position.PieceBitBoards[Piece::King][color]));
}

bool inCheck(const Position& position, Color color, Square square)
{
    BitBoard ownPieces = (color == Color::White) ? position.WhiteOccupancy : position.BlackOccupancy;
    BitBoard occupancy = position.WhiteOccupancy | position.BlackOccupancy;

    int opponentIndex = oppositeColor(color);

    if((knightBitBoard[square] & ~ownPieces) & position.PieceBitBoards[Piece::Knight][opponentIndex])
        return true;

     if((Rmagic(square, occupancy) & ~ownPieces) & (position.PieceBitBoards[Piece::Rook][opponentIndex] | position.PieceBitBoards[Piece::Queen][opponentIndex]))
        return true;


    if((Bmagic(square, occupancy) & ~ownPieces) & (position.PieceBitBoards[Piece::Bishop][opponentIndex] | position.PieceBitBoards[Piece::Queen][opponentIndex]))
        return true;


    BitBoard pawnChecks = (color == Color::White) ? ((1ULL << (square + 7)) | (1ULL << (square + 9))) : ((1ULL << (square - 7)) | (1ULL << (square - 9)));

    if(pawnChecks & position.PieceBitBoards[Piece::Pawn][oppositeColor(color)])
        return true;

    if(kingBitBoard[square] & position.PieceBitBoards[Piece::King][oppositeColor(color)])
        return true;

    return false;
}
