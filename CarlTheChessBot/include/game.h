#ifndef GAME_H
#define GAME_H

#include "position.h"
#include "evaluation.h"

class game
{
	public:

        void initialize();

		Position position;

		game makeMove(Move);

		bool exists(Piece);

		bool isDraw();

		evaluation evaluate();


    private:
        void updateBitBoards(Move);
        void assessCapture(Move);
        void castle(Move);
        void captureEnpassant(Move);
        void promote(Move);
        void updatePieces(Move);
        void updateCastlingRights(Move);
};


#endif
