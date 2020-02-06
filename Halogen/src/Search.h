#pragma once
#include "TranspositionTable.h"
#include "Position.h"
#include "MoveGeneration.h"
#include "Zobrist.h"
#include "Evaluate.h"
#include "Move.h"
#include "TimeManage.h"
#include <ctime>
#include <algorithm>
#include <utility>
#include <deque>
#include <xmmintrin.h>

extern TranspositionTable tTable;
const unsigned int THREAD_COUNT = 4;

struct Killer
{
	Move move[2];
};

struct ThreadData
{
	ThreadData();

	std::vector<Killer> KillerMoves;							//2 moves indexed by distanceFromRoot
	unsigned int HistoryMatrix[N_SQUARES][N_SQUARES];			//first index is from square and 2nd index is to square
};

void InitSearch(int allowedTimeMs);	//call before SearchPosition!
Move SearchPosition(Position position, int allowedTimeMs, ThreadData locals, bool writePermission, int threadID);
