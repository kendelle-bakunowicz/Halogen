#pragma once
#include "Move.h"
#include "TranspositionTable.h"
#include "Position.h"
#include "MoveGeneration.h"

enum class Stage
{
	TT_MOVE,
	GOOD_CAPTURES,	//+ promotions
	KILLER_1,
	KILLER_2,
	BAD_CAPTURES,
	QUIET_MOVES
};

enum class MoveScore
{
	TT_MOVE = 10000000,
	PROMOTION = 9000000,
	GOOD_CAPTURE = 8000000,
	KILLER_1 = 7500000,
	KILLER_2 = 6500000,
	BAD_CAPTURE = 6000000,
	QUIET_MAXIMUM = 1000000,
	UNDERPROMOTION = -1
};

struct Killer
{
	Move move[2];
};

class MoveGenerator
{
public:
	MoveGenerator();
	~MoveGenerator();

	bool GetNext(Move& move, Position& position, int distanceFromRoot, const std::vector<Killer>& KillerMoves, unsigned int(&HistoryMatrix)[N_PLAYERS][N_SQUARES][N_SQUARES], bool Quiescent = false);

private:
	Stage state;

	std::vector<Move> loudMoves;
	std::vector<Move> quietMoves;
	Move TTmove;
	Move Killer1;
	Move Killer2;
	int LoudIndex;
	int QuietIndex;
}; 

struct Killer;

Move GetHashMove(const Position& position, int distanceFromRoot);

int see(Position& position, int square, bool side);
int seeCapture(Position& position, const Move& move); //Don't send this an en passant move!