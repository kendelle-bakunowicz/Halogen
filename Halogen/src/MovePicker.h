#pragma once
#include "MoveGeneration.h"
#include "TranspositionTable.h"

bool GetHashMove(const Position& position, int distanceFromRoot, Move& move);
int see(Position& position, int square, bool side);
int seeCapture(Position& position, const Move& move); //Don't send this an en passant move!

enum class Stage 
{
	TTMOVE,
	GENERATE_OTHERS,
	ALL_OTHERS,
};

struct Killer
{
	Move move[2];
};

class MovePicker
{
public:
	MovePicker(int DistanceFromRoot, bool Quies = false);
	~MovePicker();

	bool Next(Position& position, Move& move, std::vector<Killer>& KillerMoves, unsigned int(&HistoryMatrix)[N_PLAYERS][N_SQUARES][N_SQUARES]);

private:
	void OrderMoves(Position& position, std::vector<Killer>& KillerMoves, unsigned int(&HistoryMatrix)[N_PLAYERS][N_SQUARES][N_SQUARES]);

	Stage stage;
	bool quies;
	int distanceFromRoot;
	int index;

	Move TTmove;
	std::vector<Move> moves;
};


