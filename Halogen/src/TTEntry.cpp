#include "TTEntry.h"

TTEntry::TTEntry()
{
	key = EMPTY;
	bestMove.data = 0;
	score = -1;
	depth = -1;
	cutoff = EntryType::EMPTY_ENTRY;
	halfmove = -1;
}

TTEntry::TTEntry(Move best, uint64_t ZobristKey, int Score, int Depth, int currentTurnCount, int distanceFromRoot, EntryType Cutoff)
{
	assert(Score < SHRT_MAX && Score > SHRT_MIN);
	assert(Depth < CHAR_MAX && Depth > CHAR_MIN);

	key = ZobristKey;
	bestMove.data = best.GetBits();
	score = static_cast<short>(Score);
	depth = static_cast<char>(Depth);
	cutoff = Cutoff;
	SetHalfMove(currentTurnCount, distanceFromRoot);
}


TTEntry::~TTEntry()
{
}

void TTEntry::MateScoreAdjustment(int distanceFromRoot)
{
	if (score > 9000)	//checkmate node
		score -= static_cast<short>(distanceFromRoot);
	if (score < -9000)
		score += static_cast<short>(distanceFromRoot);
}

void TTEntry::Reset()
{
	bestMove.data = 0;
	key = EMPTY;
	score = -1;
	depth = -1;
	cutoff = EntryType::EMPTY_ENTRY;
	halfmove = -1;
}

int8_t TTEntry::CalculateHashScore(int8_t currentGeneration)
{
	int8_t age = currentGeneration - GetHalfMove();
	return GetDepth() - 4 * (age >= 0 ? age : age + 16) + 4 * (GetCutoff() == EntryType::EXACT);
}

void TTBucket::Reset()
{
	entry[0].Reset();
	entry[1].Reset();
	entry[2].Reset();
	entry[3].Reset();
}
