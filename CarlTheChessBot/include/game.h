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
};


#endif
