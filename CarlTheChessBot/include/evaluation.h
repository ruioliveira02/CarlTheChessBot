#ifndef EVALUATION_H
#define EVALUATION_H

#include "position.h"
#include <string>
#include <limits>

using namespace std;

struct evaluation
{
	private:

		Color color; //para que cor é o mate (i. e. quem é que ganha)
		int mate_in;
		bool draw;   //TODO: draw_in ?
		double score;
		bool illegality;

	public:

		evaluation()
		{
			this->draw = true;
		}

		evaluation(int mate_in, Color color)
		{
			this->color = color;
			this->mate_in = mate_in;
		}

		evaluation(double score)
		{
			this->score = score;
		}


		static evaluation minimum()
		{
			return evaluation(0, Color::Black);
		}

		static evaluation maximum()
		{
			return evaluation(0, Color::White);
		}

		static evaluation illegal()
		{
			evaluation e;
			e.illegality = true;
			return e;
		}

		//x < y se y é melhor para as brancas
		bool operator<(const evaluation& eval)
		{
			if (mate_in >= 0 && color == Color::White)
				return eval.mate_in >= 0 && eval.mate_in < mate_in && eval.color == Color::White;

			if (mate_in >= 0 && color == Color::Black)
				return eval.mate_in < 0 || eval.color == Color::White || eval.mate_in > mate_in;

			if (draw)
				return eval.score > 0;	//TODO...

			return score < eval.score;
		}

		void nextMove(Color toMove)
		{
			if (mate_in >= 0 && toMove != color)
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

		bool isIllegal()
		{
			return illegality;
		}

		bool end_of_game()
		{
			return mate_in == 0 || draw;
		}
};


#endif