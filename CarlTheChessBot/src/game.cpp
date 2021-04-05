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

    #if STORE_HIERARCHY
    g.previous_positions.push(g.position.toFEN());
    g.previous_moves.push(move);
    #endif

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
        g.assessCapture(move);          //atualiza a avaliação, a PieceBitBoard e a ocupaçao/controlo da peça que é comida (do adversário)
        g.updatePieces(move);           //atualiza a avaliação e a PieceBitBoard da peça que se mexeu
        g.updateCastlingRights(move);   //se as torres ou o rei se mexeu, atualiza os direitos de roque
        g.updateEnPassant(move);        //atualiza EnPassant
    }

    //TODO: halfMoves e totalMoves
    g.updateBitBoards(move);                              //atualiza a ocupaçao da cor da peça que se move e o controlo das duas cores
    g.position.ToMove = oppositeColor(g.position.ToMove); //atualiza o ToMove

    if (!g.position.PieceBitBoards[Piece::King][g.position.ToMove])
        g.position.currentEvaluation = evaluation(0, oppositeColor(g.position.ToMove));

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

    //é preciso atualizar os dois por causa de cheques a descoberto e tal
    //position.WhiteControl = position.calculateControl(Color::White); //TODO: otimizar?
    //position.BlackControl = position.calculateControl(Color::Black);
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
                    position.currentEvaluation.subtract(value((Piece)i, Color::Black, move.destiny));
                    break;
                }
            }

            position.BlackOccupancy ^= aux;
            //position.BlackControl = position.calculateControl(Color::Black); //TODO: otimizar?
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
                    position.currentEvaluation.subtract(value((Piece)i, Color::White, move.destiny));
                    break;
                }
            }

            position.WhiteOccupancy ^= aux;
            //position.WhiteControl = position.calculateControl(Color::White); //TODO: otimizar?
        }
    }
}

void game::updatePieces(Move move)
{
    position.PieceBitBoards[move.piece][position.ToMove] ^= (1ULL << move.origin) | (1ULL << move.destiny);
    position.currentEvaluation.add(value(move.piece, position.ToMove, move.destiny) - value(move.piece, position.ToMove, move.origin));
}


void game::promote(Move move)
{
    position.PieceBitBoards[Piece::Pawn][position.ToMove] ^= (1ULL << move.origin);
    position.PieceBitBoards[move.piece][position.ToMove] |= (1ULL << move.destiny);
    position.currentEvaluation.add(value(move.piece, position.ToMove, move.destiny) - value(Piece::Pawn, position.ToMove, move.origin));
}


void game::captureEnpassant(Move move)
{
    Color color = position.ToMove;
    position.PieceBitBoards[Piece::Pawn][color] ^= (1ULL << move.origin) | (1ULL << move.destiny);

    Square enpassant = (color == Color::White) ? (move.destiny - 8) : (move.destiny + 8);

    if (color == Color::White)
        position.BlackOccupancy ^= (1ULL << enpassant);
    else
        position.WhiteOccupancy ^= (1ULL << enpassant);

    position.PieceBitBoards[Piece::Pawn][oppositeColor(color)] ^= (1ULL << enpassant);
    position.currentEvaluation.add(value(Piece::Pawn, color, move.destiny) - value(Piece::Pawn, color, move.origin)
                            - value(Piece::Pawn, oppositeColor(color), enpassant));
}


void game::castle(Move move)
{
    Color color = position.ToMove;
    position.Castling[0][color] = position.Castling[1][color] = false;
    BitBoard rookChange = 0;

    switch (move.destiny)
    {
        case 2:
        rookChange = 9ULL;
        position.currentEvaluation.add(value(Piece::Rook, color, 3) - value(Piece::Rook, color, 0));
        break;

        case 6:
        rookChange = 160ULL;
        position.currentEvaluation.add(value(Piece::Rook, color, 5) - value(Piece::Rook, color, 7));
        break;

        case 58:
        rookChange = 648518346341351424ULL;
        position.currentEvaluation.add(value(Piece::Rook, color, 59) - value(Piece::Rook, color, 56));
        break;

        case 62:
        rookChange = 11529215046068469760ULL;
        position.currentEvaluation.add(value(Piece::Rook, color, 61) - value(Piece::Rook, color, 63));
        break;
    }

    if (color == Color::White)
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
