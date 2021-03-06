#include "search.h"
#include "game.h"
#include <iostream>

using namespace std;

#define DEBUG false

int initial_depth = 10;
long long total_positions_analysed = 0;

//retorna o melhor movimento e a avaliação do game passado à função
pair<Move, evaluation> search(const game& game1)
{
	//TODO: tudo

	//do
	//{
	//
	//}
	//while (system_clock::now() <= limit);

	initial_depth = 6;
	total_positions_analysed = 0;
	pair<Move, evaluation> p = minimax(game1, initial_depth, evaluation::minimum(), evaluation::maximum());

	if (DEBUG)
		cout << "\nFINAL EVALUAITON: " << p.second.toString()
			 << "\nTOTAL POSITIONS ANALYSED: " << total_positions_analysed
			 << "\nBEST MOVE: " << p.first.toString(game1.position.ToMove) << endl;

	return p;
}

//recebe posições VÁLIDAS! comportamento indefinido para posições inválidas
//o Move que retorna pode não ter sido atribuído! (se não houver jogadas possíveis)
//nesse caso a evaluation é mate in 0 ou stalemate (evaluation.end_of_game() é true)
pair<Move, evaluation> minimax(const game& game1, int depth, evaluation alpha, evaluation beta)
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


	auto aux = generateAllMoves(game1.position);
	Move* moves = aux.first;
	int size = aux.second;

	if (DEBUG)
		cout << string(4 * (initial_depth - depth), ' ') << "FOUND " << size << " PSEUDOLEGAL MOVES\n\n";

	bool empty = true;
	evaluation value = maximize ? evaluation::minimum() : evaluation::maximum();

	for (int i = 0; i < size; i++)
	{
		game g = game1.makeMove(moves[i]);

		//the position in invalid
		if (inCheck(g.position, game1.position.ToMove))
		{
			if (DEBUG)
				cout << string(4 * (initial_depth - depth), ' ') << "SKIPPING ILLEGAL MOVE " << moves[i].toString(game1.position.ToMove) << "\n";

			continue;
		}

		if (DEBUG)
			cout << string(4 * (initial_depth - depth), ' ') << "SEARCHING " << moves[i].toString(game1.position.ToMove) << "\n";

		evaluation e = minimax(g, depth - 1, alpha, beta).second;

		if (DEBUG)
			cout << string(4 * (initial_depth - depth), ' ') << "EVALUATION: " << e.toString() << "\n\n";

		e.nextMove(game1.position.ToMove);
		empty = false;

		//new best move found
		if (maximize ? value < e : e < value)
		{
			value = e;
			best = moves[i];
		}

		if (maximize)
		{
			if (alpha < value)
				alpha = value;
		}
		else
		{
			if (value < beta)
				beta = value;
		}

		//alpha beta pruning
		if (!(alpha < beta))
		{
			if (DEBUG)
				cout << string(4 * (initial_depth - depth), ' ') << "Alpha beta skip...\n";

			auto ans = moves[i];
			delete[] moves;
			return make_pair(ans, e);
		}

		//guaranteed best move (checkmate in 1)
		if (maximize ? !(e < evaluation(1, Color::White)) : !(evaluation(1, Color::Black) < e))
		{
			if (DEBUG)
				cout << string(4 * (initial_depth - depth), ' ') << "Mate in 1 found, skip...\n";

			auto ans = moves[i];
			delete[] moves;
			return make_pair(ans, e);
		}
	}

	delete[] moves;

	//stalemate
	if (empty && !inCheck(game1.position, game1.position.ToMove))
		return make_pair(best, evaluation());

	return make_pair(best, value);
}
