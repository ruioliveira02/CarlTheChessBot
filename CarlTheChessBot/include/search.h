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

//1M
#define BUCKETS 1000000
//5M
#define STORED_POSITIONS 5000000

struct node
{
    node* next;
    Position position;
    int depth;
    pair<Move, evaluation> ans;

    node() { }

    void set(const Position& position, int depth, const pair<Move, evaluation>& ans)
    {
        this->position = position;
        this->depth = depth;
        this->ans = ans;
        next = nullptr;
    }
};

class linked_list
{

private:
    static int master_code;

    int code;
    int length;
    node* first;
    node* last;

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
        last = nullptr;
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
        if (length == 0 || code != master_code)
        {
            code = master_code;
            first = n;
        }
        else
            last->next = n;

        last = n;
        length++;
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
