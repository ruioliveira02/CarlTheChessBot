#include "search.h"
#include "game.h"
#include <iostream>

using namespace std;

#define DEBUG false

int initial_depth = 10;
long long total_positions_analysed = 0;

//retorna o melhor movimento e a avaliação do game passado à função
pair<Move, evaluation> search(game game1)
{
	//TODO: tudo

	//do
	//{
	//
	//}
	//while (system_clock::now() <= limit);

	initial_depth = 4;
	total_positions_analysed = 0;
	pair<Move, evaluation> p = minimax(game1, initial_depth,
		game1.position.ToMove == Color::White ? evaluation(1, Color::White) : evaluation(1, Color::Black));

	if (DEBUG)
		cout << "\nFINAL EVALUAITON: " << p.second.toString()
			 << "\nTOTAL POSITIONS ANALYSED: " << total_positions_analysed
			 << "\nBEST MOVE: " << p.first.toString(game1.position.ToMove) << endl;

	return p;
}

//recebe posições VÁLIDAS! comportamento indefinido para posições inválidas
//o Move que retorna pode não ter sido atribuído! (se não houver jogadas possíveis)
//nesse caso a evaluation é mate in 0 ou stalemate (evaluation.end_of_game() é true)
pair<Move, evaluation> minimax(game game1, int depth, evaluation minmax)
{
	bool maximize = game1.position.ToMove == Color::White;
	Move best = Move();
	total_positions_analysed++;

	//draw (repetition, 50 moves, insufficient material, dead position...)
	//basically everything except stalemate
	if (game1.isDraw())
		return make_pair(best, evaluation());

	//stop search and use heuristic
	if (depth == 0)
		return make_pair(best, game1.evaluate());

	vector<Move> moves = generateAllMoves(game1.position);
	std::vector<game> games = std::vector<game>(moves.size());

	for (int i = 0; i < moves.size(); i++)
		games[i] = game1.makeMove(moves[i]);

	if (DEBUG)
		cout << string(4 * (initial_depth - depth), ' ') << "FOUND " << games.size() << " PSEUDOLEGAL MOVES\n\n";

	bool empty = true;
	evaluation minmax2 = maximize ? evaluation::minimum() : evaluation::maximum();

	for (int i = 0; i < games.size(); i++)
	{
		//the position in invalid
		if (inCheck(games[i].position, game1.position.ToMove))
		{
			if (DEBUG)
				cout << string(4 * (initial_depth - depth), ' ') << "SKIPPING ILLEGAL MOVE " << moves[i].toString(game1.position.ToMove) << "\n";

			continue;
		}

		if (DEBUG)
			cout << string(4 * (initial_depth - depth), ' ') << "SEARCHING " << moves[i].toString(game1.position.ToMove) << "\n";

		evaluation e = minimax(games[i], depth - 1, minmax2).second;

		if (DEBUG)
			cout << string(4 * (initial_depth - depth), ' ') << "EVALUATION: " << e.toString() << "\n\n";

		e.nextMove(game1.position.ToMove);
		empty = false;

		//guaranteed best move (checkmate in 1)
		if (maximize ? !(e < evaluation(1, Color::White)) : !(evaluation(1, Color::Black) < e))
			return make_pair(moves[i], e);

		//alpha beta pruning
		if (maximize ? !(e < minmax) : !(minmax < e))
		{
			if (DEBUG)
				cout << string(4 * (initial_depth - depth), ' ') << "Alpha beta skip...\n";

			return make_pair(moves[i], e);
		}

		//new best move found
		if (maximize ? minmax2 < e : e < minmax2)
		{
			minmax2 = e;
			best = moves[i];
		}
	}

	//stalemate
	if (empty && !inCheck(game1.position, game1.position.ToMove))
		return make_pair(best, evaluation());

	return make_pair(best, minmax2);
}
