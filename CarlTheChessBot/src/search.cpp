#include "position.h"
#include "evaluation.h"
#include "game.h"
#include <chrono>
#include <tuple>
#include <vector>

using namepsace std;
using namepsace std::chrono;

//retorna o melhor movimento e a avaliação do game passado à função
pair<Move, evaluation> search(const game& game, system_clock::time_point limit)
{
	//TODO: tudo

	//do
	//{
	//
	//}
	//while (system_clock::now() <= limit);

	return minimax(game, 3, game.ToMove == Color::White ? evaluation::minimum : evaluation::maximum);
}

//o Move que retorna pode não ter sido atribuído! (se não houver jogadas possíveis)
//nesse caso a evaluation é mate in 0 ou stalemate (evaluation.end_of_game() é true) ou ilegal (evaluation.isIllegal() é true)
pair<Move, evaluation> minimax(const game& game, int depth, evaluation minmax)
{
	bool maximize = game.position.ToMove == Color::White;
	Move best;

	//checkmate
	if (inCheck(game.position, (game.position.ToMove + 1) % 2))
		return make_pair(best, evaluation::illegal);

	//draw (repetition, 50 moves, insufficient material, dead position...)
	//basically everything except stalemate
	if (game.isDraw())
		return make_pair(best, evaluation());

	if (depth == 0)
		return make_pair(best, game.evaluate());

	vector<Moves> moves = generateAllMoves(game.position);
	vector<game> games(moves.length());

	for (int i = 0; i < moves.length(); i++)
		games[i] = game.makeMove(moves[i]);

	bool empty = true;
	evaluation minimax2 = maximize ? evaluation::minimum : evaluation::maximum;

	for (int i = 0; i < games.length(); i++)
	{
		pair<Move, evaluation> p = minimax(games[i], depth - 1, minimax2);

		if (p.second.isIllegal())
			continue;

		p.second.nextMove(game.position.ToMove);
		empty = false;

		if (maximize ? p.second >= minimax : p.second <= minimax)
			return p;

		if (maximize ? p.second > minimax2 : p.second < minimax2)
		{
			minimax2 = p.second;
			best = p.first;
		}
	}

	//stalemate
	if (empty && !inCheck(game.position, game.position.ToMove))
		return make_pair(best, evaluation());

	return make_pair(best, minimax2);
}