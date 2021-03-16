/**
    \file search.h

    \brief File containing the  functions used to perform the search for the best move in a given position.
*/

#pragma once
#ifndef SEARCH_H_
#define SEARCH_H_

#include "position.h"
#include "evaluation.h"
#include "movegeneration.h"
#include "game.h"
#include <tuple>
#include <vector>

//~1M (número primo para diminuir colisões... em teoria. n sei se faz diferença)
#define BUCKETS 1000003
//5M
#define STORED_POSITIONS 5000000

extern bool DEBUG;
extern bool SHOW_SEARCH_TREE;

struct node
{
    node* next;

    Position position;
    int depth;

    Move* moves;
    int move_no;
    std::pair<Move, evaluation> ans;

    node()
    {
        moves = nullptr;
    }

    ~node()
    {
        if (moves != nullptr)
            delete[] moves;
    }

    void set(const Position& position, int depth, Move* moves, int move_no, const pair<Move, evaluation>& ans)
    {
        if (this->moves != nullptr)
            delete[] this->moves;

        this->position = position;
        this->depth = depth;
        this->moves = moves;
        this->move_no = move_no;
        this->ans = ans;
        this->next = nullptr;
    }
};

class linked_list
{

private:
    static int master_code;

    int code;
    int length;
    node* first;

public:

    static void clearAll()
    {
        master_code++;
    }

    linked_list()
    {
        code = 0;
        length = 0;
        first = nullptr;
    }

    node* get(const Position& p)
    {
        if (code != master_code)
            return nullptr;

        for (node* it = first; it != nullptr; it = it->next)
            if (it->position == p)
                return it;

        return nullptr;
    }

    void add(node* n)
    {
        if (code != master_code)
        {
            code = master_code;
            length = 1;
        }
        else
        {
            n->next = first;
            length++;
        }

        first = n;
    }

    int size()
    {
        return code != master_code ? 0 : length;
    }
};

class searcher
{

public:
    linked_list* hash_table;
    node* stored_positions;
    int stored_count;

    searcher()
    {
        hash_table = new linked_list[BUCKETS];
        stored_positions = new node[STORED_POSITIONS];
        stored_count = 0;
    }

    void clear()
    {
        linked_list::clearAll();
        stored_count = 0;
    }

    ~searcher()
    {
        delete[] hash_table;
        delete[] stored_positions;
    }

    linked_list& get_bucket(const Position&);

    //retorna false se tiver acabado o espaço (já nao lança exceção)
    bool add(linked_list&, const Position&, int, Move*, int, const pair<Move, evaluation>&);
};

/**
    Function that generates all the pseudo legal moves all the pieces of a certain color and type (e.g. white rooks, black pawns) can make.

    \param game     The given game
    \return         A pair consisting of the best move and the evaluation of the position
*/
std::pair<Move, evaluation> search(const game&, long long duration, int max_depth);


/**
    Function that runs the minimax algorithm used to search the moves

    \param game     The given game
    \param depth    The given search depth
    \param alpha    Used for alpha beta pruning
    \param beta     Used for alpha beta pruning
    \return         A pair consisting of the best move and the evaluation of the position
*/
std::pair<Move, evaluation> minimax(const game&, int, evaluation, evaluation);


#endif
