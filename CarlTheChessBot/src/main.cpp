#include <iostream>
#include "position.h"
#include "movegeneration.h"
#include "magicmoves.h"

using namespace std;

int main()
{
    Position p1 = Position();
    p1.WhiteOccupancy = 0;
    p1.BlackOccupancy = 0;

    for(int i = 0; i < 6; i++)
        for(int j = 0; j < 2; j++)
            p1.PieceLocations[i][j] = vector<Square>();

    p1.PieceLocations[Piece::Pawn][Color::White] = vector<Square>({52});

    initmagicmoves();
    initializeAllBitBoards();

    std::vector<Move> moves = generateAllMoves(p1);


    for(int i = 0; i < moves.size(); i++)
    {
        cout << moves[i].origin << " " << moves[i].destiny << endl;
    }
    return 0;
}
