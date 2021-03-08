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

//10M
#define BUCKETS 10000000
//100M
#define STORED_POSITIONS 100000000

struct node
{
    node* next;
    tuple<Position, int, pair<Move, evaluation>> value;

    node() { }

    void set(const Position& position, int depth, const pair<Move, evaluation>& ans)
    {
        value = make_tuple(position, depth, ans);
        next = nullptr;
    }
};

class linked_list
{

private:
    int length;
    node* first;
    node* last;

public:
    linked_list()
    {
        length = 0;
        first = nullptr;
        last = nullptr;
    }

    node* get(const Position& p)
    {
        for (node* it = first; it != nullptr; it = it->next)
            if (std::get<0>(it->value) == p)
                return it;

        return nullptr;
    }

    void add(node* n)
    {
        if (length == 0)
            first = n;
        else
            last->next = n;

        last = n;
        length++;
    }

    int size()
    {
        return length;
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

    ~searcher()
    {
        delete[] hash_table;
        delete[] stored_positions;
    }

    linked_list& get_bucket(const Position&);

    void add(linked_list&, const Position&, int, const pair<Move, evaluation>&);
};

/**
    Function that generates all the pseudo legal moves all the pieces of a certain color and type (e.g. white rooks, black pawns) can make.

    \param game     The given game
    \return         A pair consisting of the best move and the evaluation of the position
*/
std::pair<Move, evaluation> search(const game&);


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
