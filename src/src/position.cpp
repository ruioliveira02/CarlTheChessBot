#include "position.h"

#include <vector>

Position::Position()
{
    setStartingPosition();
}

Position::~Position()
{
    //dtor
}

void Position::setStartingPosition()
{
    EnPassant = -1;
    HalfMoves = 0;
    TotalMoves = 0;

    ToMove = Color::White;

    for(int i = 0; i < 2; i++)
        for(int j = 0; j < 2; j++)
            Castling[i][j] = true;

    WhiteOccupancy = 65535;
    BlackOccupancy = 18446462598732840960;

    PieceLocations[Piece::Rook][Color::White] = std::vector<short>({0,7});
    PieceLocations[Piece::Rook][Color::Black] = std::vector<short>({56,63});

    PieceLocations[Piece::Bishop][Color::White] = std::vector<short>({2,5});
    PieceLocations[Piece::Bishop][Color::Black] = std::vector<short>({58,61});

    PieceLocations[Piece::Knight][Color::White] = std::vector<short>({1,6});
    PieceLocations[Piece::Knight][Color::Black] = std::vector<short>({57,62});

    PieceLocations[Piece::Queen][Color::White] = std::vector<short>({3});
    PieceLocations[Piece::Queen][Color::Black] = std::vector<short>({59});

    PieceLocations[Piece::King][Color::White] = std::vector<short>({4});
    PieceLocations[Piece::King][Color::Black] = std::vector<short>({60});

    PieceLocations[Piece::Pawn][Color::White] = std::vector<short>({8,9,10,11,12,13,14,15});
    PieceLocations[Piece::Pawn][Color::Black] = std::vector<short>({48,49,50,51,52,53,54,55});
}
