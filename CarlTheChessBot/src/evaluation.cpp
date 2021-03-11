#include "evaluation.h"

using namespace std;

evaluation::evaluation()
{
    draw = true;
    mateIn = -1;
    score = 0;
}

evaluation::evaluation(int mate_in, Color color)
{
    this->color = color;
    this->mateIn = mate_in;
    draw = false;
}

evaluation::evaluation(double score)
{
    this->score = score;
    mateIn = -1;
    draw = false;
}

evaluation evaluation::minimum()
{
    return evaluation(0, Color::Black);
}

evaluation evaluation::maximum()
{
    return evaluation(0, Color::White);
}

bool evaluation::operator <(const evaluation& eval)
{
    if (mateIn >= 0)
    {
        if (color == Color::White)
            return eval.mateIn >= 0 && eval.mateIn < mateIn && eval.color == Color::White;

        //color = Black
        return eval.mateIn < 0 || eval.mateIn > mateIn || eval.color == Color::White;
    }

    if (eval.mateIn >= 0)
        return eval.color == Color::White;

    if (draw)
        return eval.score > 0;	//TODO...

    return score < eval.score;
}


void evaluation::nextMove(Color toMove)
{
    if (mateIn >= 0 && toMove == color)
        this->mateIn++;
}


double evaluation::toScore()
{
    if (mateIn >= 0)
        return color == Color::White ? numeric_limits<double>::infinity() : -numeric_limits<double>::infinity();

    if (draw)
        return 0;

    return score;
}


string evaluation::toString()
{
    if (mateIn >= 0)
        return "#" + to_string(color == Color::White ? mateIn : -mateIn);

    if (draw)
        return "£";

    return to_string(score);
}


bool evaluation::endOfGame()
{
    return mateIn == 0 || draw;
}
