/**
    \file game.h

    \brief File containing the class definition
*/


#ifndef GAME_H
#define GAME_H

#include "position.h"
#include "evaluation.h"


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
		game makeMove(Move);

        /**
            Function that returns whether or not the given piece exists on the current position
            \param piece    The given piece
            \return         The game with the move played
        */
		bool exists(Piece);


        /**
            Function that returns whether the game is a forced draw or not

            \return         Whether the game is a forced draw or not
        */
		bool isDraw();


		/**
            Function that evaluates the current position

            \return         The evaluation of the current position
        */
		evaluation evaluate();



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
};


#endif
