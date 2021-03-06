/**
    \file position.h

    \brief File containing the definition of the Position class.
*/


#pragma once
#ifndef POSITION_H
#define POSITION_H

#include "types.h"
#include "evaluation.h"



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
                        && /*WhiteControl == a.WhiteControl && BlackControl == a.BlackControl
                            &&*/ a.ToMove == ToMove && EnPassant == a.EnPassant;

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

        unsigned long long hash() const;

        void print() const;

        std::string toFEN() const;

        Move moveFromString(std::string);

        BitBoard calculateControl(Color); //definida no movegeneration.cpp (pq precisa das cenas de lá)

    private:
        Square squareFromString(std::string);
        Piece  pieceFromChar(char);
        Piece  pieceOnSquare(Square);

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

        /**
            The bitboard corresponding to the squares controlled by the white pieces (a piece does not control itself).
        */
        BitBoard WhiteControl;

        /**
            The bitboard corresponding to the squares controlled by the black pieces (a piece does not control itself).
        */
        BitBoard BlackControl;


        evaluation currentEvaluation;


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
