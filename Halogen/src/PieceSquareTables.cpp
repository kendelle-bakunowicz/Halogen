#include "PieceSquareTables.h"
#include <iostream>

int PieceSquareTables[N_STAGES][N_PIECES][N_SQUARES];
void AddPieceSquareTableLayer(unsigned int stage, unsigned int piece, int layer[64]);			

int PawnSquareValuesMid[64] = {
  0,  0,  0,  0,  0,  0,  0,  0,
 25, 46, 32, 29, 37,108,107, 54,
 39, 52, 54, 43, 65, 86,112, 65,
 35, 53, 54, 74, 84, 92, 87, 51,
 52, 72, 70, 91,100, 89, 96, 57,
 22, 10, 20, 14, 27, 48, 16, 14,
 21, 28,  7, 13,  9, 13,  1,  6,
  0,  0,  0,  0,  0,  0,  0,  0,
};
int PawnSquareValuesEndGame[64] = { 
  0,  0,  0,  0,  0,  0,  0,  0,
103,101,109, 70, 82, 81, 79, 76,
101, 99, 91, 97, 95, 81, 75, 75,
111,104, 90, 84, 81, 76, 86, 87,
118,106, 92, 73, 79, 78, 97, 99,
103, 94, 63, 24, 14, 51, 68, 83,
 44, 41,  4,  0,  0,  0, 13, 31,
  0,  0,  0,  0,  0,  0,  0,  0,
};
int KnightSquareValues[64] = {
-15,-22, -9, -7, -4, -8,-19,-13,
 -5, -1, -1, -7, -4, -1,  0, -4,
-12, -1, -6, -1, -1, -8,  0,-15,
 -1,  0,  0,  3,  4,  1,  0,  0,
 -1,  0,  0, 18, 17,  5,  0,  0,
 -7,  0,  0,  0,  0,  0,  0, -1,
-18, -1,  0,  0,  0,  0, -1,-13,
-43,-11, -1, -4,  0,-10, -6,-27,
};
int BishopSquareValues[64] = {
-11, -1,-30, -2, -1,-26, -1, -1,
 -1, -3,  0, -4,  0,  0, 13, -1,
 -1, -1,  0,  5,  4,  3,  0, -1,
  0,  0,  0,  5,  0,  0, -1,  0,
 -1, -1,  0,  2,  1,  0,  0, -1,
 -1, -1, -1,  0,  0,  0,  0,  0,
  0,  0, -1,  0,  0,  0, -1,-15,
 -1,  0, -1,  0,  0,  0,  0,  0,
};
int RookSquareValues[64] = {
 -3, -1,  1,  1,  2,  0, -2, -4,
 -9, -1, -2, -1, -1, -1,  0,-15,
 -2, -1, -1, -1, -1,  0,  0, -1,
 -1, -1,  0,  0, -1,  0,  0, -1,
  0, -1,  0,  0,  0,  0,  0, -1,
  0,  0, -1,  0,  0,  0, -1, -1,
  0,  0,  0,  0,  0,  0, -1,  0,
  0,  0,  0,  0,  0,  0,  0,  0,
};
int QueenSquareValues[64] = {
 -1,  0,  0,  6,  0, -5,  0,  0,
 -1,  0,  0,  0,  0, -1,  0,  0,
  0, -1,  0,  0,  0,  0,  0,  0,
 -1,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0, -1,
 -1,  0,  0,  0,  0,  0,  0,  0,
 -4, -7,  0,  0,  0,  0,  0,  0,
 -1,  0,  0,  0,  0,  0, -1,  0,
};
int KingSquareMid[64] = {
  1, 70, 53,  0, 32, 12, 96, 71,
  0, 20,  8,  0,  4, 35,105, 85,
  0,  0,  1,  1,  1,  1, 35,  3,
  0,  0,  1,  0,  0,  0,  1,  1,
  0,  0,  0,  1,  0,  0,  0,  0,
  0,  1,  0,  0, -1,  1,  0,  0,
  0,  0,  0, -2, -6,  0,  0,  0,
  0,  0,  0, -9,-10,  0,  0,  0,
};
int KingSquareEndGame[64] = {
-13, -1,  0,  0,  0,  1,-11,-17,
  0,  0, 14, 19, 26, 16,  0, -3,
  0,  1, 16, 28, 36, 33, 20,  0,
  0,  0, 18, 30, 35, 35, 20,  0,
  0,  0, 14, 26, 24, 30, 21,  0,
  0,  0,  1,  0,  0, 23,  9,  0,
  0,  0,  0,  0,  0,  0,  0,  0,
-14,  0,  0,  0, -1,  0,  0,-10,
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
