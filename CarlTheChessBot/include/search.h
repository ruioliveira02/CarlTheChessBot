#ifndef SEARCH_H_
#define SEARCH_H_

#include "position.h"
#include "evaluation.h"
#include "movegeneration.h"
#include "game.h"
#include <chrono>
#include <tuple>
#include <vector>

using namespace std;
using namespace std::chrono;


pair<Move, evaluation> search(game);

pair<Move, evaluation> minimax(game, int, evaluation);


#endif
