#include "search.h"
#include "game.h"

#include <iostream>
#include <chrono>
#include <unordered_map>

using namespace std;
using namespace std::chrono;

#define DEBUG true
#define SHOW_SEARCH_TREE false
#define LOG if (SHOW_SEARCH_TREE) cout << string(4 * (initial_depth - depth), ' ')


int initial_depth;
long long nodes_found;
long long nodes_visited;
long long nodes_calculated;
long long dp_skips;
long long heuristic_skips;
long long draw_skips;
long long alpha_beta_skips;
long long mate_in_one_skips;

int linked_list::master_code = 0;
searcher searcher;


//retorna o melhor movimento e a avaliação do game passado à função
pair<Move, evaluation> search(const game& game1)
{
	//TODO: tudo

	//do
	//{
	//
	//}
	//while (system_clock::now() <= limit);

	//dp.clear();

	searcher.clear();

	initial_depth = 6;
	nodes_found = 1;
	nodes_visited = 0;
	nodes_calculated = 0;

	dp_skips = 0;
	heuristic_skips = 0;
	draw_skips = 0;
	alpha_beta_skips = 0;
	mate_in_one_skips = 0;

	auto begin = system_clock::now();
	pair<Move, evaluation> p = minimax(game1, initial_depth, evaluation::minimum(), evaluation::maximum());

	if (DEBUG)
	{
		cout << "\nFINAL EVALUAITON: " << p.second.toString()
			 << "\nBEST MOVE: " << p.first.toString(game1.position.ToMove)
			 << "\nTIME: " << (duration_cast<nanoseconds>(system_clock::now() - begin)).count() / 1000000000.0 << " seconds" << endl;
	
		int max = 0;
		double average = 0;
		srand(time(NULL)); //fixar seed para ver os mesmos buckets

		for (int i = 0; i < 1000; i++)
		{
			int n = searcher.hash_table[rand() % BUCKETS].size();
			//cout << n << endl;
			if (n > max)
				max = n;
			average += n;
		}

		average /= 1000.0;

		cout << "\nAVERAGE ENTRIES PER HASH TABLE BUCKET: " << average
			 << "\nMAX ENTRIES PER HASH TABLE BUCKET: " << max
			 << "\nTOTAL POSITIONS STORED: " << searcher.stored_count << endl;

		cout << "\nTOTAL NODES FOUND: " << nodes_found
		 	 << "\nTOTAL NODES VISITED: " << nodes_visited
			 << "\nTOTAL NODES CALCULATED: " << nodes_calculated
			 << "\nDP SKIPS: " << dp_skips
			 << "\nHEURISTIC SKIPS: " << heuristic_skips
			 << "\nDRAW SKIPS: " << draw_skips
			 << "\nALPHA BETA SKIPS: " << alpha_beta_skips
			 << "\nMATE IN ONE SKIPS: " << mate_in_one_skips << endl;
	}

	return p;
}

linked_list& searcher::get_bucket(const Position& p)
{
	return hash_table[p.hash() % BUCKETS];
}

void searcher::add(linked_list& bucket, const Position& p, int depth, const pair<Move, evaluation>& ans)
{
    stored_positions[stored_count].set(p, depth, ans);
    bucket.add(&stored_positions[stored_count]);
    stored_count++;

    if (stored_count >= STORED_POSITIONS)
    {
        //TODO: reaproveitar memória?
        cout << "ERROR: Insificient memory to store positions" << endl;
        throw overflow_error("Insificient memory to store positions");
    }
}

//recebe posições VÁLIDAS! comportamento indefinido para posições inválidas
//o Move que retorna pode não ter sido atribuído se depth == 0 ou se não houver jogadas possíveis
//nesse caso a evaluation é mate in 0 ou draw (evaluation.end_of_game() é true)
pair<Move, evaluation> minimax(const game& game1, int depth, evaluation alpha, evaluation beta)
{
	nodes_visited++;
	bool maximize = game1.position.ToMove == Color::White;
	Move best = Move();

	auto& bucket = searcher.get_bucket(game1.position);
	auto search = bucket.get(game1.position);

	//DP for the win
	if (search != nullptr && get<1>(search->value) >= depth)
	{
		dp_skips++;
		return get<2>(search->value);
	}

	//draw -> repetition, 50 moves, insufficient material, dead position... basically everything except stalemate
	//depth == 0 -> stop search and use heuristic
	if (game1.isDraw() || depth == 0)
	{
		pair<Move, evaluation> ans;

		if (depth == 0)
		{
			ans = make_pair(best, game1.evaluate());
			heuristic_skips++;
		}
		else
		{
			ans = make_pair(best, evaluation());
			draw_skips++;
		}

		if (search == nullptr)
			searcher.add(bucket, game1.position, depth, ans);
		else
		{
			get<1>(search->value) = depth;
			get<2>(search->value) = ans;
		}

		return ans;
	}

	auto aux = generateAllMoves(game1.position);
	Move* moves = aux.first;
	int size = aux.second;
	nodes_calculated++;

	LOG << "FOUND " << size << " PSEUDOLEGAL MOVES\n\n";
	nodes_found += size;

	bool empty = true;
	evaluation value = maximize ? evaluation::minimum() : evaluation::maximum();

	for (int i = 0; i < size; i++)
	{
		game g = game1.makeMove(moves[i]);

		//the position in invalid
		if (inCheck(g.position, game1.position.ToMove))
		{
			LOG << "Skipping illegal move " << moves[i].toString(game1.position.ToMove) << "\n\n";
			nodes_found--;
			continue;
		}

		LOG << "SEARCHING " << moves[i].toString(game1.position.ToMove) << "\n";
		evaluation e = minimax(g, depth - 1, alpha, beta).second;
		LOG << "EVALUATION: " << e.toString() << "\n\n";

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
		else if (value < beta)
			beta = value;

		//alpha beta pruning / guaranteed best move (checkmate in 1)
		if (!(alpha < beta) || (maximize ? !(e < evaluation(1, Color::White)) : !(evaluation(1, Color::Black) < e)))
		{
			if (!(alpha < beta))
			{
				LOG << "Alpha beta skip...\n";
				alpha_beta_skips++;
			}
			else
			{
				LOG << "Mate in 1 found, skip...\n";
				mate_in_one_skips++;
			}

			auto ans = moves[i];

			delete[] moves;
			return make_pair(ans, e);
		}
	}

	delete[] moves;

	//stalemate
	if (empty && !inCheck(game1.position, game1.position.ToMove))
		value = evaluation();

	auto ans = make_pair(best, value);

	if (search == nullptr)
		searcher.add(bucket, game1.position, depth, ans);
	else
	{
		get<1>(search->value) = depth;
		get<2>(search->value) = ans;
	}

	return ans;
}
