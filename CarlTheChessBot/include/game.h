#ifndef GAME_H
#define GAME_H

#include "position.h"
#include "evaluation.h"

class game
{
	public:

		Position position;

		game makeMove(Move);

		bool isDraw();
}


#endif