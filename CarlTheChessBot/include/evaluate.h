/**
    \file evaluate.h

    \brief File containing the functions used to evaluate any given position.
*/


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
double evaluateSide(const Position&, Color);


/**
    Function that evaluates the material situation for one of the sides.

    For example, if the given side has two pawns, a queen and two knights, the function returns 2*1 + 1*9 + 2*3 = 17 .

    \param position The given position
    \param color    The given color
    \return         The vector containing the requested moves
*/
double material(const Position&, Color);


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
    {0,  0,  0,  0,  0,  0,  0,  0,
     50, 50, 50, 50, 50, 50, 50, 50,
     10, 10, 20, 30, 30, 20, 10, 10,
     5,  5, 10, 25, 25, 10,  5,  5,
     0,  0,  0, 20, 20,  0,  0,  0,
     5, -5,-10,  0,  0,-10, -5,  5,
     5, 10, 10,-20,-20, 10, 10,  5,
     0,  0,  0,  0,  0,  0,  0,  0},
    //Knight
    {-50,-40,-30,-30,-30,-30,-40,-50,
     -40,-20,  0,  0,  0,  0,-20,-40,
     -30,  0, 10, 15, 15, 10,  0,-30,
     -30,  5, 15, 20, 20, 15,  5,-30,
     -30,  0, 15, 20, 20, 15,  0,-30,
     -30,  5, 10, 15, 15, 10,  5,-30,
     -40,-20,  0,  5,  5,  0,-20,-40,
     -50,-40,-30,-30,-30,-30,-40,-50},
    //Bishop
    {-20,-10,-10,-10,-10,-10,-10,-20,
     -10,  0,  0,  0,  0,  0,  0,-10,
     -10,  0,  5, 10, 10,  5,  0,-10,
     -10,  5,  5, 10, 10,  5,  5,-10,
     -10,  0, 10, 10, 10, 10,  0,-10,
     -10, 10, 10, 10, 10, 10, 10,-10,
     -10,  5,  0,  0,  0,  0,  5,-10,
     -20,-10,-10,-10,-10,-10,-10,-20},
    //Rook
    {0,  0,  0,  0,  0,  0,  0,  0,
     5, 10, 10, 10, 10, 10, 10,  5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
     0,  0,  0,  5,  5,  0,  0,  0},
    //Queen
    {-20,-10,-10, -5, -5,-10,-10,-20,
     -10,  0,  0,  0,  0,  0,  0,-10,
     -10,  0,  5,  5,  5,  5,  0,-10,
     -5,  0,  5,  5,  5,  5,  0, -5,
     0,  0,  5,  5,  5,  5,  0, -5,
     -10,  5,  5,  5,  5,  5,  0,-10,
     -10,  0,  5,  0,  0,  0,  0,-10,
     -20,-10,-10, -5, -5,-10,-10,-20},
    //King middle game
    {-30,-40,-40,-50,-50,-40,-40,-30,
     -30,-40,-40,-50,-50,-40,-40,-30,
     -30,-40,-40,-50,-50,-40,-40,-30,
     -30,-40,-40,-50,-50,-40,-40,-30,
     -20,-30,-30,-40,-40,-30,-30,-20,
     -10,-20,-20,-20,-20,-20,-20,-10,
     20, 20,  0,  0,  0,  0, 20, 20,
     20, 30, 10,  0,  0, 10, 30, 20},
    //King endgame
    {-50,-40,-30,-20,-20,-30,-40,-50,
     -30,-20,-10,  0,  0,-10,-20,-30,
     -30,-10, 20, 30, 30, 20,-10,-30,
     -30,-10, 30, 40, 40, 30,-10,-30,
     -30,-10, 30, 40, 40, 30,-10,-30,
     -30,-10, 20, 30, 30, 20,-10,-30,
     -30,-30,  0,  0,  0,  0,-30,-30,
     -50,-30,-30,-30,-30,-30,-30,-50}
};

#endif // EVALUATE_H_
