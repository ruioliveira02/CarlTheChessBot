#include "game.h"
#include "movegeneration.h"
#include "magicmoves.h"
#include "evaluate.h"
#include <algorithm>



void game::initialize()
{
    initializeAllBitBoards();
    initmagicmoves();
}



game game::makeMove(Move move) const
{
    game g(*this);

    g.updateBitBoards(move);            //atualiza a ocupaçao da peça que se move
    g.position.EnPassant = -1;

    if(move.type == MoveType::Promotion)
    {
        g.assessCapture(move);
        g.promote(move);                //atualiza as PieceBitBoards do jogador a jogar
    }
    else if(move.type == MoveType::EnPassant)
    {
        g.captureEnpassant(move);       //atualiza a PieceBitBoard do peao que se move e a ocupacao e PieceBitBoard do peao que é comido
    }
    else if(move.type == MoveType::Castling)
    {
        g.castle(move);                 //atualiza os direitos de roque e a ocupaçao e PieceBitBoard da torre
        g.updatePieces(move);
    }
    else
    {
        g.assessCapture(move);          //atualiza a PieceBitBoard e a ocupaçao da peça que é comida (do adversário)
        g.updatePieces(move);           //atualiza a PieceBitBoard da peça que se mexeu
        g.updateCastlingRights(move);   //se as torres ou o rei se mexeu, atualiza os direitos de roque
        g.updateEnPassant(move);         //atualiza EnPassant
    }

    //TODO: halfMoves e totalMoves
    g.position.ToMove = oppositeColor(g.position.ToMove); //atualiza o ToMove
    return g;
}


bool game::exists(Piece piece) const
{
    return (position.PieceBitBoards[piece][Color::White] | position.PieceBitBoards[piece][Color::Black]);
}

bool game::isDraw() const
{
	//material insuficiente
    if (!exists(Piece::Queen) && !exists(Piece::Rook) && !exists(Piece::Pawn))
    {
        //TODO: verificar material

    }

    //TODO: detetar dead position (?)

    //TODO: 50 moves rule

    //TODO: empate por repeticao...

    return false;
}

void game::updateBitBoards(Move move)
{
    if(position.ToMove == Color::White)
        position.WhiteOccupancy ^= (1ULL << move.origin) | (1ULL << move.destiny);
    else
        position.BlackOccupancy ^= (1ULL << move.origin) | (1ULL << move.destiny);
}


void game::assessCapture(Move move)
{
    BitBoard aux = (1ULL << move.destiny);

    if (position.ToMove == Color::White)
    {
        if (position.BlackOccupancy & aux)
        {
            for (int i = 0; i < 6; i++)
            {
                if (position.PieceBitBoards[i][Color::Black] & aux)
                {
                    position.PieceBitBoards[i][Color::Black] ^= aux;
                    break;
                }
            }

            position.BlackOccupancy ^= aux;
        }
    }
    else
    {
        if (position.WhiteOccupancy & aux)
        {
            for (int i = 0; i < 6; i++)
            {
                if (position.PieceBitBoards[i][Color::White] & aux)
                {
                    position.PieceBitBoards[i][Color::White] ^= aux;
                    break;
                }
            }

            position.WhiteOccupancy ^= aux;
        }
    }
}

void game::updatePieces(Move move)
{
    position.PieceBitBoards[move.piece][position.ToMove] ^= (1ULL << move.origin) | (1ULL << move.destiny);
}


void game::promote(Move move)
{
    position.PieceBitBoards[Piece::Pawn][position.ToMove] ^= (1ULL << move.origin);
    position.PieceBitBoards[move.piece][position.ToMove] |= (1ULL << move.destiny);
}



void game::captureEnpassant(Move move)
{
    position.PieceBitBoards[Piece::Pawn][position.ToMove] ^= (1ULL << move.origin) | (1ULL << move.destiny);

    int enpassant = (position.ToMove == Color::White) ? (move.destiny - 8) : (move.destiny + 8);

    if (position.ToMove == Color::White)
        position.BlackOccupancy ^= (1ULL << enpassant);
    else
        position.WhiteOccupancy ^= (1ULL << enpassant);

    position.PieceBitBoards[Piece::Pawn][oppositeColor(position.ToMove)] ^= (1ULL << enpassant);
}


void game::castle(Move move)
{
    position.Castling[0][position.ToMove] = position.Castling[1][position.ToMove] = false;
    BitBoard rookChange = 0;

    switch (move.destiny)
    {
        case 2:     rookChange = 9ULL;                      break;
        case 6:     rookChange = 160ULL;                    break;
        case 58:    rookChange = 648518346341351424ULL;     break;
        case 62:    rookChange = 11529215046068469760ULL;   break;
    }

    if (position.ToMove == Color::White)
        position.WhiteOccupancy ^= rookChange;
    else
        position.BlackOccupancy ^= rookChange;

    position.PieceBitBoards[Piece::Rook][position.ToMove] ^= rookChange;
}


void game::updateCastlingRights(Move move)
{
    switch (move.origin)
    {
        case 0: //torre branca a1
        position.Castling[1][0] = false;
        return;

        case 4: //rei branco
        position.Castling[1][0] = false;

        case 7: //torre branca h1
        position.Castling[0][0] = false;
        return;

        case 56: //torre preta a8
        position.Castling[1][1] = false;
        return;

        case 60: //rei preto
        position.Castling[1][1] = false;

        case 63: //torre preta h8
        position.Castling[0][1] = false;
    }

    switch (move.destiny) //ao comer a torre adversária
    {
        case 0: //torre branca a1
        position.Castling[1][0] = false;
        return;

        case 7: //torre branca h1
        position.Castling[0][0] = false;
        return;

        case 56: //torre preta a8
        position.Castling[1][1] = false;
        return;

        case 63: //torre preta h8
        position.Castling[0][1] = false;
    }
}

void game::updateEnPassant(Move move)
{
    if (move.piece != Piece::Pawn)
        return;

    if (position.ToMove == Color::White)
    {
        if (move.origin / 8 == 1 && move.destiny / 8 == 3)
            position.EnPassant = move.destiny - 8;
    }
    else
    {
        if (move.origin / 8 == 6 && move.destiny / 8 == 4)
            position.EnPassant = move.destiny + 8;
    }
}


evaluation game::evaluate() const
{
   return evaluatePosition(position);
}
