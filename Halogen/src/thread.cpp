#include "thread.h"

void LaunchSearchThread(Position position, int allowedTimeMs)
{
	InitSearch(allowedTimeMs);
	ThreadData data;

	for (int i = 0; i < THREAD_COUNT; i++)
	{
		std::thread SearchThread(SearchPosition, position, allowedTimeMs, data, i == 0, i);
		SearchThread.detach();
	}
}
