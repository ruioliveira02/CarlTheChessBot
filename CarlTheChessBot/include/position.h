/**
    \file position.h

    \brief File containing the definition of the Position class and the various enums used to describe a chess position.
*/


#pragma once
#ifndef POSITION_H
#define POSITION_H

#include <bits/stdc++.h>

#define oppositeColor(x) x == Color::White ? Color::Black : Color::White

#define countBits(x) __builtin_popcountll(x)

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
};


/**
    Class used to store data regarding a chess position
*/
class Position
{
    public:
        /**
            Standard constructor
        */
        Position();

        /**
            FEN constructor
        */
        Position(const char*);

        /**
            Copy constructor
        */
        Position(const Position&);

        /**
            Constructor

            \param pieceLocations The location of the pieces
            \param whiteOccupancy The bitboard corresponding to the squares occupied by white pieces
            \param blackOccupancy The bitboard corresponding to the squares occupied by black pieces
            \param toMove         The color of the side to move
            \param castling       Castling rights of the players (first coordinate is the side to castle on (0: kingside, 1: queenside), and the second one is the color (0:White, 1:Black)
            \param enpassant      The square to which a pawn can enpassant to (same as in FEN notation) (-1 if there is no pawn that can be captured enpassant)
            \param halfMoves      The number of half moves since the last pawn push or capture (to account to fifty move rule)
            \param totalMoves     The number of total moves in the game
        */
        Position(std::vector<Square>[6][2],Color,bool[2][2],short,short,short);

        /**
            Virtual destructor
        */
        virtual ~Position();

        /**
            Comparator used to sort positions. They are sorted by the value of the WhiteOccupancy property.
        */
        bool operator < (const Position& a) const
        {
            return WhiteOccupancy < a.WhiteOccupancy;
        }

        /**
            Comparator to check for position equality.

            Two positions are considered equal if the pieces are on the same squares, it is the same player to move, and the enpassant and castling rights are the same-
        */
        bool operator == (const Position& a) const
        {
            bool first = WhiteOccupancy == a.WhiteOccupancy && a.BlackOccupancy == BlackOccupancy
                            && a.ToMove == ToMove && EnPassant == a.EnPassant;

            if(!first)
                return false;

            for(int i = 0; i < 2; i++)
                for(int j = 0; j < 2; j++)
                    if(a.Castling[i][j] != Castling[i][j])
                        return false;

            for(int i = 0; i < 6; i++)
                for(int j = 0; j < 2; j++)
                    if(a.PieceBitBoards[i][j] != PieceBitBoards[i][j])
                        return false;

            return true;
        }

        unsigned long long hash() const
        {
            //ignoring HalfMoves and TotalMoves...
            
            unsigned long long ans = 0ULL;

            for (int i = 0; i < 6; i++)
                for (int j = 0; j < 2; j++)
                    ans = (ans << 1) ^ PieceBitBoards[i][j];

            for (int i = 0; i < 6; i++)
                for (int j = 0; j < 2; j++)
                    ans = (ans >> 2) ^ PieceBitBoards[i][j];

            ans ^= Castling[0][0] ? 7506213756019786892ULL : 769871230598702352ULL;
            ans ^= Castling[0][1] ? 6947560976017010655ULL : 7605723695871603876ULL;
            ans ^= Castling[1][0] ? 569123876017860753ULL : 75910873607260375ULL;
            ans ^= Castling[1][1] ? 720395873860297367ULL : 6091764108734018973ULL;
            ans ^= EnPassant;

            return ans;
        }

        std::string toFEN() const;


    public:
        /**
            The Bitboards with the occupancies of pieces divided by color and type.

            The first coordinate is the piece type (as in the Piece enum order); second one is the color, as per the Color enum
        */
        BitBoard PieceBitBoards[6][2];

        /**
            The bitboard corresponding to the squares occupied by the white pieces.
        */
        BitBoard WhiteOccupancy;

        /**
            The bitboard corresponding to the squares occupied by the black pieces.
        */
        BitBoard BlackOccupancy;


        /** Color of the player to move */
        Color ToMove;

        /**
            The castling rights in the position. Does not mean that castling is legal, but that no pieces have been moved such that
            castling would be forever illegal.

            First coordinate is kingside/queenside (0 = kingside, 1 = queenside), the second one is white/black
        */
        bool Castling[2][2];

        /** Square of the pawn that can be captured en passant (-1 if there is no such pawn)*/
        short EnPassant;

        /** Number of half-moves since last capture or pawn push (50 move rule)*/
        short HalfMoves;

        /** Total number of moves in the game */
        short TotalMoves;
};


#endif // POSITION_H
