#ifndef EVALUATION_H
#define EVALUATION_H

#include "position.h"
#include <string>
#include <limits>

using namespace std;

class evaluation
{
	private:

		Color color; //para que cor é o mate (i. e. quem é que ganha)
		int mateIn;
		bool draw;   //TODO: draw_in ?
		double score;

	public:

		evaluation()
		{
			draw = true;
			mateIn = -1;
			score = 0;
		}

		evaluation(int mate_in, Color color)
		{
			this->color = color;
			this->mateIn = mate_in;
			draw = false;
		}

		evaluation(double score)
		{
			this->score = score;
			mateIn = -1;
			draw = false;
		}


		static evaluation minimum()
		{
			return evaluation(0, Color::Black);
		}

		static evaluation maximum()
		{
			return evaluation(0, Color::White);
		}

		//x < y if y is better for white
		bool operator<(const evaluation& eval)
		{
			if (mateIn >= 0 && color == Color::White)
				return eval.mateIn >= 0 && eval.mateIn < mateIn && eval.color == Color::White;

			if (mateIn >= 0 && color == Color::Black)
				return eval.mateIn < 0 || eval.color == Color::White || eval.mateIn > mateIn;

			if (eval.mateIn >= 0)
				return eval.color == Color::White;

			if (draw)
				return eval.score > 0;	//TODO...

			return score < eval.score;
		}

		void nextMove(Color toMove)
		{
			if (mateIn >= 0 && toMove == color)
				this->mateIn++;
		}

		double toScore()
		{
			if (mateIn >= 0)
				return color == Color::White ? numeric_limits<double>::infinity() : -numeric_limits<double>::infinity();

			if (draw)
				return 0;

			return score;
		}

		string toString()
		{
			if (mateIn >= 0)
				return "#" + to_string(color == Color::White ? mateIn : -mateIn);

			if (draw)
				return "£";

			return to_string(score);
		}

		bool endOfGame()
		{
			return mateIn == 0 || draw;
		}
};


#endif
