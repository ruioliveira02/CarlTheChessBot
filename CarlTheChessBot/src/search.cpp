#include "search.h"
#include "game.h"

using namespace std;
using namespace std::chrono;


//retorna o melhor movimento e a avaliação do game passado à função
pair<Move, evaluation> search(game game1)
{
	//TODO: tudo

	//do
	//{
	//
	//}
	//while (system_clock::now() <= limit);

	return minimax(game1, 3, evaluation());
}

//o Move que retorna pode não ter sido atribuído! (se não houver jogadas possíveis)
//nesse caso a evaluation é mate in 0 ou stalemate (evaluation.end_of_game() é true) ou ilegal (evaluation.isIllegal() é true)
pair<Move, evaluation> minimax(game game1, int depth, evaluation minmax)
{
	bool maximize = game1.position.ToMove == Color::White;
	Move best = Move();

	//checkmate
	if (inCheck(game1.position, oppositeColor(game1.position.ToMove)))
		return make_pair(best, evaluation());

	//draw (repetition, 50 moves, insufficient material, dead position...)
	//basically everything except stalemate

	if (game1.isDraw())
		return make_pair(best, evaluation(0.0));

	if (depth == 0)
		return make_pair(best, game1.evaluate());

	vector<Move> moves = generateAllMoves(game1.position);
	std::vector<game> games = std::vector<game>(moves.size());

	for (int i = 0; i < moves.size(); i++)
		games[i] = game1.makeMove(moves[i]);

	bool empty = true;
	evaluation minimax2 = maximize ? evaluation::minimum() : evaluation::maximum();

	for (int i = 0; i < games.size(); i++)
	{
		pair<Move, evaluation> p = {Move(), evaluation()}; //minimax(games[i], depth - 1, minimax2);

		if (p.second.isIllegal())
			continue;

		p.second.nextMove(game1.position.ToMove);
		empty = false;

		if (maximize ? minmax < p.second : p.second < minmax)
			return p;

		if (maximize ? minimax2 < p.second : p.second < minimax2)
		{
			minimax2 = p.second;
			best = p.first;
		}
	}

	//stalemate
	if (empty && !inCheck(game1.position, game1.position.ToMove))
		return make_pair(best, evaluation());

	return make_pair(best, minimax2);
}
