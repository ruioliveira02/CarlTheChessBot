/**
    \file movegeneration.h

    \brief File containing the  functions used to generate all the pseudo legals moves in a given chess position, as well as the functions to evaluate checks.

    Generating pseudo legal moves means that all the moves generated follow all the rules of chess, except for pins, meaning that a move which puts its own king in check can
    and will also be generated. Those cases are truncated by the search algorithm.
*/


#ifndef MOVEGENERATION_H_
#define MOVEGENERATION_H_


#include "evaluation.h"

/**
    Function that generates all the pseudo legal (not worrying about pins on the king or necessarily evading checks) moves in a position.

    \param position The given position
    \return         The vector containing the requested moves
*/
std::pair<Move*, int> generateAllMoves(const Position&);

/**
    Function that generates all the pseudo legal moves all the pieces of a certain color and type (e.g. white rooks, black pawns) can make.

    \param position The given position
    \param piece    The type of piece (rook, bishop, king, .etc)
    \param color    The color of the pieces to generate moves for
    \return         The vector containing the requested moves
*/
BitBoard* generateAllPieceMoves(const Position&,Piece,Color);

/**
    Function that vector of pseudo legals move a piece on the given square can make, given a bitboard in which all the set bits correspond to squares said piece can move to.

    \param bitBoard The given bitboard (the i-th bit is set if and only if the given piece can move to the corresponding square)
    \param square   The square the piece is on
    \param piece    The type of piece
    \return         The vector containing the requested moves
*/
void convertBitBoardToMoves(BitBoard, Square, Piece, Move*&);


/**
    Function that generates all the pseudo legal moves the king on the given square can make.

    \param position The given position
    \param square   The square the king is on
    \return         The vector containing the requested moves
*/
BitBoard generateKingMoves(const Position&,Square);

/**
    Function that generates all the pseudo legal moves the pawn on the given square can make.

    \param position The given position
    \param square   The square the pawn is on
    \return         The vector containing the requested moves
*/
BitBoard generatePawnMoves(const Position&,Square);

/**
    Function that generates all the pseudo legal moves the knight on the given square can make.

    \param position The given position
    \param square   The square the knight is on
    \return         The vector containing the requested moves
*/
BitBoard generateKnightMoves(const Position&,Square);

/**
    Function that generates all the pseudo legal moves the bishop on the given square can make.

    \param position The given position
    \param square   The square the bishop is on
    \return         The vector containing the requested moves
*/
BitBoard generateBishopMoves(const Position&,Square);

/**
    Function that generates all the pseudo legal moves the rook on the given square can make.

    \param position The given position
    \param square   The square the rook is on
    \return         The vector containing the requested moves
*/
BitBoard generateRookMoves(const Position&,Square);

/**
    Function that generates all the pseudo legal moves the queen on the given square can make.

    \param position The given position
    \param square   The square the queen is on
    \return         The vector containing the requested moves
*/
BitBoard generateQueenMoves(const Position&,Square);


/**
    Function that generates all the legal (not pseudo legal) castling moves the king on the given square can make.

    \param position The given position
    \param square   The square the king is on
    \return         The vector containing the requested moves
*/
Move* generateCastling(const Position&, Square);

/**
    Function that generates the pseudo legal enpassant capture the pawn on the given square can make (if there is no capture possible, returns the empty list).

    \param position The given position
    \param square   The square the pawn is on
    \return         The vector containing the requested moves
*/
BitBoard generateEnPassant(const Position&);


/**
    Function that checks whether the king of the given color is in check or not

    \param position The given position
    \param color    The color of the king
    \return         Whether the king of the given color is in check or not
*/
bool inCheck(const Position&,Color);


/**
    Function that checks whether the king of the given color is in check or not, supposing it sits on the given square.

    \param position The given position
    \param color    The color of the king
    \param square   The square the king is on
    \return         Whether the king of the given color is in check or not
*/
bool inCheck(const Position&,Color,Square);


/** The bitboards used to compute the movements of knights */
extern BitBoard knightMoves[64];

/** The bitboards used to compute the movements of kings */
extern BitBoard kingMoves[64];



/** Function that initializes the knights and king bitboards */
void initializeAllBitBoards();

/** Function that initializes the knight bitboards */
void initializeKnightBitBoard();

/** Function that initializes the kings bitboards */
void initializeKingBitBoard();

#endif // MOVEGENERATION_H_
