/**
    \file game.h

    \brief File containing the class definition
*/

#pragma once
#ifndef GAME_H
#define GAME_H

#include "position.h"
#include "evaluation.h"


#define STORE_HIERARCHY 0


/**
    Class to store a game, i.e., the current position and all the previous positions and often they occured.
*/
class game
{
	public:
        /**
            Function that initializes all the bitboards and magic needed for move generation.
        */
        static void initialize();

        /**
            Function that returns the game which results from making the given move

            \param move     The given move
            \return         The game with the move played
        */
		game makeMove(Move) const;

        /**
            Function that returns whether or not the given piece exists on the current position
            \param piece    The given piece
            \return         The game with the move played
        */
		bool exists(Piece) const;


        /**
            Function that returns whether the game is a forced draw or not

            \return         Whether the game is a forced draw or not
        */
		bool isDraw() const;


		/**
            Yaaa.... eu apaguei isto... agora é uma variável dentro da Position


            Function that evaluates the current position

            \return         The evaluation of the current position
        */

        #if STORE_HIERARCHY
        stack<std::string> previous_positions;
        stack<Move> previous_moves;
        #endif

    public:
        /** The current position */
        Position position;

    private:
        /**
            Function that updates the occupancy bitboards of the same which made the given move.

            \param move     The given move
        */
        void updateBitBoards(Move);

        /**
            Function that captures the opponent's piece on the destination square of the given move.

            If no capture is to be done, it doesn't change anything.

            \param move     The given move
        */
        void assessCapture(Move);

        /**
            Function that applies the rook movement when castling

            \param move     The given move (assumed to be of type castling)
        */
        void castle(Move);

        /**
            Function that returns the game which results from making the given move

            \param move     The given move
        */
        void captureEnpassant(Move);

        /**
            Function that promotes the pawn to the piece given in the move

            \param move     The given move
        */
        void promote(Move);

        /**
            Function that updates the pieces involved in the given move.

            \param move     The given move
        */
        void updatePieces(Move);

        /**
            Function that updates the castling rights after the given move has been played.

            \param move     The given move
        */
        void updateCastlingRights(Move);

        void updateEnPassant(Move);
};


#endif
