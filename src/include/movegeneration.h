#ifndef MOVEGENERATION_H_
#define MOVEGENERATION_H_


#include "position.h"



std::vector<Move> generateAllMoves(Position);
std::vector<Move> generateAllPieceMoves(Position,Piece,Color);
std::vector<Move> generateKingMoves(Position,Square);
std::vector<Move> generatePawnMoves(Position,Square);
std::vector<Move> generateKnightMoves(Position,Square);
std::vector<Move> generateBishopMoves(Position,Square);
std::vector<Move> generateRookMoves(Position,Square);
std::vector<Move> generateQueenMoves(Position,Square);


std::vector<Move> generateCastling(Position, Square);
std::vector<Move> generatePromotions(Position, Square);
std::vector<Move> generateEnPassant(Position, Square);


std::vector<Move> convertBitBoardToMoves(BitBoard, Square, Piece);

bool inCheck(Position,Color);

BitBoard knightMoves[64];
BitBoard kingMoves[64];

void initializeBitBoards();

void initializeKnightBitBoard();
void initializeKingBitBoard();

#endif // MOVEGENERATION_H_
