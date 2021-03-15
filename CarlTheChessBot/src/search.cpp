#include "search.h"
#include "game.h"

#include <iostream>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace std::chrono;

#define DEBUG true
#define SHOW_SEARCH_TREE true
#define LOG if (SHOW_SEARCH_TREE) cout << string(4 * (initial_depth - depth), ' ')

time_point<system_clock> end_time;
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


//duration está em milisegundos. o tempo não é exato... pode demorar mais um bocadinho
//retorna antes se completar a pesquisa de profundidade max_depth
pair<Move, evaluation> search(const game& game1, long long duration, int max_depth)
{
	end_time = system_clock::now() + milliseconds(duration);
	pair<Move, evaluation> ans;
	searcher.clear();

	for (initial_depth = 0; system_clock::now() < end_time && initial_depth <= max_depth; initial_depth++)
	{
		nodes_found = 1;
		nodes_visited = 0;
		nodes_calculated = 0;

		dp_skips = 0;
		heuristic_skips = 0;
		draw_skips = 0;
		alpha_beta_skips = 0;
		mate_in_one_skips = 0;

		auto current_begin_time = system_clock::now();
		auto aux = minimax(game1, initial_depth, evaluation::minimum(), evaluation::maximum());

		if (system_clock::now() < end_time)
			ans = aux;
		else
		{
			if (DEBUG)
				cout << "\nTIME LIMIT REACHED. DISCARDING DEPTH " << initial_depth
				 	 << "\nRETURN DELAY: " << ((system_clock::now() - end_time).count() / 1000000000.0) << " seconds\n\n" << endl;

			return ans;
		}

		if (DEBUG)
		{
			cout << "\n\nDEPTH: " << initial_depth
				 << "\nEVALUAITON: " << ans.second.toString()
				 << "\nBEST MOVE: " << ans.first.toString(game1.position.ToMove)
				 << "\nTIME: " << (duration_cast<nanoseconds>(system_clock::now() - current_begin_time)).count() / 1000000000.0 << " seconds" << endl;
		
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

			average /= 1000;

			cout << "\nAVERAGE ENTRIES PER HASH TABLE BUCKET: " << average
				 << "\nMAX ENTRIES PER HASH TABLE BUCKET: " << max
				 << "\nTOTAL POSITIONS STORED: " << searcher.stored_count << endl;

			cout << "\nNODES FOUND: " << nodes_found
			 	 << "\nNODES VISITED: " << nodes_visited
				 << "\nNODES CALCULATED: " << nodes_calculated << endl;

			cout << "\nDP SKIPS: " << dp_skips
				 << "\nHEURISTIC SKIPS: " << heuristic_skips
				 << "\nDRAW SKIPS: " << draw_skips
				 << "\nALPHA BETA SKIPS: " << alpha_beta_skips
				 << "\nMATE IN ONE SKIPS: " << mate_in_one_skips << endl;
		}
	}

	return ans;
}

linked_list& searcher::get_bucket(const Position& p)
{
	return hash_table[p.hash() % BUCKETS];
}

bool searcher::add(linked_list& bucket, const Position& p, int depth, Move* moves, int move_no, const pair<Move, evaluation>& ans)
{
	if (stored_count >= STORED_POSITIONS)
		return false;

    stored_positions[stored_count].set(p, depth, moves, move_no, ans);
    bucket.add(&stored_positions[stored_count]);
    stored_count++;

    if (DEBUG && stored_count == STORED_POSITIONS)
    	cout << "\n\nSTORED POSITIONS LIMIT REACHED\n\n\n";

    return true;
}

evaluation* evaluations_pointer;

int compareWhite(int a, int b)
{
	return evaluations_pointer[b] < evaluations_pointer[a];
}

int compareBlack(int a, int b)
{
	return evaluations_pointer[a] < evaluations_pointer[b];
}


//recebe posições VÁLIDAS! comportamento indefinido para posições inválidas
//o Move que retorna pode não ter sido atribuído se depth == 0 ou se não houver jogadas possíveis
//nesse caso a evaluation é mate in 0 ou draw (evaluation.end_of_game() é true)
pair<Move, evaluation> minimax(const game& game1, int depth, evaluation alpha, evaluation beta)
{
	if (system_clock::now() >= end_time)
		return make_pair(Move(), evaluation());

	nodes_visited++;
	bool maximize = game1.position.ToMove == Color::White;

	auto& bucket = searcher.get_bucket(game1.position);
	auto search = bucket.get(game1.position);

	//DP for the win
	if (search != nullptr && search->depth >= depth)
	{
		dp_skips++;
		return search->ans;
	}

	//draw -> repetition, 50 moves, insufficient material, dead position... basically everything except stalemate
	//depth == 0 -> stop search and use heuristic
	if (game1.isDraw() || depth == 0)
	{
		pair<Move, evaluation> ans;

		if (depth == 0)
		{
			ans = make_pair(Move(), game1.evaluate());
			heuristic_skips++;
		}
		else
		{
			ans = make_pair(Move(), evaluation());
			draw_skips++;
		}

		return ans;
	}

	Move* result = nullptr;
	Move moves[300];
	int indices[300];
	int size;
	evaluation evaluations[300];

	if (search != nullptr)
	{
		result = search->moves;
		size = search->move_no;

		LOG << "RETRIEVED " << size << " PSEUDOLEGAL MOVES\n\n";
	}
	else
	{
		size = generateAllMoves(game1.position, moves);
		nodes_calculated++;

		LOG << "FOUND " << size << " PSEUDOLEGAL MOVES\n\n";
	}


	nodes_found += size;	
	evaluation value = maximize ? evaluation::minimum() : evaluation::maximum();
	bool empty = true;
	int i;

	for (i = 0; i < size; i++)
	{
		if (system_clock::now() >= end_time)
			break;

		if (result != nullptr)
			moves[i] = result[i];

		game g = game1.makeMove(moves[i]);

		//the position in invalid
		if (inCheck(g.position, game1.position.ToMove))
		{
			LOG << "Skipping illegal move " << moves[i].toString(game1.position.ToMove) << "\n\n";
			nodes_found--;

			moves[i] = moves[size - 1];
			size--;
			i--;

			continue;
		}

		LOG << "SEARCHING " << moves[i].toString(game1.position.ToMove) << "\n";
		evaluations[i] = minimax(g, depth - 1, alpha, beta).second;
		LOG << "EVALUATION: " << evaluations[i].toString() << "\n\n";

		evaluations[i].nextMove(game1.position.ToMove);
		empty = false;

		//new best move found
		if (maximize ? value < evaluations[i] : evaluations[i] < value)
			value = evaluations[i];

		if (maximize)
		{
			if (alpha < value)
				alpha = value;
		}
		else if (value < beta)
			beta = value;

		//alpha beta pruning / guaranteed best move (checkmate in 1)
		if (!(alpha < beta) || (maximize ? !(evaluations[i] < evaluation(1, Color::White)) : !(evaluation(1, Color::Black) < evaluations[i])))
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

			value = evaluations[i];
			break;
		}
	}

	//stalemate
	if (empty && !inCheck(game1.position, game1.position.ToMove))
		value = evaluation();

	if (search != nullptr || searcher.stored_count < STORED_POSITIONS)
	{
		for (int j = 0; j < i; j++)
			indices[j] = j;

		evaluations_pointer = evaluations;
		sort(indices, indices + i, maximize ? compareWhite : compareBlack);
		
		if (result == nullptr)
		{
			result = new Move[size];

			for (int j = i; j < size; j++)
				result[j] = moves[j];
		}

		for (int j = 0; j < i; j++)
			result[j] = moves[indices[j]];
	}

	auto ans = make_pair(size > 0 ? result[0] : Move(), value);

	if (search == nullptr)
		searcher.add(bucket, game1.position, depth, result, size, ans);
	else
	{
		search->depth = depth;
		search->ans = ans;
		search->move_no = size;
	}

	return ans;
}
