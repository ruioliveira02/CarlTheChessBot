#include "search.h"
#include "game.h"

#include <iostream>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace std::chrono;

#define LOG if (SHOW_SEARCH_TREE) out << string(4 * (initial_depth - depth), ' ')

bool DEBUG = true;
bool SHOW_SEARCH_TREE = false;


time_point<system_clock> end_time;
int initial_depth;
long long nodes_found;
long long nodes_visited;
long long nodes_calculated;
long long dp_skips;
long long heuristic_skips;
long long draw_skips;
long long alpha_beta_skips;

int linked_list::master_code = 0;
searcher searcher;
std::ofstream out;

//duration está em milisegundos. o tempo não é exato... pode demorar mais um bocadinho
//retorna antes se completar a pesquisa de profundidade max_depth
//se max_depth < 0, nao tem limite. para max_depth = 0 retorna um valor nao inicializado
pair<Move, evaluation> search(const game& game1, long long duration, int max_depth)
{
	end_time = system_clock::now() + milliseconds(duration);
	pair<Move, evaluation> ans;
	searcher.clear();

	if (DEBUG || SHOW_SEARCH_TREE)
		out = std::ofstream("out.txt");

	for (initial_depth = 1; system_clock::now() < end_time && (max_depth < 0 || initial_depth <= max_depth); initial_depth++)
	{
		nodes_found = 1;
		nodes_visited = 0;
		nodes_calculated = 0;

		dp_skips = 0;
		heuristic_skips = 0;
		draw_skips = 0;
		alpha_beta_skips = 0;

		if (SHOW_SEARCH_TREE)
			out << "\nDEPTH: " << initial_depth << endl;

		auto current_begin_time = system_clock::now();
		auto aux = minimax(game1, initial_depth, evaluation::minimum(), evaluation::maximum());

		if (system_clock::now() < end_time)
			ans = aux;
		else
		{
			if (DEBUG)
				out << "\nTIME LIMIT REACHED. DISCARDING DEPTH " << initial_depth
				 	 << "\nRETURN DELAY: " << ((system_clock::now() - end_time).count() / 1000000000.0) << " seconds\n\n" << endl;

			return ans;
		}

		if (DEBUG)
		{
			out << "\nDEPTH: " << initial_depth
				 << "\nTIME: " << (duration_cast<nanoseconds>(system_clock::now() - current_begin_time)).count() / 1000000000.0 << " seconds"
				 << "\nBEST MOVE: " << ans.first.toString(game1.position.ToMove)
				 << "\nEVALUATION: " << ans.second.toString() << endl;

			out << "\nOPTIMAL GAME:";
			game g = game1;

			for (int i = 0; i < initial_depth; i++)
			{
				Move best = searcher.get_bucket(g.position).get(g.position)->moves[0];
				out << " " << best.toString(i % 2 == 0 ? game1.position.ToMove : oppositeColor(game1.position.ToMove));
				g = g.makeMove(best);
			}

			out << endl;
			int max = 0;
			double average = 0;
			srand(time(NULL)); //fixar seed para ver os mesmos buckets

			for (int i = 0; i < 1000; i++)
			{
				int n = searcher.hash_table[rand() % BUCKETS].size();
				//out << n << endl;
				if (n > max)
					max = n;
				average += n;
			}

			average /= 1000;

			out << "\nAVERAGE ENTRIES PER HASH TABLE BUCKET: " << average
				 << "\nMAX ENTRIES PER HASH TABLE BUCKET: " << max
				 << "\nTOTAL POSITIONS STORED: " << searcher.stored_count << endl;

			out << "\nNODES FOUND: " << nodes_found
			 	 << "\nNODES VISITED: " << nodes_visited
				 << "\nNODES CALCULATED: " << nodes_calculated << endl;

			out << "\nDP SKIPS: " << dp_skips
				 << "\nHEURISTIC SKIPS: " << heuristic_skips
				 << "\nDRAW SKIPS: " << draw_skips
				 << "\nALPHA BETA SKIPS: " << alpha_beta_skips << "\n" << endl;
		}
	}

	if (DEBUG || SHOW_SEARCH_TREE)
		out.close();

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
    	out << "\n\nSTORED POSITIONS LIMIT REACHED\n\n\n";

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


double pieceValues[6] = { 1, 3, 3.5, 5, 9, 999 };
int* scores_pointer;
Move sort_aux[300];

int compareScore(int a, int b)
{
    return scores_pointer[a] > scores_pointer[b];
}

//ordena os moves de provavelmente melhor para provavelmente pior
void sortMoves(const Position& pos, Move* moves, int size)
{
/*
    int indices[size];
    int scores[size];

    for (int i = 0; i < size; i++)
    {
        //promoção e roque é fixe
        if (moves[i].type == MoveType::Promotion)
            scores[i] += 50;
        else if (moves[i].type == MoveType::Castling)
            scores[i] += 25;

        BitBoard destBoard = 1ULL << moves[i].destiny;

        //ir para um sítio controlado pelo oponente é mau
        //if (destBoard & (pos.ToMove == Color::White ? pos.BlackControl : pos.WhiteControl))
        //    scores[i] -= pieceValues[moves[i].piece];

        //capturas sao boas (TODO: ver que peças estao a ser capturadas)
        if (destBoard & (pos.ToMove == Color::White ? pos.BlackOccupancy : pos.WhiteOccupancy))
            scores[i] += 4 - 0.3 * pieceValues[moves[i].piece];
    }

    for (int i = 0; i < size; i++)
    {
        indices[i] = i;
        sort_aux[i] = moves[i];
    }

    scores_pointer = scores;
    sort(indices, indices + size, compareScore);

    for (int i = 0; i < size; i++)
        moves[i] = sort_aux[indices[i]];
    */
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
			ans = make_pair(Move(), capturesMinimax(game1, alpha, beta)/*game1.evaluate()*/);
			heuristic_skips++;
		}
		else
		{
			ans = make_pair(Move(), evaluation::draw());
			draw_skips++;
		}

		if (search == nullptr)
			searcher.add(bucket, game1.position, depth, nullptr, 0, ans);

		return ans;
	}

	Move moves[300];
	int indices[300];
	int size;
	evaluation evaluations[300];
	Move* result = search == nullptr ? nullptr : search->moves;

	if (result != nullptr)
	{
		size = search->move_no;

		LOG << "RETRIEVED " << size << " PSEUDOLEGAL MOVES\n" << endl;
	}
	else
	{
		size = generateAllMoves(game1.position, moves);
		nodes_calculated++;

		LOG << "FOUND " << size << " PSEUDOLEGAL MOVES\n" << endl;
	}


	nodes_found += size;
	sortMoves(game1.position, moves, size);
	evaluation value = maximize ? evaluation::minimum() : evaluation::maximum();
	bool empty = true;
	bool skipped = false;
	int i;

	for (i = 0; i < size; i++)
	{
		if (system_clock::now() >= end_time)
			break;

		if (result != nullptr)
			moves[i] = result[i];

		game g = game1.makeMove(moves[i]);

		//the position is invalid
		if (inCheck(g.position, game1.position.ToMove))
		{
			LOG << "Skipping illegal move " << moves[i].toString(game1.position.ToMove) << "\n\n";
			nodes_found--;

			if (result == nullptr)
				moves[i] = moves[size - 1];
			else
				result[i] = result[size - 1];

			i--;
			size--;
			continue;
		}

		LOG << "SEARCHING " << moves[i].toString(game1.position.ToMove) << endl;
		evaluations[i] = minimax(g, depth - 1, alpha, beta).second;
		LOG << "EVALUATION: " << evaluations[i].toString() << " Alpha=" << alpha.toString() << "; Beta=" << beta.toString() << "\n" << endl;

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

		//alpha beta pruning
		if (!(alpha < beta))
		{
			LOG << "Alpha beta skip... Alpha=" << alpha.toString() << "; Beta=" << beta.toString() << endl;
			alpha_beta_skips++;

			skipped = true;
			break;
		}
	}

	//stalemate
	if (empty && !inCheck(game1.position, game1.position.ToMove))
		value = evaluation::draw();

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
	else
	{
        int max = 0;

        if (maximize)
        {
            for (int i = 0; i < size; i++)
                if (evaluations[max] < evaluations[i])
                    max = i;
        }
        else
        {
            for (int i = 0; i < size; i++)
                if (evaluations[i] < evaluations[max])
                    max = i;
        }

        return make_pair(size > 0 ? moves[max] : Move(), skipped ? (maximize ? evaluation::maximum() : evaluation::minimum()) : value);
	}

	auto ans = make_pair(size > 0 ? result[0] : Move(), skipped ? (maximize ? evaluation::maximum() : evaluation::minimum()) : value);

	if (search == nullptr)
		searcher.add(bucket, game1.position, depth, result, size, ans);
	else
	{
		search->depth = depth;
		search->ans = ans;
		search->moves = result;
		search->move_no = size;
	}

	return ans;
}

int depth = 0;
Move captures_array[9600]; //300 * 32

evaluation capturesMinimax(const game& game1, evaluation alpha, evaluation beta)
{
    if (game1.position.currentEvaluation.endOfGame())
        return game1.position.currentEvaluation;

    Move* captures = captures_array + (depth * 300);
    evaluation best = game1.position.currentEvaluation;
    bool maximize = game1.position.ToMove == Color::White;
    int size = generateAllCaptures(game1.position, captures);
    sortMoves(game1.position, captures, size);

    if (maximize)
    {
        if (alpha < best)
            alpha = best;
    }
    else if (best < beta)
        beta = best;

    //alpha beta pruning
    if (!(alpha < beta))
        return best;

    for (int i = 0; i < size; i++)
    {
        game game2 = game1.makeMove(captures[i]);

        //se o movimento é inválido
        if (inCheck(game2.position, game1.position.ToMove))
            continue;

        depth++;
        evaluation eval = capturesMinimax(game2, alpha, beta);
        depth--;


        //novo melhor encontrado
        if (maximize ? best < eval : eval < best)
            best = eval;

        if (maximize)
		{
			if (alpha < eval)
				alpha = eval;
		}
		else if (eval < beta)
			beta = eval;

		//alpha beta pruning
		if (!(alpha < beta))
			return best;
    }

    return best;
}
