/**
    \file types.h

    \brief File containing various types and stuff.
*/

#pragma once
#ifndef TYPES_H
#define TYPES_H


#include <bits/stdc++.h>


#define oppositeColor(x) x == Color::White ? Color::Black : Color::White

#define countBits __builtin_popcountll

#define rightmostBit(x) (ffsll(x) - 1)

#define FORBIT(i,bitboard) for (BitBoard bb##i = bitboard, i = 0; (i = rightmostBit(bb##i)) != 0xffffffffffffffffULL; bb##i &= ~(1ULL << i))

typedef uint64_t BitBoard;

typedef short Square; /*!< Squares are labeled as numbers from 0 to 63. A1 (bottom left corner) is 0, A2 is 1, A3 is 2,..., and H8 (top right corner) is 63.
                        More formally, should a square correspond to the i-th rank (0 based, i.e., A is 0, B is 1, ..., H is 7) and on the j-th file (again 0-based),
                        its corresponding number is 8i+j  */


/**
    Enum containing the color of the pieces
*/
enum Color
{
    White,
    Black,
};

/**
    Enum containing the different types of pieces
*/
enum Piece
{
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};

char pieceChar(Piece, Color);

bool readPiece(char, std::pair<Piece, Color>&);


/**
    Enum containing the different types of moves the generation algorithm can produce
*/
enum MoveType
{
    Normal, /*!< A normal move. Can be a check or capture (except enpassant) */
    EnPassant,/*!< Enpassant capture */
    Castling, /*!<  Player castled */
    Promotion /*!< Player promoted */
};


/**
    Struct representing a chess move.
*/
struct Move
{
    MoveType type; /*!< The type of move */
    Square origin; /*!< The origin square of the piece */
    Square destiny; /*!< The destiny square of the piece (in case of enpassant, it is the square the pawn lands in) */
    Piece piece; /*!< The moved piece or, if the move is a promotion, the piece the pawn was promoted to */

    /**
        Standard constructor

        \param t The type of move
        \param o The origin square
        \param d The destiny square
        \param p The moved piece
    */
    Move(MoveType t, Square o, Square d, Piece p)
    {
        type = t;
        origin = o;
        destiny = d;
        piece = p;
    }

    Move()
    {

    }

    std::string toString(Color);
    std::string toString();

    std::string squareToString(Square);
};

#endif //TYPES_H
