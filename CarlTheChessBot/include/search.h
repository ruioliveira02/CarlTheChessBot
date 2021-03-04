/**
    \file search.h

    \brief File containing the  functions used to perform the search for the best move in a given position.
*/

#ifndef SEARCH_H_
#define SEARCH_H_

#include "position.h"
#include "evaluation.h"
#include "movegeneration.h"
#include "game.h"
#include <tuple>
#include <vector>


/**
    Function that generates all the pseudo legal moves all the pieces of a certain color and type (e.g. white rooks, black pawns) can make.

    \param game     The given game
    \return         A pair consisting of the best move and the evaluation of the position
*/
std::pair<Move, evaluation> search(game);


/**
    Function that runs the minimax algorithm used to search the moves

    \param game     The given game
    \param depth    The given search depth
    \param minmax   Threshold for alpha beta pruning
    \return         A pair consisting of the best move and the evaluation of the position
*/
std::pair<Move, evaluation> minimax(game, int, evaluation);


#endif
