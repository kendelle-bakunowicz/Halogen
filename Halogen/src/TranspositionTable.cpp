#include "TranspositionTable.h"

TranspositionTable::TranspositionTable()
{
	TTHits = 0;
	table.push_back(TTEntry());
}

TranspositionTable::~TranspositionTable()
{
}

bool TranspositionTable::CheckEntry(uint64_t key, int depth)
{
	if ((table.at(HashFunction(key)).GetKey() == key) && (table.at(HashFunction(key)).GetDepth() >= depth) && table.at(HashFunction(key)).GetCutoff() != EntryType::EMPTY_ENTRY)
		return true;
	return false;
}

uint64_t TranspositionTable::HashFunction(const uint64_t& key)
{
	return key % table.size();
	//return key & 0x3FFFFF;
}

bool TranspositionTable::CheckEntry(uint64_t key)
{
	if ((table.at(HashFunction(key)).GetKey() == key))
		return true;
	return false;
}

void TranspositionTable::AddEntry(const Move& best, uint64_t ZobristKey, int Score, int Depth, int distanceFromRoot, EntryType Cutoff)
{
	size_t hash = HashFunction(ZobristKey);

	if (Score > 9000)	//checkmate node
		Score += distanceFromRoot;
	if (Score < -9000)
		Score -= distanceFromRoot;

	if ((table.at(hash).GetKey() == EMPTY) || (table.at(hash).GetDepth() <= Depth) || (table.at(hash).IsAncient()) || table.at(hash).GetCutoff() == EntryType::EMPTY_ENTRY)
		table.at(hash) = TTEntry(best, ZobristKey, Score, Depth, Cutoff);
}

TTEntry TranspositionTable::GetEntry(uint64_t key)
{
	table.at(HashFunction(key)).SetAncient(false);
	return table.at(HashFunction(key));
}

void TranspositionTable::SetAllAncient()
{
	lock.lock();
	for (int i = 0; i < table.size(); i++)
	{
		table.at(i).SetAncient(true);
	}
	lock.unlock();
}

int TranspositionTable::GetCapacity()
{
	int count = 0;

	for (int i = 0; i < table.size(); i++)
	{
		if (!table.at(i).IsAncient())
			count++;
	}

	return count;
}

void TranspositionTable::ResetTable()
{
	lock.lock();
	TTHits = 0;

	for (int i = 0; i < table.size(); i++)
	{
		table.at(i) = TTEntry();
	}
	lock.unlock();
}

void TranspositionTable::SetSize(unsigned int MB)
{
	lock.lock();
	table.clear();
	unsigned int EntrySize = sizeof(TTEntry);
	unsigned int entries = MB * 1024 * 1024 / EntrySize;
	
	for (size_t i = 0; i < entries; i++)
	{
		table.push_back(TTEntry());
	}
	lock.unlock();
}

void TranspositionTable::PreFetch(uint64_t key)
{
	_mm_prefetch((char*)(&table[HashFunction(key)]), _MM_HINT_T0);
}

bool TranspositionTable::ExclusiveRights(uint64_t key)
{
	size_t hash = HashFunction(key);

	if (table.at(hash).IsInUse())
	{
		return false;
	}
	else
	{
		table.at(hash).MarkInUse();
		return true;
	}
}

void TranspositionTable::FreeExclusiveRights(uint64_t key)
{
	size_t hash = HashFunction(key);
	table.at(hash).ClearInUse();
}
