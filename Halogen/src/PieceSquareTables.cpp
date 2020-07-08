#include "PieceSquareTables.h"
#include <iostream>

int PieceSquareTables[N_STAGES][N_PIECES][N_SQUARES];
void AddPieceSquareTableLayer(unsigned int stage, unsigned int piece, int layer[64]);			

int PawnSquareValuesMid[64] = {
0,0,0,0,0,0,0,0,-33,-35,-42,-42,-42,-7,0,-28,-20,-21,-27,-35,-24,-14,12,-19,-18,-16,-15,-13,-4,-4,-4,-26,-7,1,-6,10,19,26,2,-14,6,14,19,26,32,31,15,2,50,48,48,48,46,47,44,45,0,0,0,0,0,0,0,0

};
int PawnSquareValuesEndGame[64] = {
0,0,0,0,0,0,0,0,17,21,23,12,15,10,5,4,8,11,7,7,9,7,-3,-2,17,19,4,-1,1,3,9,5,23,19,13,1,-1,1,13,9,28,26,22,6,9,19,22,16,23,24,21,17,19,21,20,20,35,35,35,35,35,35,35,35

};
int KnightSquareValues[64] = {
-51,-43,-38,-37,-34,-40,-40,-51,-40,-27,-27,-24,-23,-24,-28,-38,-42,-20,-18,-3,-3,-16,-12,-39,-19,-1,-2,-4,-1,-1,3,-12,-14,-6,10,11,6,17,3,-3,-21,0,7,14,13,14,6,-19,-34,-17,2,2,3,-2,-18,-33,-55,-38,-27,-27,-24,-29,-38,-51

};
int BishopSquareValues[64] = {
-25,-13,-32,-23,-25,-28,-17,-23,-3,-16,-2,-15,-10,-12,-4,-5,-19,-1,-9,-3,-5,-5,-4,-11,-9,-5,-1,1,5,-4,0,-3,-9,-3,5,11,9,8,0,-11,-12,4,-1,6,10,0,10,-2,-9,0,0,-5,-5,-1,-9,-10,-19,-21,-10,-9,-9,-15,-9,-20

};
int RookSquareValues[64] = {
-19,-18,-18,-16,-16,-18,-24,-23,-35,-25,-24,-26,-24,-23,-26,-49,-24,-18,-20,-21,-19,-22,-15,-29,-12,-5,-7,-10,-7,-8,-3,-10,3,6,5,5,6,9,7,6,9,11,9,11,9,13,9,8,11,9,15,14,14,11,8,11,14,13,9,7,8,8,8,13

};
int QueenSquareValues[64] = {
-7,-14,-14,-16,-20,-29,-15,-16,-10,-10,-7,-11,-15,-13,-13,-11,-10,-10,-11,-15,-13,-14,-6,-6,-10,-2,-7,-14,-6,-5,-3,-4,-9,-6,1,-1,2,4,4,-2,-16,0,1,4,6,9,7,0,-11,-15,0,0,1,1,-1,-1,-12,-6,-6,-1,-3,-6,-7,-14

};
int KingSquareMid[64] = {
14,41,17,-33,-7,-39,22,21,21,14,-1,-15,-11,-18,15,18,-8,-13,-23,-22,-19,-17,-4,-7,-18,-25,-30,-42,-40,-29,-25,-17,-29,-38,-39,-50,-51,-40,-35,-27,-29,-38,-39,-50,-50,-39,-37,-29,-30,-39,-40,-49,-49,-39,-39,-30,-30,-40,-40,-50,-50,-40,-40,-30

};
int KingSquareEndGame[64] = {
-53,-46,-31,-31,-40,-22,-39,-63,-27,-18,-8,-7,-10,-7,-21,-39,-20,2,8,9,9,3,-4,-16,-23,2,21,23,24,17,3,-18,-25,-1,25,28,26,22,6,-19,-27,-4,21,25,26,17,0,-23,-29,-26,1,1,1,1,-25,-29,-50,-30,-30,-29,-31,-30,-30,-51

};

void InitializePieceSquareTable()
{
	for (int i = 0; i < N_STAGES; i++)
	{
		AddPieceSquareTableLayer(i, WHITE_KNIGHT, KnightSquareValues);
		AddPieceSquareTableLayer(i, BLACK_KNIGHT, KnightSquareValues);
		AddPieceSquareTableLayer(i, WHITE_BISHOP, BishopSquareValues);
		AddPieceSquareTableLayer(i, BLACK_BISHOP, BishopSquareValues);
		AddPieceSquareTableLayer(i, WHITE_QUEEN, QueenSquareValues);
		AddPieceSquareTableLayer(i, BLACK_QUEEN, QueenSquareValues);
		AddPieceSquareTableLayer(i, WHITE_ROOK, RookSquareValues);
		AddPieceSquareTableLayer(i, BLACK_ROOK, RookSquareValues);
	}

	AddPieceSquareTableLayer(MIDGAME, WHITE_KING, KingSquareMid);
	AddPieceSquareTableLayer(MIDGAME, BLACK_KING, KingSquareMid);
	AddPieceSquareTableLayer(ENDGAME, WHITE_KING, KingSquareEndGame);
	AddPieceSquareTableLayer(ENDGAME, BLACK_KING, KingSquareEndGame);

	AddPieceSquareTableLayer(MIDGAME, WHITE_PAWN, PawnSquareValuesMid);
	AddPieceSquareTableLayer(MIDGAME, BLACK_PAWN, PawnSquareValuesMid);
	AddPieceSquareTableLayer(ENDGAME, WHITE_PAWN, PawnSquareValuesEndGame);
	AddPieceSquareTableLayer(ENDGAME, BLACK_PAWN, PawnSquareValuesEndGame);
}

void AddPieceSquareTableLayer(unsigned int stage, unsigned int piece, int layer[64])
{
	if (ColourOfPiece(piece) == WHITE)
	{
		for (int i = 0; i < N_SQUARES; i++)
		{
			PieceSquareTables[stage][piece][i] = layer[i];
		}
	}

	if (ColourOfPiece(piece) == BLACK)
	{
		for (int i = 0; i < N_SQUARES; i++)
		{
			PieceSquareTables[stage][piece][i] = layer[GetPosition(GetFile(i), RANK_8 - GetRank(i))];
		}
	}
}
