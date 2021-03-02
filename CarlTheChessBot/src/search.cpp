#include "position.h"
#include "evaluation.h"
#include <chrono>
#include <tuple>
#include <vector>

using namepsace std;
using namepsace std::chrono;

pair<Move, evaluation> search(const Position& position, system_clock::time_point limit)
{
	//TODO: tudo

	do
	{

	}
	while (system_clock::now() <= limit);

	return minimax(position, 3, position.ToMove == Color::White ? minimum_evaluation : maximum_evaluation);
}

//o Move que retorna pode não ter sido atribuído! (se não houver jogadas possíveis)
//nesse caso a evaluation é mate in 0 ou stalemate.
pair<Move, evaluation> minimax(const Position& position, int depth, evaluation minmax)
{
	if (depth == 0)
		return position.evaluate();

	bool maximize = position.ToMove == Color::White;

	//checkmate
	if (inCheck(position, (position.ToMove + 1) % 2))
		return evaluation(0, position.ToMove);

	vector<Moves> moves = generateAllMoves(position);

	//stalemate
	if (moves.length == 0 && !inCheck(position, position.ToMove))
		return evaluation();

	vector<Position> positions(moves.length());

	for (int i = 0; i < moves.length(); i++)
		positions[i] = position.makeMove(moves[i]);

	evaluation minimax2 = maximize ? minimum_evaluation : maximum_evaluation;
	Move best;

	for (int i = 0; i < positions.length(); i++)
	{
		pair<Move, evaluation> p = minimax(positions[i], depth - 1, minimax2);
		p.second.nextMove(position.ToMove);

		if (maximize ? p.second >= minimax : p.second <= minimax)
			return p;

		if (maximize ? p.second > minimax2 : p.second < minimax2)
		{
			minimax2 = p.second;
			best = p.first;
		}
	}

	return make_pair(best, minimax2);
}