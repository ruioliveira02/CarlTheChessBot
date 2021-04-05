#include "movegeneration.h"
#include "magicmoves.h"

#define convertBitBoardToMoves(bitboard, square, pieceType, it) FORBIT(i, bitboard) { it->type = MoveType::Normal; it->origin = square; it->destiny = i; (it++)->piece = pieceType; }

BitBoard knightBitBoard[64];
BitBoard kingBitBoard[64];


//assume-se que a array it tem espaço suficiente para guardar todos os movimentos
//assume-se também que a posição é válida
//retorna todos os movimentos pseudo-legais
int generateAllMoves(const Position& position, Move* moves)
{
    Move* it = moves;
    Color color = position.ToMove;

    generateCastling(position, it, rightmostBit(position.PieceBitBoards[Piece::King][color]));
    generateEnPassant(position, it);

    Square kingPos = rightmostBit(position.PieceBitBoards[Piece::King][color]);
    convertBitBoardToMoves(generateKingMoves(position, kingPos), kingPos, Piece::King, it);

    FORBIT(j, position.PieceBitBoards[Piece::Queen][color])
        convertBitBoardToMoves(generateQueenMoves(position, j), j, Piece::Queen, it);

    FORBIT(j, position.PieceBitBoards[Piece::Rook][color])
        convertBitBoardToMoves(generateRookMoves(position, j), j, Piece::Rook, it);

    FORBIT(j, position.PieceBitBoards[Piece::Bishop][color])
        convertBitBoardToMoves(generateBishopMoves(position, j), j, Piece::Bishop, it);

    FORBIT(j, position.PieceBitBoards[Piece::Knight][color])
        convertBitBoardToMoves(generateKnightMoves(position, j), j, Piece::Knight, it);

    FORBIT(j, position.PieceBitBoards[Piece::Pawn][color])
    {
        BitBoard bb = generatePawnMoves(position, j);
        convertBitBoardToMoves(bb & 72057594037927680ULL, j, Piece::Pawn, it);

        FORBIT(k, bb & 18374686479671623935ULL)
        {
            *(it++) = Move(MoveType::Promotion, j, k, Piece::Queen);
            *(it++) = Move(MoveType::Promotion, j, k, Piece::Rook);
            *(it++) = Move(MoveType::Promotion, j, k, Piece::Knight);
            *(it++) = Move(MoveType::Promotion, j, k, Piece::Bishop);
        }
    }

    return it - moves;
}

int generateAllCaptures(const Position& position, Move* captures)
{
    Move* it = captures;
    Color color = position.ToMove;
    BitBoard oppositeOccupancy = color == Color::White ? position.BlackOccupancy : position.WhiteOccupancy;

    generateEnPassant(position, it);

    Square kingPos = rightmostBit(position.PieceBitBoards[Piece::King][color]);
    convertBitBoardToMoves(generateKingMoves(position, kingPos) & oppositeOccupancy, kingPos, Piece::King, it);

    FORBIT(j, position.PieceBitBoards[Piece::Queen][color])
        convertBitBoardToMoves(generateQueenMoves(position, j) & oppositeOccupancy, j, Piece::Queen, it);

    FORBIT(j, position.PieceBitBoards[Piece::Rook][color])
        convertBitBoardToMoves(generateRookMoves(position, j) & oppositeOccupancy, j, Piece::Rook, it);

    FORBIT(j, position.PieceBitBoards[Piece::Bishop][color])
        convertBitBoardToMoves(generateBishopMoves(position, j) & oppositeOccupancy, j, Piece::Bishop, it);

    FORBIT(j, position.PieceBitBoards[Piece::Knight][color])
        convertBitBoardToMoves(generateKnightMoves(position, j) & oppositeOccupancy, j, Piece::Knight, it);

    FORBIT(j, position.PieceBitBoards[Piece::Pawn][color])
    {
        BitBoard push = 1ULL << (j + 8);
        BitBoard bb = (((push & 18374403900871474942ULL) >> 1) | ((push & 9187201950435737471ULL) << 1)) & oppositeOccupancy;
        convertBitBoardToMoves(bb & 72057594037927680ULL, j, Piece::Pawn, it);

        FORBIT(k, bb & 18374686479671623935ULL)
        {
            *(it++) = Move(MoveType::Promotion, j, k, Piece::Queen);
            *(it++) = Move(MoveType::Promotion, j, k, Piece::Rook);
            *(it++) = Move(MoveType::Promotion, j, k, Piece::Knight);
            *(it++) = Move(MoveType::Promotion, j, k, Piece::Bishop);
        }
    }

    return it - captures;
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
    BitBoard freeSquares = ~(position.WhiteOccupancy | position.BlackOccupancy);

    if (position.ToMove == Color::White)
    {
        BitBoard push = (1ULL << (square + 8)) & freeSquares;
        BitBoard doublePush = 4278190080ULL & (push << 8) & freeSquares;
        BitBoard captures = (5ULL << (square + 7)) & (255ULL << ((square / 8 + 1) * 8)) & position.BlackOccupancy;
        return push | doublePush | captures;
    }
    else
    {
        BitBoard push = (1ULL << (square - 8)) & freeSquares;
        BitBoard doublePush = 1095216660480ULL & (push >> 8) & freeSquares;
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


void generateCastling(const Position& position, Move*& it, Square square)
{

    //BitBoard checks = checkBoard(position, position.ToMove);
    Color color = position.ToMove;
    bool check, calculated = false;

    bool kingside = (color == Color::White) ? position.Castling[0][0] : position.Castling[0][1];
    bool queenside = (color == Color::White) ? position.Castling[1][0] : position.Castling[1][1];

    BitBoard occupancy = position.WhiteOccupancy | position.BlackOccupancy;

    if (kingside && !(occupancy & (3ULL << (square + 1))) && /*!(checks & (3ULL << (square + 1)))*/!inCheck(position, color, square + 1) && !inCheck(position, color, square + 2))
    {
        if (!(check = inCheck(position, color, square)))
        {
            it->type = MoveType::Castling;
            it->origin = square;
            it->destiny = square + 2;
            (it++)->piece = Piece::King;
        }

        calculated = true;
    }

    if (queenside && !(occupancy & (7ULL << (square - 3))) && /*!(checks & (3ULL << (square - 2)))*/!inCheck(position, color, square - 1) && !inCheck(position, color, square - 2))
    {
        if (calculated ? !check : !inCheck(position, color, square))

        it->type = MoveType::Castling;
        it->origin = square;
        it->destiny = square - 2;
        (it++)->piece = Piece::King;
    }
}


//returns the positions of the pawns that can capture en passant, not their destinations
void generateEnPassant(const Position& position, Move*& it)
{
    if (position.EnPassant == -1)
        return;

    BitBoard enPassant;

    if (position.ToMove == Color::White)
        enPassant = position.PieceBitBoards[Piece::Pawn][position.ToMove] & (5ULL << (position.EnPassant - 9)) & 1095216660480ULL;
    else
        enPassant = position.PieceBitBoards[Piece::Pawn][position.ToMove] & (5ULL << (position.EnPassant + 7)) & 4278190080ULL;

    if (enPassant)
    {
        it->type = MoveType::EnPassant;
        it->origin = rightmostBit(enPassant);
        it->destiny = position.EnPassant;
        (it++)->piece = Piece::Pawn;
    }
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
    //return position.PieceBitBoards[Piece::King][color] & (color == Color::White ? position.BlackControl : position.WhiteControl);

    return inCheck(position, color, rightmostBit(position.PieceBitBoards[Piece::King][color]));
}

bool inCheck(const Position& position, Color color, Square square)
{
    //return (1ULL << square) & (color == Color::White ? position.BlackControl : position.WhiteControl);

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

BitBoard Position::calculateControl(Color color)
{
    BitBoard occupancy = WhiteOccupancy | BlackOccupancy;
    BitBoard ans = kingBitBoard[rightmostBit(PieceBitBoards[Piece::King][color])];

    FORBIT(i, PieceBitBoards[Piece::Knight][color])
        ans |= knightBitBoard[i];

    FORBIT(i, PieceBitBoards[Piece::Rook][color])
        ans |= Rmagic(i, occupancy);

    FORBIT(i, PieceBitBoards[Piece::Bishop][color])
        ans |= Bmagic(i, occupancy);

    FORBIT(i, PieceBitBoards[Piece::Queen][color])
        ans |= Rmagic(i, occupancy) | Bmagic(i, occupancy);

    BitBoard pushes = color == Color::White ? PieceBitBoards[Piece::Pawn][color] << 8 : PieceBitBoards[Piece::Pawn][color] >> 8;
    ans |= ((pushes & 18374403900871474942ULL) >> 1) | ((pushes & 9187201950435737471ULL) << 1);
    return ans;
}
