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
		int mate_in;
		bool draw;   //TODO: draw_in ?
		double score;

	public:

		evaluation()
		{
			draw = true;
			mate_in = -1;
			score = 0;
		}

		evaluation(int mate_in, Color color)
		{
			this->color = color;
			this->mate_in = mate_in;
			draw = false;
		}

		evaluation(double score)
		{
			this->score = score;
			mate_in = -1;
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

		//x < y se y é melhor para as brancas
		bool operator<(const evaluation& eval)
		{
			if (mate_in >= 0 && color == Color::White)
				return eval.mate_in >= 0 && eval.mate_in < mate_in && eval.color == Color::White;

			if (mate_in >= 0 && color == Color::Black)
				return eval.mate_in < 0 || eval.color == Color::White || eval.mate_in > mate_in;

			if (eval.mate_in >= 0)
				return eval.color == Color::White;

			if (draw)
				return eval.score > 0;	//TODO...

			return score < eval.score;
		}

		void nextMove(Color toMove)
		{
			if (mate_in >= 0 && toMove == color)
				this->mate_in++;
		}

		double toScore()
		{
			if (mate_in >= 0)
				return color == Color::White ? numeric_limits<double>::infinity() : -numeric_limits<double>::infinity();

			if (draw)
				return 0;

			return score;
		}

		string toString()
		{
			if (mate_in >= 0)
				return "#" + to_string(color == Color::White ? mate_in : -mate_in);

			if (draw)
				return "£";

			return to_string(score);
		}

		bool end_of_game()
		{
			return mate_in == 0 || draw;
		}
};


#endif
