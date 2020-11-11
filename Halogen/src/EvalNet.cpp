#include "EvalNet.h"

int pieceValueVector[N_STAGES][N_PIECE_TYPES] = { {91, 532, 568, 715, 1279, 5000},
                                                  {111, 339, 372, 638, 1301, 5000} };

constexpr int TEMPO = 10;

bool InsufficentMaterialEvaluation(const Position& position);

int EvaluatePositionNet(Position& position, EvalCacheTable& evalTable)
{
    int eval;

    if (!evalTable.GetEntry(position.GetZobristKey(), eval))
    {
        if (InsufficentMaterialEvaluation(position))
        {
            eval = 0;
        }
        else
        {
            eval = position.GetEvaluation() + (position.GetTurn() == WHITE ? TEMPO : -TEMPO);
        }

        evalTable.AddEntry(position.GetZobristKey(), eval);
    }

    return std::min(4000, std::max(-4000, eval));
}

int PieceValues(unsigned int Piece, GameStages GameStage)
{
    return pieceValueVector[GameStage][Piece % N_PIECE_TYPES];
}

bool DeadPosition(const Position& position)
{
    if ((position.GetPieceBB(WHITE_PAWN)) != 0) return false;
    if ((position.GetPieceBB(WHITE_ROOK)) != 0) return false;
    if ((position.GetPieceBB(WHITE_QUEEN)) != 0) return false;

    if ((position.GetPieceBB(BLACK_PAWN)) != 0) return false;
    if ((position.GetPieceBB(BLACK_ROOK)) != 0) return false;
    if ((position.GetPieceBB(BLACK_QUEEN)) != 0) return false;

    /*
    From the Chess Programming Wiki:
        According to the rules of a dead position, Article 5.2 b, when there is no possibility of checkmate for either side with any series of legal moves, the position is an immediate draw if
        - both sides have a bare king													1.
        - one side has a king and a minor piece against a bare king						2.
        - both sides have a king and a bishop, the bishops being the same color			Not covered
    */

    //We know the board must contain just knights, bishops and kings
    int WhiteBishops = GetBitCount(position.GetPieceBB(WHITE_BISHOP));
    int BlackBishops = GetBitCount(position.GetPieceBB(BLACK_BISHOP));
    int WhiteKnights = GetBitCount(position.GetPieceBB(WHITE_KNIGHT));
    int BlackKnights = GetBitCount(position.GetPieceBB(BLACK_KNIGHT));
    int WhiteMinor = WhiteBishops + WhiteKnights;
    int BlackMinor = BlackBishops + BlackKnights;

    if (WhiteMinor == 0 && BlackMinor == 0) return true;	//1
    if (WhiteMinor == 1 && BlackMinor == 0) return true;	//2
    if (WhiteMinor == 0 && BlackMinor == 1) return true;	//2

    return false;
}

bool InsufficentMaterialEvaluation(const Position& position)
{
    if ((position.GetPieceBB(WHITE_PAWN)) != 0) return false;
    if ((position.GetPieceBB(WHITE_ROOK)) != 0) return false;
    if ((position.GetPieceBB(WHITE_QUEEN)) != 0) return false;

    if ((position.GetPieceBB(BLACK_PAWN)) != 0) return false;
    if ((position.GetPieceBB(BLACK_ROOK)) != 0) return false;
    if ((position.GetPieceBB(BLACK_QUEEN)) != 0) return false;

    /*
    From the Chess Programming Wiki:
        According to the rules of a dead position, Article 5.2 b, when there is no possibility of checkmate for either side with any series of legal moves, the position is an immediate draw if
        - both sides have a bare king																																									1.
        - one side has a king and a minor piece against a bare king																																		1.
        - both sides have a king and a bishop, the bishops being the same color																															1.

        The bishops of different colors are not counted as an immediate draw, because of the possibility of a helpmate in the corner.
        Since this is unlikely given even a four ply search, we may introduce another class of drawn positions: those that cannot be claimed, but can be evaluated as draws:
        - two knights against the bare king																																								2.
        - both sides have a king and a minor piece each																																					1.
        - the weaker side has a minor piece against two knights																																			2.
        - two bishops draw against a bishop																																								4.
        - two minor pieces against one draw, except when the stronger side has a bishop pair																											3.
        Please note that a knight or even two knights against two bishops are not included here, as it is possible to win this ending.
    */

    //We know the board must contain just knights, bishops and kings
    int WhiteBishops = GetBitCount(position.GetPieceBB(WHITE_BISHOP));
    int BlackBishops = GetBitCount(position.GetPieceBB(BLACK_BISHOP));
    int WhiteKnights = GetBitCount(position.GetPieceBB(WHITE_KNIGHT));
    int BlackKnights = GetBitCount(position.GetPieceBB(BLACK_KNIGHT));
    int WhiteMinor = WhiteBishops + WhiteKnights;
    int BlackMinor = BlackBishops + BlackKnights;

    if (WhiteMinor <= 1 && BlackMinor <= 1) return true;												//1		
    if (WhiteMinor <= 1 && BlackKnights <= 2 && BlackBishops == 0) return true;							//2		KNvKNN, KBvKNN, KvKNN or combinations with less nights
    if (BlackMinor <= 1 && WhiteKnights <= 2 && WhiteBishops == 0) return true;							//2		
    if (WhiteMinor <= 1 && BlackMinor <= 2 && BlackBishops < 2) return true;							//3		
    if (BlackMinor <= 1 && WhiteMinor <= 2 && WhiteBishops < 2) return true;							//3
    if (WhiteBishops == 1 && BlackBishops == 2 && WhiteKnights == 0 && BlackKnights == 0) return true;	//4
    if (BlackBishops == 1 && WhiteBishops == 2 && BlackKnights == 0 && WhiteKnights == 0) return true;	//4

    return false;
}




