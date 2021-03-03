#include "movegeneration.h"
#include "magicmoves.h"

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

    BitBoard result = Bmagic(square,occupancy) & ~ownPieces;

    return convertBitBoardToMoves(result, square, Piece::Bishop);
}


std::vector<Move> generatePawnMoves(Position position, Square square)
{
    std::vector<Move> pushes = generatePawnPushes(position, square);
    std::vector<Move> captures = generatePawnCaptures(position, square);
    std::vector<Move> enpassant = generateEnPassant(position, square);

    pushes.insert(pushes.end(), captures.begin(), captures.end());
    pushes.insert(pushes.end(), enpassant.begin(), enpassant.end());

    return pushes;
}

std::vector<Move> generateKnightMoves(Position position, Square square)
{
    BitBoard ownPieces = (position.ToMove == Color::White) ? position.WhiteOccupancy : position.BlackOccupancy;
    BitBoard result = knightMoves[square] & ~ownPieces;

    return convertBitBoardToMoves(result, square, Piece::Knight);
}

std::vector<Move> generateKingMoves(Position position, Square square)
{
    BitBoard ownPieces = (position.ToMove == Color::White) ? position.WhiteOccupancy : position.BlackOccupancy;
    BitBoard result = kingMoves[square] & ~ownPieces;

    std::vector<Move> answer = convertBitBoardToMoves(result, square, Piece::King);
    std::vector<Move> castling = generateCastling(position, square);

    castling.insert(castling.end(), answer.begin(), answer.end());

    return castling;
}

std::vector<Move> generateCastling(Position position, Square square)
{
    std::vector<Move> answer = std::vector<Move>();

    if(inCheck(position, position.ToMove, square))
        return answer;

    bool kingside = (position.ToMove == Color::White) ? position.Castling[0][0] : position.Castling[0][1];
    bool queenside = (position.ToMove == Color::White) ? position.Castling[1][0] : position.Castling[1][1];

    BitBoard occupancy = position.WhiteOccupancy | position.BlackOccupancy;

    if(kingside && !(occupancy & (1ULL << (square + 1))) && !(occupancy & (1ULL << (square + 2)))
        && !inCheck(position, position.ToMove, square + 1) && !inCheck(position, position.ToMove, square + 2))
    {
        answer.push_back(Move(MoveType::Castling, square, square + 2, Piece::King));
    }

    if(queenside && !(occupancy & (1ULL << (square - 1))) && !(occupancy & (1ULL << (square - 2))) && !(occupancy & (1ULL << (square - 3)))
        && !inCheck(position, position.ToMove, square - 1) && !inCheck(position, position.ToMove, square - 2))
    {
        answer.push_back(Move(MoveType::Castling, square, square - 2, Piece::King));
    }

    return answer;
}

std::vector<Move> generatePromotions(Square origin, Square destiny)
{
    std::vector<Move> answer = std::vector<Move>();

    answer.push_back(Move(MoveType::Promotion, origin, destiny, Piece::Queen));
    answer.push_back(Move(MoveType::Promotion, origin, destiny, Piece::Rook));
    answer.push_back(Move(MoveType::Promotion, origin, destiny, Piece::Knight));
    answer.push_back(Move(MoveType::Promotion, origin, destiny, Piece::Bishop));

    return answer;
}


std::vector<Move> generateEnPassant(Position position, Square square)
{
    std::vector<Move> answer = std::vector<Move>();

    if(position.EnPassant != -1 && ((position.ToMove == Color::Black && (position.EnPassant + 7 == square || position.EnPassant + 9 == square))
        || (position.ToMove == Color::White && (position.EnPassant - 7 == square || position.EnPassant - 9 == square))))
    {
        answer.push_back(Move(MoveType::EnPassant, square, position.EnPassant, Piece::Pawn));
    }

    return answer;
}


std::vector<Move> generatePawnPushes(Position position, Square square)
{
    std::vector<Move> answer = std::vector<Move>();
    BitBoard occupancy = (uint64_t)position.WhiteOccupancy + (uint64_t)position.BlackOccupancy;


    if(position.ToMove == Color::White)
    {
        if(square / 8 == 1 && !(occupancy & (1ULL << (square + 8))) && !(occupancy & (1ULL << (square + 16))))
            answer.push_back(Move(MoveType::Normal, square, square + 16, Piece::Pawn));

        if(!(occupancy & (1ULL << (square + 8))))
        {
            if(square / 8 == 6)
            {
                std::vector<Move> promotions = generatePromotions(square, square + 8);
                answer.insert(answer.end(), promotions.begin(), promotions.end());
            }
            else
            {
                answer.push_back(Move(MoveType::Normal, square, square + 8, Piece::Pawn));;
            }
        }

    }
    else
    {

        if(square / 8 == 6 && !(occupancy & (1ULL << (square - 8))) && !(occupancy & (1ULL << (square - 16))))
            answer.push_back(Move(MoveType::Normal, square, square - 16, Piece::Pawn));

        if(!(occupancy & (1ULL << (square - 8))))
        {
            if(square / 8 == 1)
            {
                std::vector<Move> promotions = generatePromotions(square, square - 8);
                answer.insert(answer.end(), promotions.begin(), promotions.end());
            }
            else
            {
                answer.push_back(Move(MoveType::Normal, square, square - 8, Piece::Pawn));
            }
        }
    }

    return answer;
}


std::vector<Move> generatePawnCaptures(Position position, Square square)
{
    std::vector<Move> answer = std::vector<Move>();

    if(position.ToMove == Color::White)
    {
        if(position.BlackOccupancy & (1ULL << (square + 7)))
        {
            if(square / 8 == 6)
            {
                std::vector<Move> promotions = generatePromotions(square, square + 7);
                answer.insert(answer.end(), promotions.begin(), promotions.end());
            }
            else
            {
                answer.push_back(Move(MoveType::Normal, square, square + 7, Piece::Pawn));
            }
        }

        if(position.BlackOccupancy & (1ULL << (square + 9)))
        {
            if(square / 8 == 6)
            {
                std::vector<Move> promotions = generatePromotions(square, square + 9);
                answer.insert(answer.end(), promotions.begin(), promotions.end());
            }
            else
            {
                answer.push_back(Move(MoveType::Normal, square, square + 9, Piece::Pawn));
            }
        }
    }
    else
    {
        if(position.WhiteOccupancy & (1ULL << (square - 7)))
        {
            if(square / 8 == 1)
            {
                std::vector<Move> promotions = generatePromotions(square, square - 7);
                answer.insert(answer.end(), promotions.begin(), promotions.end());
            }
            else
            {
                answer.push_back(Move(MoveType::Normal, square, square - 7, Piece::Pawn));
            }
        }

        if(position.WhiteOccupancy & (1ULL << (square - 9)))
        {
            if(square / 8 == 1)
            {
                std::vector<Move> promotions = generatePromotions(square, square - 9);
                answer.insert(answer.end(), promotions.begin(), promotions.end());
            }
            else
            {
                answer.push_back(Move(MoveType::Normal, square, square - 9, Piece::Pawn));
            }
        }
    }

    return answer;
}







std::vector<Move> convertBitBoardToMoves(BitBoard bitboard, Square square, Piece piece)
{
    std::vector<Move> answer = std::vector<Move>();

    for(int i = 0; i < 64; i++)
    {
        //Check if i-th bit is set
        if(bitboard & (1ULL << i))
        {
            answer.push_back(Move(MoveType::Normal, square, i, piece));
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


bool inCheck(Position position, Color color)
{
    return inCheck(position, color, position.PieceLocations[Piece::King][color][0]);
}

bool inCheck(Position position, Color color, Square square)
{
    BitBoard ownPieces = (color == Color::White) ? position.WhiteOccupancy : position.BlackOccupancy;
    BitBoard occupancy = position.WhiteOccupancy | position.BlackOccupancy;

    int opponentIndex = (color == Color::White) ? 1 : 0;

    if((knightMoves[square] & ~ownPieces) & position.PieceBitBoards[Piece::Knight][opponentIndex])
        return true;

     if((Rmagic(square, occupancy) & ~ownPieces) & (position.PieceBitBoards[Piece::Rook][opponentIndex] | position.PieceBitBoards[Piece::Queen][opponentIndex]))
        return true;


    if((Bmagic(square, occupancy) & ~ownPieces) & (position.PieceBitBoards[Piece::Bishop][opponentIndex] | position.PieceBitBoards[Piece::Queen][opponentIndex]))
        return true;


    BitBoard pawnChecks = (color == Color::White) ? ((1ULL << (square + 7)) | (1ULL << (square + 9))) : ((1ULL << (square - 7)) | (1ULL << (square - 9)));

    if(pawnChecks & position.PieceBitBoards[Piece::Pawn][oppositeColor(color)])
        return true;

    return false;
}
