/**
    \file evaluation.h

    \brief File containing the definition of the Evaluation class, used to describe the evaluation of a given chess position.
*/

#pragma once
#ifndef EVALUATION_H
#define EVALUATION_H

#include "types.h"
#include <string>
#include <limits>

using namespace std;

/**
    The class used to describe the evaluation of a given chess position
*/
class evaluation
{
	public:
        /** Empty constructor. Returns an unitialized variable. */
		evaluation();

		/** Constructor for mate evaluations. */
		evaluation(int, Color);

		/** Constructor which fills the score with the given value. */
		evaluation(double);

        /** The minimum possible evaluation */
		static evaluation minimum();

		/** The maximum possible evaluation */
		static evaluation maximum();

		/** A forced draw */
		static evaluation draw();

		/** Comparator between two evaluations.

            One evaluation X  is smaller than another Y if it is better for black than Y.
		 */
		bool operator<(const evaluation&) const;

		void add(double delta);

		void subtract(double delta);

		/**
            Function that updates the number of moves until mate by augmenting it by one.
        */
		void nextMove(Color);

		/**
            Function that converts the evaluation to its score

            \return         The resulting score
        */
		double toScore() const;

		/**
            Function that converts the evaluation to a string

            \return         The resulting string
        */
		string toString() const;

		/**
            Function that returns whether the game is over or not

            \return         Whether the game is over or not
        */
		bool endOfGame() const;

    private:
        /** The color which will be delivering the mate (i.e. who wins) */
		Color color;

		/** How many moves until move*/
		int mateIn;

		/** Whether the position is a forced draw or not */
		bool isDraw;   //TODO: draw_in ?

		/** The score of the given position*/
		double score;
};


#endif
