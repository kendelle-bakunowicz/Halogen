#pragma once
#include "Position.h"
#include "MoveGeneration.h"

struct Killer
{
	Move move[2];
};

class MoveGenerator
{
public:
	MoveGenerator();
	~MoveGenerator();

	bool GetNext(Move& move, Position& position, int distanceFromRoot, Move& hashMove, std::vector<Killer>& KillerMoves, unsigned int(&HistoryMatrix)[N_PLAYERS][N_SQUARES][N_SQUARES]);

private:
	enum class Stage
	{
		GEN_TT_MOVE,
		GIVE_TT_MOVE,
		GEN_ALL_OTHERS,
		GIVE_ALL_OTHERS,
	};

	std::vector<Move> moveList;
	Move TTmove;
	Stage stage;

	size_t index;
};

class QuiesMoveGenerator
{
public:
	QuiesMoveGenerator();
	~QuiesMoveGenerator();

	bool GetNext(Move& move, Position& position, int distanceFromRoot, Move& hashMove, std::vector<Killer>& KillerMoves, unsigned int(&HistoryMatrix)[N_PLAYERS][N_SQUARES][N_SQUARES]);

private:
	enum class Stage
	{
		GEN_ALL,
		GIVE_ALL,
	};

	std::vector<Move> moveList;
	Move TTmove;
	Stage stage;

	size_t index;
};


int see(Position& position, int square, bool side);
int seeCapture(Position& position, const Move& move); //Don't send this an en passant move!
