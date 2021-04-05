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


//retorna o valor de uma peça. As peças pretas teem valor negativo
double value(Piece, Color, Square);


/**
    The piece square tables used to assess the position of the pieces on the chessboard.

    Taken from \link https://www.chessprogramming.org/Simplified_Evaluation_Function and added the value of each piece (as per pieceValues)
*/
const double pieceSquaresTable[7][64] = {
    //Pawn
    {1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
     1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
     1.10, 1.10, 1.20, 1.30, 1.30, 1.20, 1.10, 1.10,
     1.05, 1.05, 1.10, 1.25, 1.25, 1.10, 1.05, 1.05,
     1.00, 1.00, 1.00, 1.20, 1.20, 1.00, 1.00, 1.00,
     1.05, 0.95, 0.90, 1.00, 1.00, 0.90, 0.95, 1.05,
     1.05, 1.10, 1.10, 0.80, 0.80, 1.10, 1.10, 1.05,
     1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00},
    //Knight
    {2.50, 2.60, 2.70, 2.70, 2.70, 2.70, 2.60, 2.50,
     2.60, 2.80, 3.00, 3.00, 3.00, 3.00, 2.80, 2.60,
     2.70, 3.00, 3.10, 3.15, 3.15, 3.10, 3.00, 2.70,
     2.70, 3.05, 3.15, 3.20, 3.20, 3.15, 3.05, 2.70,
     2.70, 3.00, 3.15, 3.20, 3.20, 3.15, 3.00, 2.70,
     2.70, 3.05, 3.10, 3.15, 3.15, 3.10, 3.05, 2.70,
     2.60, 2.80, 3.00, 3.05, 3.05, 3.00, 2.80, 2.60,
     2.50, 2.60, 2.70, 2.70, 2.70, 2.70, 2.60, 2.50},
    //Bishop
    {3.05, 3.15, 3.15, 3.15, 3.15, 3.15, 3.15, 3.05,
     3.15, 3.25, 3.25, 3.25, 3.25, 3.25, 3.25, 3.05,
     3.15, 3.25, 3.30, 3.35, 3.35, 3.30, 3.25, 3.15,
     3.15, 3.30, 3.30, 3.35, 3.35, 3.30, 3.30, 3.15,
     3.15, 3.25, 3.35, 3.35, 3.35, 3.35, 3.25, 3.15,
     3.15, 3.35, 3.35, 3.35, 3.35, 3.35, 3.35, 3.15,
     3.15, 3.30, 3.25, 3.25, 3.25, 3.25, 3.30, 3.15,
     3.05, 3.15, 3.15, 3.15, 3.15, 3.15, 3.15, 3.05},
    //Rook
    {5.00, 5.00, 5.00, 5.00, 5.00, 5.00, 5.00, 5.00,
     5.05, 5.10, 5.10, 5.10, 5.10, 5.10, 5.10, 5.05,
     4.95, 5.00, 5.00, 5.00, 5.00, 5.00, 5.00, 4.95,
     4.95, 5.00, 5.00, 5.00, 5.00, 5.00, 5.00, 4.95,
     4.95, 5.00, 5.00, 5.00, 5.00, 5.00, 5.00, 4.95,
     4.95, 5.00, 5.00, 5.00, 5.00, 5.00, 5.00, 4.95,
     4.95, 5.00, 5.00, 5.00, 5.00, 5.00, 5.00, 4.95,
     5.00, 5.00, 5.00, 5.05, 5.05, 5.00, 5.00, 5.00},
    //Queen
    {8.80, 8.90, 8.90, 8.95, 8.95, 8.90, 8.90, 8.80,
     8.90, 9.00, 9.00, 9.00, 9.00, 9.00, 9.00, 8.90,
     8.90, 9.00, 9.05, 9.05, 9.05, 9.05, 9.00, 8.90,
     8.95, 9.00, 9.05, 9.05, 9.05, 9.05, 9.00, 8.95,
     9.00, 9.00, 9.05, 9.05, 9.05, 9.05, 9.00, 8.95,
     8.90, 9.05, 9.05, 9.05, 9.05, 9.05, 9.00, 8.90,
     8.90, 9.00, 9.05, 9.00, 9.00, 9.00, 9.00, 8.90,
     8.80, 8.90, 8.90, 8.95, 8.95, 8.90, 8.90, 8.80},
    //King middle game
    {-0.30, -0.40, -0.40, -0.50, -0.50, -0.40, -0.40, -0.30,
     -0.30, -0.40, -0.40, -0.50, -0.50, -0.40, -0.40, -0.30,
     -0.30, -0.40, -0.40, -0.50, -0.50, -0.40, -0.40, -0.30,
     -0.30, -0.40, -0.40, -0.50, -0.50, -0.40, -0.40, -0.30,
     -0.20, -0.30, -0.30, -0.40, -0.40, -0.30, -0.30, -0.20,
     -0.10, -0.20, -0.20, -0.20, -0.20, -0.20, -0.20, -0.10,
      0.20,  0.20,     0,     0,     0,     0,  0.20,  0.20,
      0.20,  0.30,  0.10,     0,     0,  0.10,  0.30,  0.20},
    //King endgame
    {-0.50, -0.40, -0.30, -0.20, -0.20, -0.30, -0.40, -0.50,
     -0.30, -0.20, -0.10,     0,     0, -0.10, -0.20, -0.30,
     -0.30, -0.10,  0.20,  0.30,  0.30,  0.20, -0.10, -0.30,
     -0.30, -0.10,  0.30,  0.40,  0.40,  0.30, -0.10, -0.30,
     -0.30, -0.10,  0.30,  0.40,  0.40,  0.30, -0.10, -0.30,
     -0.30, -0.10,  0.20,  0.30,  0.30,  0.20, -0.10, -0.30,
     -0.30, -0.30,     0,     0,     0,     0, -0.30, -0.30,
     -0.50, -0.30, -0.30, -0.30, -0.30, -0.30, -0.30, -0.50}
};

#endif // EVALUATE_H_
