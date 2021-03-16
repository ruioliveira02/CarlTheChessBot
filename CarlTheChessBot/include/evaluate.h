/**
    \file evaluate.h

    \brief File containing the functions used to evaluate any given position.
*/

#pragma once
#ifndef EVALUATE_H_
#define EVALUATE_H_


#include "position.h"
#include "evaluation.h"


/**
    Function that evaluates a given position.

    Note that it is a static evaluation, meaning that checks, checkmates, stalemates, draws by repetition, .etc, are not evaluated by this function. Instead, this function
    assesses the material count and the position of the pieces to make its evaluation of the position.

    \param position The given position
    \return         The evaluation of the position
*/
evaluation evaluatePosition(const Position&);


/**
    Function that evaluates one side (white or black) of a given position.

    The evaluation of a position corresponds to the evaluation of the white side minus the evaluation of the black side.

    \param position The given position
    \param color    The given color
    \return         The vector containing the requested moves
*/
//double evaluateSide(const Position&, Color);


/**
    Function that evaluates the material situation for one of the sides.

    For example, if the given side has two pawns, a queen and two knights, the function returns 2*1 + 1*9 + 2*3 = 17 .

    \param position The given position
    \param color    The given color
    \return         The vector containing the requested moves
*/
//double material(const Position&, Color);


/**
    Function that evaluates the location of the pieces for a given side of a position, based on the piece square tables below.

    \param position The given position
    \param color    The given color
    \param color    The given color
    \return         The vector containing the requested moves
*/
double pieceLocations(const Position&, Color);



/**
    The value (in pawns) of each piece in chess, in the order of the Piece enum.

    Note that bishops are worth 3.25 pawns, instead of 3.
*/
const double pieceValues[6] = {1.0, 3.0, 3.25, 5.0, 9.0, 0.0}; //King does not count for evaluation purposes


/**
    The piece square tables used to assess the position of the pieces on the chessboard.

    Taken from \link https://www.chessprogramming.org/Simplified_Evaluation_Function
*/

const double pieceSquaresTable[7][64] = {
    //Pawn
    {100, 100, 100, 100, 100, 100, 100, 100,
     150, 150, 150, 150, 150, 150, 150, 150,
     110, 110, 120, 130, 130, 120, 110, 110,
     105, 105, 110, 125, 125, 110, 105, 105,
     100, 100, 100, 120, 120, 100, 100, 100,
     105,  95,  90, 100, 100,  90,  95, 105,
     105, 110, 110,  80,  80, 110, 110, 105,
     100, 100, 100, 100, 100, 100, 100, 100},
    //Knight
    {250, 260, 270, 270, 270, 270, 260, 250,
     260, 280, 300, 300, 300, 300, 280, 260,
     270, 300, 310, 315, 315, 310, 300, 270,
     270, 305, 315, 320, 320, 315, 305, 270,
     270, 300, 315, 320, 320, 315, 300, 270,
     270, 305, 310, 315, 315, 310, 305, 270,
     260, 280, 300, 305, 305, 300, 280, 260,
     250, 260, 270, 270, 270, 270, 260, 250},
    //Bishop
    {305, 315, 315, 315, 315, 315, 315, 305,
     315, 325, 325, 325, 325, 325, 325, 305,
     315, 325, 330, 335, 335, 330, 325, 315,
     315, 330, 330, 335, 335, 330, 330, 315,
     315, 325, 335, 335, 335, 335, 325, 315,
     315, 335, 335, 335, 335, 335, 335, 315,
     315, 330, 325, 325, 325, 325, 330, 315,
     305, 315, 315, 315, 315, 315, 315, 305},
    //Rook
    {500, 500, 500, 500, 500, 500, 500, 500,
     505, 510, 510, 510, 510, 510, 510, 505,
     495, 500, 500, 500, 500, 500, 500, 495,
     495, 500, 500, 500, 500, 500, 500, 495,
     495, 500, 500, 500, 500, 500, 500, 495,
     495, 500, 500, 500, 500, 500, 500, 495,
     495, 500, 500, 500, 500, 500, 500, 495,
     500, 500, 500, 505, 505, 500, 500, 500},
    //Queen
    {880, 890, 890, 895, 895, 890, 890, 880,
     890, 900, 900, 900, 900, 900, 900, 890,
     890, 900, 905, 905, 905, 905, 900, 890,
     895, 900, 905, 905, 905, 905, 900, 895,
     900, 900, 905, 905, 905, 905, 900, 895,
     890, 905, 905, 905, 905, 905, 900, 890,
     890, 900, 905, 900, 900, 900, 900, 890,
     880, 890, 890, 895, 895, 890, 890, 880},
    //King middle game
    {-30, -40, -40, -50, -50, -40, -40, -30,
     -30, -40, -40, -50, -50, -40, -40, -30,
     -30, -40, -40, -50, -50, -40, -40, -30,
     -30, -40, -40, -50, -50, -40, -40, -30,
     -20, -30, -30, -40, -40, -30, -30, -20,
     -10, -20, -20, -20, -20, -20, -20, -10,
      20,  20,   0,   0,   0,   0,  20,  20,
      20,  30,  10,   0,   0,  10,  30,  20},
    //King endgame
    {-50, -40, -30, -20, -20, -30, -40, -50,
     -30, -20, -10,   0,   0, -10, -20, -30,
     -30, -10,  20,  30,  30,  20, -10, -30,
     -30, -10,  30,  40,  40,  30, -10, -30,
     -30, -10,  30,  40,  40,  30, -10, -30,
     -30, -10,  20,  30,  30,  20, -10, -30,
     -30, -30,   0,   0,   0,   0, -30, -30,
     -50, -30, -30, -30, -30, -30, -30, -50}
};

#endif // EVALUATE_H_
