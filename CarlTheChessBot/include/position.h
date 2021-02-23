#ifndef POSITION_H
#define POSITION_H

#include <vector>


typedef unsigned long long BitBoard;
typedef short Square;

enum Color
{
    White,
    Black
};

enum Piece
{
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};


enum MoveType
{
    Normal,
    EnPassant,
    Castling,
    Promotion
};


struct Move
{
    MoveType type;
    Square origin;
    Square destiny;
    Piece piece;

    Move(MoveType t, Square o, Square d, Piece p)
    {
        type = t;
        origin = o;
        destiny = d;
        piece = p;
    }
};


class Position
{
    public:
        Position();
        Position(const Position&);
        Position(std::vector<Square>[6][2],BitBoard,BitBoard,Color,bool[2][2],short,short,short);
        virtual ~Position();


        bool operator < (const Position& a)
        {
            return WhiteOccupancy < a.WhiteOccupancy;
        }

        bool operator == (const Position& a)
        {
            bool first = WhiteOccupancy == a.WhiteOccupancy && a.BlackOccupancy == BlackOccupancy && a.ToMove == ToMove;

            if(!first)
                return false;

            for(int i = 0; i < 2; i++)
                for(int j = 0; j < 2; j++)
                    if(a.Castling[i][j] != Castling[i][j])
                        return false;


            for(int i = 0; i < 6; i++)
                for(int j = 0; j < 2; j++)
                    if(a.PieceLocations[i][j] != PieceLocations[i][j])
                        return false;

            return true;
        }

    public:
        //First argument is the piece type; second one is the color, as per the enums above
        std::vector<Square> PieceLocations[6][2];
        BitBoard PieceBitBoards[6][2];

        BitBoard WhiteOccupancy;
        BitBoard BlackOccupancy;


        Color ToMove;

        //First argument is kingside/queenside, the second one is white/black
        bool Castling[2][2];

        //Square of the pawn that can be captured en passant (-1 if there is no such pawn)
        short EnPassant;

        //Number of half-moves since last capture or pawn push (50 move rule)
        short HalfMoves;

        //Total number of moves in the game
        short TotalMoves;
};





#endif // POSITION_H
